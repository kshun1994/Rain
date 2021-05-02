#include "rnpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


void Log::Init()
{
	spdlog::set_pattern("%^[%T] [%!] [Line %#]: %v%$");

	spdlog::set_level(spdlog::level::trace);
}
