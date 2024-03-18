#ifndef __FILM_H__
#define __FILM_H__

#include "engine/Structs.h"


class Film {
	
	std::string id;

	std::vector<Rect> vBoxes;
	Bitmap* bBitmap;

public:

	int GetTotalFrames(void);
	Bitmap* GetBitmap(void);
	std::string GetId(void);
	Rect GetFrameBox(int fBox);

	void Append(Rect rBox);
	void SetBitmap(Bitmap *bBitmap);
	void DisplayFrame(Bitmap* bDest, Point at, int frameNo);


	Film(std::string id);
	Film(Bitmap *bBitmap, std::vector<Rect> vBoxes, std::string id);
};

class FilmHolder {
public:
	using Films = std::map<std::string, Film*>;

	Films films;

	static FilmHolder holder;

	FilmHolder(void) {}

	void Load(std::string id, json jConfig, Bitmap* bm);

	static auto Get(void) -> FilmHolder& { return holder; }
	Film* GetFilm(std::string id);




};


#endif