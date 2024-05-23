#include "ToggleEquip.h"

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");
	
	REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(303130, 2248744), OFFSET(0x1B3, 0x1AE) };  // ActorEquipManager::ToggleEquipItem()
	
	stl::write_thunk_call<ToggleEquipItemHook>(target.address());
	
	logger::info("Writing to address {:X}", target.address());
	logger::info("Installed ToggleEquip dispatcher hook");
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

	// RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem REL::ID is yet to be found for CommonLibF4 NG
#ifndef NEXT_GEN
	const RE::BGSInventoryItem* item = RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem(a_itemHandle.id);

	if (!retVal)
	{
		logger::warn("Could not toggleEquip item 0x{:X} with editor id {}", item->object->GetFormID(), item->object->GetFormEditorID());
	}
	else
	{
		logger::trace("toggleEquip item 0x{:X} with editor id {}", item->object->GetFormID(), item->object->GetFormEditorID());
	}

#endif

	return retVal;
}
