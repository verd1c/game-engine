#include "engine/Structs.h"

Settings gGameSettings;

Rect::Rect(int xx, int yy, int ww, int hh) :
	x(xx),
	y(yy),
	w(ww),
	h(hh)
{}


Point::Point(int xx, int yy) :
	x(xx),
	y(yy)
{}