#pragma once

class ToggleEquip :
	public RE::BSTEventSink<RE::TESEquipEvent>
{	
public:
	static void RegisterHooks();
	static void RegisterEvents();

private:
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

	RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent& a_event, RE::BSTEventSource<RE::TESEquipEvent>*) override;

private:
	static ToggleEquip singleton;
};
