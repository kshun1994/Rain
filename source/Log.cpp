#include "rnpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


std::shared_ptr<spdlog::logger> Log::sLogger;

void Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	sLogger = spdlog::stdout_color_mt("LOG");
	sLogger->set_level(spdlog::level::trace);
}
