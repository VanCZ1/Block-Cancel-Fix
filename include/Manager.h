#pragma once

namespace Manager
{
	class PlayerState : public REX::Singleton<PlayerState>
	{
	public:
		std::atomic<bool> isBlockCanceling{ false };
	};

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
}
