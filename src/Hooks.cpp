#include "Hooks.h"
#include "Manager.h"

namespace Hooks
{
	void Install()
	{
		SKSE::log::info("Installing hooks...");
		MotionUpdateHook::InstallHook();
		SKSE::log::info("Hooks installed successfully.");
	}

	void MotionUpdateHook::InstallHook()
	{
		// 1.5.97: sub_1407020E0 call sub_1404E6360
		// 1.6.1170: sub_140797ED0 call sub_140540990
		// MovementTweenerAgentAnimationDriven::VTABLE[0], offset 0x11
		// call Character::ProcessMotionData
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(41160, 42246) };

		std::uintptr_t offset = 0x111;
		if (REL::Module::IsAE()) {
			offset = 0xFF;
		}

		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(14);
		originalFunction = trampoline.write_call<5>(target.address() + offset, Thunk);
	}

	bool MotionUpdateHook::Thunk(RE::Character* a_character, float a_deltaTime, RE::NiPoint3* a_translation, RE::NiPoint3* a_rotation, bool* a_result)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		bool isPlayer = (player && a_character == player);

		if (isPlayer) {
			auto handler = Manager::PlayerAnimationHandler::GetSingleton();

			// Check if the animation graph manager has changed (e.g., using ShowRaceMenu)
			RE::BSAnimationGraphManagerPtr manager;
			player->GetAnimationGraphManager(manager);
			if (manager && manager.get() != lastGraphManager) {
				handler->isRegistered = false;
				lastGraphManager = manager.get();
			}

			if (!handler->isRegistered) {
				handler->Register();
			}
		}

		auto result = originalFunction(a_character, a_deltaTime, a_translation, a_rotation, a_result);

		if (isPlayer && Manager::PlayerState::GetSingleton()->isBlockCanceling) {
			a_translation->x = 0.0f;
			a_translation->y = 0.0f;
		}

		return result;
	}
}
