#include"Time.h"
#include<thread>

TimeNamespace::TimePoint TimeNamespace::GetTimePoint() {
	return std::chrono::steady_clock::now();
}

float TimeNamespace::GetTime() {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - ProgramStartTime).count();
}

float TimeNamespace::GetDuration(const TimePoint& start, const TimePoint& end) {
	return std::chrono::duration<float>(end - start).count();
}

void TimeNamespace::Wait(float seconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds*1000)));
}