#include "engine/Physics.h"
#include <iostream>
#include <math.h>
#include <algorithm>

template <typename Tnum>
int number_sign(Tnum x) {
	if (x > 0)
		return 1;
	if (x < 0)
		return -1;
	return 0;
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

MotionQuantizer& MotionQuantizer::SetUsed(bool b)
{
	this->bUsed = b;
	return *this;
}

MotionQuantizer& MotionQuantizer::SetRange(int h, int v)
{
	this->lMaxHorizontal = h;
	this->lMaxVertical = v;
	return *this;
}

MotionQuantizer& MotionQuantizer::SetMover(Mover& f)
{
	this->mover = f;
	return *this;
}

void MotionQuantizer::Move(Rect r, int* dx, int* dy) {
	if (false) {
		mover(r, dx, dy);
	}
	else {
		do {
			auto sign_x = number_sign(*dx);
			auto sign_y = number_sign(*dy);

			auto dxFinal = sign_x * min(this->lMaxHorizontal, sign_x * (*dx));
			auto dyFinal = sign_y * min(this->lMaxVertical, sign_y * (*dy));
			
			mover(r, &dxFinal, &dyFinal);
			r.x += dxFinal;
			r.y += dyFinal;

			if (!dxFinal)
				*dx = 0;
			else
				*dx -= dxFinal;
			if (!dyFinal)
				*dy = 0;
			else
				*dy -= dyFinal;
		} while (*dx || *dy);
	}
}