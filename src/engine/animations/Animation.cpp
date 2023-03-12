#include "engine/animations/Animation.h"

int MovingAnimation::GetDx(void) { return dx; }
MovingAnimation::Me& MovingAnimation::SetDx(int v) { dx = v; return *this; }
int MovingAnimation::GetDy(void) { return dy; }
MovingAnimation::Me& MovingAnimation::SetDy(int v) { dy = v; return *this; }
unsigned MovingAnimation::GetDelay(void) { return delay; }
MovingAnimation::Me& MovingAnimation::SetDelay(unsigned v) { delay = v; return *this; }
unsigned MovingAnimation::GetReps(void) { return reps; }
MovingAnimation::Me& MovingAnimation::SetReps(unsigned n) { reps = n; return *this; }
bool MovingAnimation::IsForever(void) { return !reps; }
MovingAnimation::Me& MovingAnimation::SetForever(void) { reps = 0; return *this; }
MovingAnimation::MovingAnimation(
	const std::string& _id, unsigned _reps, int _dx, int _dy, unsigned _delay
) : Animation(_id),
reps(_reps),
dx(_dx),
dy(_dy),
delay(_delay) {
}