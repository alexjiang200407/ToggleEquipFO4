#include "ToggleEquip.h"


ToggleEquip ToggleEquip::singleton;

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");
	
	REL::Relocation<std::uintptr_t> target{ REL::ID(303130), 0x1B3 };  // ActorEquipManager::ToggleEquipItem()
	stl::write_thunk_call<ToggleEquipItemHook>(target.address());
	
	logger::info("Writing to address {:X}", target.address());
	logger::info("Installed ToggleEquip dispatcher hook");
}

RE::BSEventNotifyControl ToggleEquip::ProcessEvent(const RE::TESEquipEvent& a_event, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	if (a_event.a != RE::PlayerCharacter::GetSingleton())
		return RE::BSEventNotifyControl::kContinue;

	auto* item = RE::TESForm::GetFormByID(a_event.formId);

	if (!item)
	{
		logger::error("Could not find item with id 0x{:X}", a_event.formId);
		return RE::BSEventNotifyControl::kContinue;
	}

	logger::info("Processing {} event for object 0x{:X} {}", a_event.isEquip ? "EQUIP" : "UNEQUIP", a_event.formId, item->GetFormEditorID());

	return RE::BSEventNotifyControl::kContinue;
}

void ToggleEquip::RegisterEvents()
{
	logger::info("Registering Event sink");
	RE::EquipEventSource::GetSingleton()->RegisterSink(&singleton);
}

bool ToggleEquip::ToggleEquipItemHook::thunk(
	RE::ActorEquipManager* a_self, 
	RE::Actor* a_actor, 
	RE::InventoryInterface::Handle& a_itemHandle, 
	uint32_t a_stackID, 
	RE::BGSEquipSlot* a_equipSlot, 
	bool a_allowUnequip,
	bool a_unk)
{
	// Not Player Character do as default
	if (a_actor != RE::PlayerCharacter::GetSingleton())
		return func(a_self, a_actor, a_itemHandle, a_stackID, a_equipSlot, a_allowUnequip, a_unk);	

	bool retVal = func(a_self, a_actor, a_itemHandle, a_stackID, a_equipSlot, true, a_unk);	
	if (!retVal)
	{
		const RE::BGSInventoryItem* item = RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem(a_itemHandle.id);
		logger::warn("Could not toggleEquip item 0x{:X} with editor id {}", item->object->GetFormID(), item->object->GetFormEditorID());
	}

	return retVal;
}
