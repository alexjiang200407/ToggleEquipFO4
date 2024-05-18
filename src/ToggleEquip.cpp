#include "ToggleEquip.h"

ToggleEquip* ToggleEquip::GetSingleton()
{
	static ToggleEquip singleton;
	return &singleton;
}

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");
	REL::Relocation<std::uintptr_t> target{ REL::ID(332489), 0x17A };  // ???
	logger::info("Writing to address {:X}", target.address());
	stl::write_thunk_call<CantEquipHook>(target.address());

	logger::info("Installed equip/unequip dispatcher hook");
}


void ToggleEquip::GameLoaded()
{
	logger::info("Game loaded");
}

bool ToggleEquip::CantEquipHook::thunk(RE::ActorEquipManager* a_self, RE::Actor& a_actor, RE::InventoryInterface::Handle& item_handle, uint32_t unk)
{
	logger::info("Is player character? {}", a_actor.formID == RE::PlayerCharacter::GetSingleton()->formID ? "yes" : "no");

	bool retVal = func(a_self, a_actor, item_handle, unk);

	logger::info("Default return {}", retVal? "true" : "false");

	// Cant equip due to some reason
	if (retVal == true)
		return retVal;

	// Can equip check if already equipped
	const RE::BGSInventoryItem* item = RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem(item_handle.id);
	const bool isAlreadyEquipped = item->stackData->IsEquipped();

	logger::info("0x{:X}", uintptr_t(item->object->GetFormID()));
	logger::info("Is already equipped? {}", item->stackData->IsEquipped()? "YES!" : "no");


	if (isAlreadyEquipped) 
	{			
		auto slotsFilledMask = item->object->GetFilledSlots();
		uint8_t slotsUsedMask = item->stackData->flags.underlying() & uint16_t(RE::BGSInventoryItem::Stack::Flag::kSlotMask);

		logger::info("Slot usage: 0b{:03b}", slotsUsedMask);
		
		logger::info("slotsFilledMask 0x{:06X}", slotsFilledMask);

		// For each slot from 3 to 1
		for (int i = 0; i < static_cast<uint32_t>(RE::BIPED_OBJECT::kTotal); i++)
		{
			// Slot is not being used
			//if (!((slotsUsedMask >> (1 * i)) & 0x1))
			//	continue;

			item->object->As<RE::BGSBipedObjectForm>();

			const auto& targetItem = a_actor.biped->object[i].parent;


			if (!targetItem.object)
			{
				logger::error("targetItem.object not found");
				continue;
			}

			if (targetItem.object->GetFilledSlots() != item->object->GetFilledSlots())
			{
				logger::info("Not the same object");
				continue;
			}

			auto inst = RE::BGSObjectInstance(targetItem.object, targetItem.instanceData.get());
			a_self->UnequipObject(&a_actor, &inst, 0xFFFFFFFF, nullptr, 0, true, true, true, true, nullptr);
		}
	}
	
	return retVal;
}
