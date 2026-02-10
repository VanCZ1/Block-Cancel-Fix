#include "Manager.h"

namespace Manager
{
	void PlayerAnimationHandler::Register()
	{
		if (isRegistered) {
			return;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		player->RemoveAnimationGraphEventSink(this);
		auto isSuccess = player->AddAnimationGraphEventSink(this);
		if (isSuccess) {
			isRegistered = true;
		}
	}

	RE::BSEventNotifyControl PlayerAnimationHandler::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto playerState = PlayerState::GetSingleton();
		const std::string_view tagStr = a_event->tag.c_str();
		bool isBlockStart = (tagStr == "blockStartOut");
		bool isAttackStop = (tagStr == "attackStop");

		if (isBlockStart) {
			bool isAttacking = false;
			player->GetGraphVariableBool("IsAttacking", isAttacking);
			if (isAttacking) {
				playerState->isBlockCanceling = true;
			}
		} else if (isAttackStop) {
			if (playerState->isBlockCanceling) {
				playerState->isBlockCanceling = false;
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}
}
