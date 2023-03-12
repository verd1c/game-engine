#include "engine/animations/Film.h"
#include "display/DisplayTools.h"

int Film::GetTotalFrames(void)
{
	return this->vBoxes.size();
}

Bitmap* Film::GetBitmap(void)
{
	return this->bBitmap;
}

std::string Film::GetId(void)
{
	return id;
}

Rect Film::GetFrameBox(int fBox)
{
	return this->vBoxes[fBox];
}

void Film::Append(Rect rBox)
{
	this->vBoxes.push_back(rBox);
}

void Film::SetBitmap(Bitmap* bBitmap)
{
	this->bBitmap = bBitmap;
}

void Film::DisplayFrame(Bitmap* bDest, Point at, int frameNo)
{
	Rect r = GetFrameBox(frameNo);
	Blit(bDest, (Rect&)Rect(at.x, at.y, r.w, r.h), this->bBitmap, r);
}

Film::Film(std::string id) :
	id(id),
	bBitmap(nullptr)
{
}

Film::Film(Bitmap* bBitmap, std::vector<Rect> vBoxes, std::string id) :
	bBitmap(bBitmap),
	vBoxes(vBoxes),
	id(id)
{

}

void FilmHolder::Load(std::string id, json jConfig, Bitmap *bm)
{
	std::vector<Rect> vBoxes;
	for (json &e : jConfig) {
		vBoxes.push_back(Rect(e["x_pos"], e["y_pos"], e["width"], e["height"]));
	}
	Film* fNewFilm = new Film(bm, vBoxes, id);
	this->films.insert({id, fNewFilm});
	return;
}

Film* FilmHolder::GetFilm(std::string id)
{
	auto e = films.find(id);
	return e != films.end() ? e->second : nullptr;
}
