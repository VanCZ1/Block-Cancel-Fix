#include "Log.h"
#include "Hooks.h"
#include "Manager.h"

SKSEPluginInfo(
	.Version = { 1, 0, 0, 0 },
	.Name = "BlockCancelFix"sv,
	.Author = "VanCZ1"sv,
	.SupportEmail = ""sv,
	.StructCompatibility = SKSE::StructCompatibility::Independent,
	.RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
)

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kPreLoadGame:

		Manager::PlayerState::GetSingleton()->isBlockCanceling = false;

		break;
	case SKSE::MessagingInterface::kPostLoadGame:
        break;
	case SKSE::MessagingInterface::kNewGame:
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
	
	SetupLog();

	auto plugin = SKSE::PluginDeclaration::GetSingleton();
	SKSE::log::info("{} v{}", plugin->GetName(), plugin->GetVersion());
	auto gameVersion = skse->RuntimeVersion().string();
	SKSE::log::info("Game version: {}", gameVersion);

	Hooks::Install();

    auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	SKSE::log::info("Plugin loaded successfully.");

    return true;
}
