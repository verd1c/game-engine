#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <map>
#include "tools/json.h"

#define MAX_WIDTH	1024
#define MAX_HEIGHT	256

typedef unsigned char		byte;
typedef ALLEGRO_BITMAP		Bitmap;
typedef ALLEGRO_DISPLAY		Display;
typedef unsigned char		Index;
typedef Index				TileMap[MAX_WIDTH][MAX_HEIGHT];

class Rect {
public:
	int x, y, w, h;

	Rect(int, int, int, int);
	Rect() : x(0), y(0), w(0), h(0) {

	}
};

class Point {
public:
	int x, y;

	Point(int xx, int yy);
	Point() : x(0), y(0) {

	}
};

typedef struct Settings {

	INT lWindowWidth, lWindowHeight;
	SHORT lFpsLimit;
	SHORT lJumpSpeed;
	FLOAT fGravity;
} Settings;

extern Settings gGameSettings;

#endif