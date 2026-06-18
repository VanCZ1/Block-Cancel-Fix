#include "Log.h"
#include "Hooks.h"
#include "Manager.h"

SKSEPluginInfo(
	.Version = REL::Version{ Version::MAJOR, Version::MINOR, Version::PATCH },
	.Name = Version::PROJECT,
	.Author = "VanCZ1"sv,
	.SupportEmail = ""sv,
	.StructCompatibility = SKSE::StructCompatibility::Independent,
	.RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
)

void Load()
{
	Hooks::Install();
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kPostPostLoad:
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
        break;
	case SKSE::MessagingInterface::kNewGame:
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	SetupLog();
	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	logger::info("{} v{}", plugin->GetName(), plugin->GetVersion());
	const auto gameVersion = a_skse->RuntimeVersion().string();
	logger::info("Game version: {}", gameVersion);

	const auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}
	Load();

	return true;
}
