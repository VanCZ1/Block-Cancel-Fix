#pragma once

namespace Hooks
{
	void Install();

	class Load3D
	{
	public:
		static void Install();

	private:
		static RE::NiAVObject* Thunk(RE::PlayerCharacter* a_this, bool a_backgroundLoading);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class MotionUpdate
	{
	public:
		static void Install();

	private:
		static bool Thunk(RE::TESObjectREFR* a_this, float a_deltaTime, RE::NiPoint3* a_translation, RE::NiPoint3* a_rotation, bool* a_result);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};
}
