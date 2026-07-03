#include "Hooks.h"
#include "Manager.h"

namespace Hooks
{
	void Install()
	{
		logger::info("Installing hooks...");
		SKSE::AllocTrampoline(14);
		Load3D::Install();
		MotionUpdate::Install();
		logger::info("Hooks installed successfully.");
	}

	void Load3D::Install()
	{
		REL::Relocation<std::uintptr_t> vTable{ RE::PlayerCharacter::VTABLE[0] };
		originalFunction = vTable.write_vfunc(0x6A, Thunk);
	}

	RE::NiAVObject* Load3D::Thunk(RE::PlayerCharacter* a_this, bool a_backgroundLoading)
	{
		auto result = originalFunction(a_this, a_backgroundLoading);

		Manager::PlayerAnimationHandler::GetSingleton()->isRegistered = false;
		Manager::PlayerAnimationState::GetSingleton()->isBlockCanceling = false;

		return result;
	}

	void MotionUpdate::Install()
	{
		// 1.5.97: sub_1407020E0 call sub_1404E6360
		// 1.6.1170: sub_140797ED0 call sub_140540990
		// RE::VTABLE_MovementTweenerAgentAnimationDriven[1], offset 0x4
		// call ProcessMotionData
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(41160, 42246), REL::VariantOffset(0x111, 0xFF, 0x111) };
		
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_call<5>(target.address(), Thunk);
	}

	bool MotionUpdate::Thunk(RE::TESObjectREFR* a_this, float a_deltaTime, RE::NiPoint3* a_translation, RE::NiPoint3* a_rotation, bool* a_result)
	{
		auto result = originalFunction(a_this, a_deltaTime, a_translation, a_rotation, a_result);

		if (a_this && a_this->IsPlayerRef()) {
			auto handler = Manager::PlayerAnimationHandler::GetSingleton();
			if (!handler->isRegistered) {
				handler->Register();
			}

			if (Manager::PlayerAnimationState::GetSingleton()->isBlockCanceling) {
				a_translation->x = 0.0f;
				a_translation->y = 0.0f;
			}
		}

		return result;
	}
}
