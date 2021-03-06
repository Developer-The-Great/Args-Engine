#pragma once
#include <core/engine/engine.hpp>
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <iostream>

/**
 * @file entry_point.hpp
 * @brief When ARGS_ENTRY is defined, this file will create a function with signature main(int,char**) -> int
 *        implementing the common main function of a c++ program.
 * @note When defining ARGS_ENTRY do no create your own CRT_STARTUP such as main()->int, main(int,char**)->int, wmain(), etc...
 * @note When using ARGS_ENTRY you must instead implement reportModules(args::core::Engine*).
 * @note When not using ARGS_ENTRY you must call creation and initialization of the engine manually.
 */


 /**@brief Reports engine modules to the engine, must be implemented by you.
  * @param [in] engine The engine object as ptr *
  * @ref args::core::Engine::reportModule<T,...>()
  */
extern void reportModules(args::core::Engine* engine);

#if defined(ARGS_ENTRY)

	#if (defined(ARGS_HIGH_PERFORMANCE) && defined(ARGS_WINDOWS))
		__declspec(dllexport) DWORD NvOptimusEnablement = 0x0000001;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	#endif

	int main(int argc, char** argv)
	{
	#if (!defined(ARGS_DEBUG) && defined(ARGS_WINDOWS) && !defined(ARGS_KEEP_CONSOLE))
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	#endif

	#if defined(ARGS_HIGH_PERFORMANCE)
		#if defined(ARGS_WINDOWS)
		if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
		{
			DWORD error = GetLastError();
			std::cout << "==============================================================" << std::endl;
			std::cout << "| Failed to enter real-time performance mode, error: " << error << " |" << std::endl;
			std::cout << "==============================================================" << std::endl;
		}
		else
		{
			std::cout << "=======================================" << std::endl;
			std::cout << "| Entered real-time performance mode. |" << std::endl;
			std::cout << "=======================================" << std::endl;
		}

		#elif defined(ARGS_LINUX)
		pid_t pid = getpid();
		if (setpriority(PRIO_PROCESS, pid, sched_get_priority_max(sched_getscheduler(pid))) == -1)
		{
			int errornum = errno;
			cstring error;

			switch (errornum)
			{
			case ESRCH:
				error = "ESRCH";
				break;
			case EINVAL:
				error = "EINVAL";
				break;
			case EPERM:
				error = "EPERM";
				break;
			case EACCES:
				error = "EACCES";
				break;
			default:
				error = std::to_string(errornum).c_str();
				break;
			}

			std::cout << "=============================================================" << std::endl;
			std::cout << "| Failed to enter real-time performance mode, error: " << error << " |" << std::endl;
			std::cout << "=============================================================" << std::endl;
		}
		else
		{
			std::cout << "=======================================" << std::endl;
			std::cout << "| Entered real-time performance mode. |" << std::endl;
			std::cout << "=======================================" << std::endl;
		}
		#endif
	#endif

		//try
		{
			args::core::Engine engine;

			reportModules(&engine);
			std::cout << "==========================" << std::endl;
			std::cout << "| Initializing engine... |" << std::endl;
			std::cout << "==========================" << std::endl;
			engine.init();

			std::cout << "==============================" << std::endl;
			std::cout << "| Entering main engine loop. |" << std::endl;
			std::cout << "==============================" << std::endl;
			engine.run();

	#if defined(ARGS_DEBUG) || defined(ARGS_KEEP_CONSOLE)
			std::cout << "Press any key to exit." << std::endl;
			std::cin.ignore();
	#endif

		}
		/*catch (const args::core::exception& e)
		{
	#if (defined(ARGS_WINDOWS) && !defined(ARGS_KEEP_CONSOLE))
			::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	#endif

			std::cout << "Encountered exception:" << std::endl;
			std::cout << "  msg:     \t" << e.what() << std::endl;
			std::cout << "  file:    \t" << e.file() << std::endl;
			std::cout << "  line:    \t" << e.line() << std::endl;
			std::cout << "  function:\t" << e.func() << std::endl;

	#if defined(ARGS_DEBUG)
			throw e;
	#else
			std::cout << "Press any key to exit." << std::endl;
			std::cin.ignore();
	#endif
		}
		catch (const std::exception& e)
		{
	#if (defined(ARGS_WINDOWS) && !defined(ARGS_KEEP_CONSOLE))
			::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	#endif

			std::cout << e.what() << std::endl;

	#if defined(ARGS_DEBUG)
			throw e;
	#else
			std::cout << "Press any key to exit." << std::endl;
			std::cin.ignore();
	#endif
		}*/

		return 0;
	}
#endif
