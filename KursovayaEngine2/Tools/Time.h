#pragma once
#include"DLL.h"
#include<chrono>
namespace Time {

	typedef std::chrono::steady_clock::time_point TimePoint;

	inline const TimePoint ProgramStartTime = std::chrono::steady_clock::now();
	
	DLLTREATMENT TimePoint GetTimePoint();

	DLLTREATMENT float GetTime();

	DLLTREATMENT float GetDuration(const TimePoint& start, const TimePoint& end);

	DLLTREATMENT void Wait(float seconds);
}

#define MeasureTime(toAsign,code) TimePoint MeasureStartTime=Time::GetTimePoint(); code; toAsign=Time::GetDuration(MeasureStartTime,Time::GetTimePoint());}
