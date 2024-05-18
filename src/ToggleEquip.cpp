#include "ToggleEquip.h"

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");
	REL::Relocation<std::uintptr_t> target{ REL::ID(332489), 0x17A };  // ActorEquipManager::CanEquip
	logger::info("Writing to address {:X}", target.address());
	stl::write_thunk_call<CanEquipHook>(target.address());

	logger::info("Installed equip/unequip dispatcher hook");
}

bool ToggleEquip::CanEquipHook::thunk(RE::ActorEquipManager* a_self, RE::Actor& a_actor, RE::InventoryInterface::Handle& item_handle, uint32_t unk)
{
	bool retVal = func(a_self, a_actor, item_handle, unk);

	logger::trace("Default return {}", retVal? "true" : "false");

	// Cant equip due to some reason, true return means can't equip
	if (retVal == true)
		return retVal;

	// Can equip check if already equipped
	const RE::BGSInventoryItem* item = RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem(item_handle.id);
	const bool isAlreadyEquipped = item->stackData->IsEquipped();

	logger::trace("Is already equipped? {}", item->stackData->IsEquipped()? "YES!" : "no skipping...");

	// Toggle the equip if already equipped
	if (isAlreadyEquipped) 
	{
		logger::info("Toggle equip 0x{:X}, Editor ID {}", uintptr_t(item->object->GetFormID()), item->object->GetFormEditorID());

		const RE::BSAutoReadLock l{ a_actor.inventoryList->rwLock };

		// For each of the 44 slots check if object is same
		for (int i = 0; i < static_cast<uint32_t>(RE::BIPED_OBJECT::kTotal); i++)
		{
			item->object->As<RE::BGSBipedObjectForm>();

			const auto& targetItem = a_actor.biped->object[i].parent;

			if (!targetItem.object)
			{
				logger::trace("targetItem object not found for slot {}", i);
				continue;
			}

			if (targetItem.object->GetFilledSlots() != item->object->GetFilledSlots())
			{
				logger::trace("Not the same object");
				continue;
			}

			logger::info("Found same object at slot index {}", i);

			auto inst = RE::BGSObjectInstance(targetItem.object, targetItem.instanceData.get());
			a_self->UnequipObject(&a_actor, &inst, 0xFFFFFFFF, nullptr, 0, true, true, true, true, nullptr);
		}
	}
	
	return retVal;
}
