#include"Time.h"
#include<thread>

Time::TimePoint Time::GetTimePoint() {
	return std::chrono::steady_clock::now();
}

float Time::GetTime() {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - ProgramStartTime).count();
}

float Time::GetDuration(const TimePoint& start, const TimePoint& end) {
	return std::chrono::duration<float>(end - start).count();
}

void Time::Wait(float seconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds*1000)));
}