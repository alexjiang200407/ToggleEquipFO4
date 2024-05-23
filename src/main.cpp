#include "ToggleEquip.h"
#include <string>
#include "Version.h"


spdlog::level::level_enum GetLogLevel()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	std::ostringstream oss;

	oss << "./Data/F4SE/Plugins/" << Version::PROJECT << ".ini";

	if (ini.LoadFile(oss.str().c_str()) < 0)
		return spdlog::level::level_enum::trace;

	std::string logLevelStr = ini.GetValue("Debug", "logLevel", "0");
	size_t digits;

	int logLevel = std::stoi(logLevelStr, &digits);

	if (digits != logLevelStr.size() || logLevel > spdlog::level::level_enum::n_levels || logLevel < 0)
		return spdlog::level::level_enum::trace;

	return static_cast<spdlog::level::level_enum>(logLevel);
}

void MessageCallback(F4SE::MessagingInterface::Message* msg)
{
	switch (msg->type) {
	case F4SE::MessagingInterface::kGameDataReady:
		RE::ConsoleLog::GetSingleton()->PrintLine("ToggleEquip has been loaded");
		break;
	default:
		break;
	}
}

void InitializeLog()
{
	auto path = logger::log_directory();
	if (!path) {
		F4SE::stl::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

	auto logLevel = GetLogLevel();

	log->set_level(logLevel);
	log->flush_on(logLevel);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%H:%M:%S:%e] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

#ifdef NEXT_GEN

extern "C" DLLEXPORT constinit auto F4SEPlugin_Version = []() noexcept {
	F4SE::PluginVersionData data{};

	data.PluginVersion({ Version::MAJOR, Version::MINOR, Version::PATCH });
	data.PluginName(Version::PROJECT.data());
	data.AuthorName("shdowraithe101");
	data.UsesAddressLibrary(true);
	data.UsesSigScanning(false);
	data.IsLayoutDependent(false);
	data.HasNoStructUse(false);
	data.CompatibleVersions({ F4SE::RUNTIME_LATEST });

	return data;
}();

#else

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_f4se->IsEditor()) {
		logger::critical("loaded in editor");
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_162) {
		logger::critical("unsupported runtime v{}", ver.string());
		return false;
	}

	return true;
}

#endif

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);
	InitializeLog();

	logger::info("ToggleItems has been initialized by F4SE bitch");

	ToggleEquip::RegisterHooks();
	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}
