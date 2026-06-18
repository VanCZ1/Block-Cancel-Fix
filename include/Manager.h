#pragma once

namespace Manager
{
	class PlayerAnimationHandler : 
		public REX::Singleton<PlayerAnimationHandler>,
		public RE::BSTEventSink<RE::BSAnimationGraphEvent>
	{
	public:
		void Register();
		std::atomic<bool> isRegistered{ false };

	private:
		RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>*) override;
	};

	class PlayerAnimationState : public REX::Singleton<PlayerAnimationState>
	{
	public:
		std::atomic<bool> isBlockCanceling{ false };
	};
}
