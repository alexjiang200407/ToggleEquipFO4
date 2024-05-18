#pragma once

namespace ToggleEquip
{	
	void RegisterHooks();

	struct CanEquipHook
	{
		static bool thunk(RE::ActorEquipManager* a_self, RE::Actor& a_actor, RE::InventoryInterface::Handle& a_inventory_intfc, uint32_t unk);
		static inline REL::Relocation<decltype(thunk)> func;
	};
};
