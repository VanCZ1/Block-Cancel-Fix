#pragma once

namespace Hooks
{
	void Install();

	class MotionUpdateHook
	{
	public:
		static void InstallHook();
		static inline RE::BSAnimationGraphManager* lastGraphManager{ nullptr };

	private:
		static bool Thunk(RE::Character* a_character, float a_deltaTime, RE::NiPoint3* a_translation, RE::NiPoint3* a_rotation, bool* a_result);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};
}
