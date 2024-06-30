#pragma once
#include<chrono>
typedef std::chrono::steady_clock::time_point TimePoint;
namespace Time {
	inline const TimePoint ProgramStartTime = std::chrono::steady_clock::now();
	
	TimePoint GetTimePoint();

	float GetTime();

	float GetDuration(const TimePoint& p1, const TimePoint& p2);
}

#define MeasureTime(toAsign,code) TimePoint MeasureStartTime=Time::GetTimePoint(); code; toAsign=Time::GetDuration(MeasureStartTime,Time::GetTimePoint());}
