#pragma once

using FavoritesEvent = RE::FavoriteMgr_Events::ComponentFavoriteEvent;

class ToggleEquip:
	public RE::BSTEventSink<RE::TESEquipEvent>,
	public RE::BSTEventSink<RE::BGSInventoryListEvent::Event>
{
public:
	
	static ToggleEquip* GetSingleton();
	
	void RegisterHooks();

	struct EquipEventDispatcherHook
	{
		static void thunk(RE::TESEquipEvent& a_event);
		static inline REL::Relocation<decltype(&thunk)> func;
	};

	virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent& evn, RE::BSTEventSource<RE::TESEquipEvent>*) override;
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::BGSInventoryListEvent::Event& evn, RE::BSTEventSource<RE::BGSInventoryListEvent::Event>*) override;
};
