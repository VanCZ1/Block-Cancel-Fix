#include "Manager.h"

namespace Manager
{
	void PlayerAnimationHandler::Register()
	{
		if (isRegistered) {
			return;
		}

		const auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		player->RemoveAnimationGraphEventSink(this);
		if (player->AddAnimationGraphEventSink(this)) {
			isRegistered = true;
		}
	}

	RE::BSEventNotifyControl PlayerAnimationHandler::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		const auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto state = PlayerAnimationState::GetSingleton();
		bool isBlockStart = (a_event->tag == "blockStartOut");
		bool isAttackStop = (a_event->tag == "attackStop");

		if (isBlockStart) {
			bool isAttacking = false;
			player->GetGraphVariableBool("IsAttacking", isAttacking);
			if (isAttacking) {
				state->isBlockCanceling = true;
			}
		} else if (isAttackStop) {
			if (state->isBlockCanceling) {
				state->isBlockCanceling = false;
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}
}
