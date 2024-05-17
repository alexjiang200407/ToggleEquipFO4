#include "ToggleEquip.h"

ToggleEquip* ToggleEquip::GetSingleton()
{
	static ToggleEquip singleton;
	return &singleton;
}

void ToggleEquip::RegisterHooks()
{
	logger::info("Installing hooks");

	//REL::Relocation<std::uintptr_t> target1{ REL::ID(702912), 0x0 };  // ActorEquipManager::Unequip<SpellItem*,MagicEquipParams>(Actor*,SpellItem*,MagicEquipParams&)
	//REL::Relocation<std::uintptr_t> target2{ REL::ID(134742), 0x0 };  // Actor::ShouldQueueEquipObject(TESBoundObject*)
	//REL::Relocation<std::uintptr_t> target3{ REL::ID(691570), 0x0 };  // Actor::PollItemEquip(bool)
	REL::Relocation<std::uintptr_t> target{ REL::ID(332489), 0x17A };  // ???

	//logger::info("{:X}", target1.address());
	//logger::info("{:X}", target2.address());
	//logger::info("{:X}", target3.address());
	logger::info("Writing to address {:X}", target.address());
	stl::write_thunk_call<CantEquipHook>(target.address());

	logger::info("Installed equip/unequip dispatcher hook");
}


void ToggleEquip::GameLoaded()
{
	RE::EquipEventSource::GetSingleton()->RegisterSink(this);

	RE::TESEquipEvent event{};
	event.a = RE::PlayerCharacter::GetSingleton();

	logger::info("Game loaded");
}

RE::BSEventNotifyControl ToggleEquip::ProcessEvent(const RE::TESEquipEvent& evn, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	if (evn.a != RE::PlayerCharacter::GetSingleton())
		return RE::BSEventNotifyControl::kContinue;

	//logger::info("Player {} something!", evn.isEquip ? "equipped" : "unequipped");

	//logger::info("0x{:x} 0x{:x} {}", evn.formId, evn.refFormID, evn.unk10);

	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ToggleEquip::ProcessEvent(const RE::BGSInventoryListEvent::Event& evn, RE::BSTEventSource<RE::BGSInventoryListEvent::Event>*)
{
	if (evn.owner != RE::PlayerCharacter::GetSingleton()->GetHandle())
		return RE::BSEventNotifyControl::kContinue;
		
	logger::info("Player inventory event");



	return RE::BSEventNotifyControl::kContinue;
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

	item->object->GetBaseInstanceData();
	RE::BGSObjectInstance* f = RE::fallout_cast<RE::BGSObjectInstance*>((RE::TESObjectREFR*)nullptr);

	if (isAlreadyEquipped)
	{
		a_self->UnequipObject(&a_actor, , )
	}
	

	

	//a_actor.currentProcess->middleHigh->equippedItemsLock.unlock();



	return retVal;
}
