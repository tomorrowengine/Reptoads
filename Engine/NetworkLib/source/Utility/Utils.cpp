#include "Utility/NetUtils.h"
#include <iostream>

#include <cassert>
#include <time.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Utils {

	
	
	
	
	void Timer::Begin() noexcept
	{
		start = std::chrono::system_clock::now();
	}

	void Timer::End() noexcept
	{
		end = std::chrono::system_clock::now();
		elapsedSeconds = end - start;
		endTime = std::chrono::system_clock::to_time_t(end);
	}

	void Timer::Print()
	{
		std::cout << "[Timer] " << "finished computation at " << ctime(&endTime)
			<< "elapsed time: " << elapsedSeconds.count() << " sec\n";
	}

	void Timer::Print(const char* msg)
	{
		std::cout << "[Timer] " << msg << " " << "finished computation at " << ctime(&endTime)
			<< "elapsed time: " << elapsedSeconds.count() << " sec\n";
	}

}
