#pragma once

#include "spdlog/spdlog.h"


class Log
{
public:
	static void														Init();

	inline static std::shared_ptr<spdlog::logger>&					GetLogger() {return sLogger; }

private:
	static std::shared_ptr<spdlog::logger>							sLogger;
};


// Log macros
#define RN_TRACE(...)		 ::Log::GetLogger()->trace(__VA_ARGS__)
#define RN_INFO(...)		 ::Log::GetLogger()->info(__VA_ARGS__)
#define RN_WARN(...)		 ::Log::GetLogger()->warn(__VA_ARGS__)
#define RN_ERROR(...)		 ::Log::GetLogger()->error(__VA_ARGS__)
#define RN_FATAL(...)		 ::Log::GetLogger()->fatal(__VA_ARGS__)
