#pragma once

using FavoritesEvent = RE::FavoriteMgr_Events::ComponentFavoriteEvent;

class ToggleEquip:
	public RE::BSTEventSink<RE::TESEquipEvent>,
	public RE::BSTEventSink<RE::BGSInventoryListEvent::Event>
{
public:
	
	static ToggleEquip* GetSingleton();
	
	void RegisterHooks();
	void GameLoaded();


	struct CantEquipHook
	{
		static bool thunk(RE::ActorEquipManager* a_self, RE::Actor& a_actor, RE::InventoryInterface::Handle& a_inventory_intfc, uint32_t unk);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent& evn, RE::BSTEventSource<RE::TESEquipEvent>*) override;
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::BGSInventoryListEvent::Event& evn, RE::BSTEventSource<RE::BGSInventoryListEvent::Event>*) override;
};
