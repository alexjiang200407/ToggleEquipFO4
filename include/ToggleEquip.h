#pragma once

namespace ToggleEquip
{	
	void RegisterHooks();

	struct ToggleEquipItemHook
	{
		static bool thunk(
			RE::ActorEquipManager* a_self,
			RE::Actor* a_actor,
			RE::InventoryInterface::Handle& a_itemHandle,
			uint32_t a_stackID,
			RE::BGSEquipSlot* a_equipSlot,
			bool a_allowUnequip,
			bool a_unk);
		static inline REL::Relocation<decltype(thunk)> func;
	};
};
