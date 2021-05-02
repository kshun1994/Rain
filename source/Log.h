#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#include "spdlog/spdlog.h"


class Log
{
public:
	static void Init();
};

// Log macros
#define RN_DEBUG(...)		 SPDLOG_DEBUG(__VA_ARGS__)
#define RN_TRACE(...)		 SPDLOG_TRACE(__VA_ARGS__)
#define RN_INFO(...)		 SPDLOG_INFO(__VA_ARGS__)
#define RN_WARN(...)		 SPDLOG_WARN(__VA_ARGS__)
#define RN_ERROR(...)		 SPDLOG_ERROR(__VA_ARGS__)
#define RN_FATAL(...)		 SPDLOG_FATAL(__VA_ARGS__)