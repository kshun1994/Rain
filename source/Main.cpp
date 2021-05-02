#include "rnpch.h"
#include "Application.h"


int main()
{
	Log::Init(); // Initialize logging

	//#ifdef RN_DEBUG
	//	spdlog::set_level(spdlog::level::debug);
	//#endif // RN_DEBUG

	try
	{
		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
} 