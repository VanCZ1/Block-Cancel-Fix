#pragma once

inline void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) {
		SKSE::stl::report_and_fail("Failed to find standard logging directory."sv);
	}
	
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
	
    spdlog::set_default_logger(std::move(loggerPtr));

#ifdef NDEBUG
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);
#else
	spdlog::set_level(spdlog::level::debug);
	spdlog::flush_on(spdlog::level::debug);
#endif
}
