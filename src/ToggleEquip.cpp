#include "ToggleEquip.h"

ToggleEquip* ToggleEquip::GetSingleton()
{
	static ToggleEquip singleton;
	return &singleton;
}

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");


	REL::Relocation<std::uintptr_t> target{ REL::ID(706150), 0x0 };  // BSTEventSource<TESEquipEvent>::Notify(TESEquipEvent&)
	stl::write_thunk_call<EquipEventDispatcherHook>(target.address());

	logger::info("Installed equip/unequip dispatcher hook");
}


RE::BSEventNotifyControl ToggleEquip::ProcessEvent(const RE::TESEquipEvent& evn, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	if (evn.a == RE::PlayerCharacter::GetSingleton())
		logger::info("Player {} something!", evn.isEquip ? "equipped" : "unequipped");

	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ToggleEquip::ProcessEvent(const RE::BGSInventoryListEvent::Event& evn, RE::BSTEventSource<RE::BGSInventoryListEvent::Event>*)
{
	if (evn.owner == RE::PlayerCharacter::GetSingleton()->GetHandle())
		logger::info("Player inventory event");

	return RE::BSEventNotifyControl::kContinue;
}

void ToggleEquip::EquipEventDispatcherHook::thunk(RE::TESEquipEvent& a_event)
{
	logger::info("Equip Event dispatched");

	func(a_event);
}
