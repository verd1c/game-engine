#include"engine/animations/SystemClock.h"


uint64_t SystemClock::milli_secs(void) const {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(clock.now().time_since_epoch()).count();
}
uint64_t SystemClock::micro_secs(void) const {
	return std::chrono::duration_cast<std::chrono::microseconds>
		(clock.now().time_since_epoch()).count();
}
uint64_t SystemClock::nano_secs(void) const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>
		(clock.now().time_since_epoch()).count();
}


void SystemClock::setgametime()
{
	this->currTime = SystemClock::Get().milli_secs();
}

unsigned long  SystemClock::getgametime()
{
	return currTime;
}