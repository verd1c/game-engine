#ifndef __SPRITE_H__
#define __SPRITE_H__

#include<string>
#include<list>
#include<map>
#include<functional>

#include <engine/Structs.h>
#include <engine/Map.h>
#include <engine/Physics.h>
#include "engine/animations/Film.h"
#include "engine/Game.h"

//class Clipper;
//class MotionQuantizer;
//class BoundingArea;

class Sprite {
public:
	using Mover = std::function<void(const Rect&, int* dx, int* dy)>;
private:

	

	Rect frameBox; // inside the film
	bool isVisible = false;

	//BoundingArea* boundingArea = nullptr;
	unsigned zorder = 0;
	std::string typeId, stateId;
	Mover mover;

	Bitmap* bitmap;


	/*
	* Physics
	*/
	GravityHandler hGravityHandler;
	MotionQuantizer qMotionQuantizer;

	/*
	* Movement
	*/
	std::map<std::string, std::function<void(Sprite*)>> vActions;
	std::map<std::string, std::function<void(Sprite*)>> vDefaultActions;


public:

	/*
	* Meta Data
	*/
	std::string id;
	bool bKillable = false;
	bool bDead = false;
	int health;


	int frameNo = 0;
	int dx = 2;
	bool bLooking = true;
	bool bAttacking = false;

	Film* currFilm = nullptr;

	std::string GetTrimmedID(void);

	/*
	* Movement
	*/
	void RegisterAction(std::string id, std::function<void(Sprite*)> fAction);
	void RegisterDefaultAction(std::string id, std::function<void(Sprite*)> fAction);
	void RegisterDefaultActions(void);
	void CallAction(std::string id);
	void CallDefaultAction(std::string id);


	/*
	* Physics
	*/
	GravityHandler& GetGravityHandler(void);
	MotionQuantizer& GetMotionQuantizer(void);

	void SetMover(MotionQuantizer::Mover f)
	{
		this->qMotionQuantizer.SetMover(f);
		//quantizer.SetMover(mover = f);
	}

	Rect GetBox(void)
	{
		if (currFilm) {
			frameBox.w = currFilm->GetFrameBox(frameNo).w;
			frameBox.h = currFilm->GetFrameBox(frameNo).h;
		}

		return { x, y, frameBox.w, frameBox.h };
	}
	
	void Move(int dx, int dy);
	
	Bitmap * getBitamp() { return this->bitmap; }

	void SetPos(int _x, int _y) { x = _x; y = _y; }
	void SetZorder(unsigned z) { zorder = z; }
	unsigned GetZorder(void) { return zorder; }
	
	void SetFrame(byte i) {
		this->frameNo = i;
	}
	
	byte GetFrame(void) const { return frameNo; }

	auto GetTypeId(void) -> const std::string& { return typeId; }
	void SetVisibility(bool v) { isVisible = v; }
	bool IsVisible(void) const { return isVisible; }
	bool CollisionCheck( Sprite* s);
	void Display(Bitmap* dest, const Rect& dpyArea) const;


	Sprite(std::string id, int _x, int _y, Bitmap * bitmap,int width,int height, bool bKillable) :
		id(id), x(_x), y(_y), bitmap(bitmap), bKillable(bKillable)
	{
		this->frameBox = Rect(_x, _y, width, height);
		//frameNo = currFilm->GetTotalFrames(); SetFrame(0);
	}

	Mover MakeSpriteGridLayerMover(GridLayer* glLayer);

	int x = 0, y = 0;

};


class SpriteManager final {
public:
	using SpriteList = std::list<Sprite*>;
	using TypeLists = std::map<std::string, SpriteList>;
private:
	std::list<Sprite*> dpyList;
	std::map<std::string, std::list<Sprite*> > types;
	static  SpriteManager singleton;

public:
	Sprite* SpawnSprite(json jObject, std::string sName, std::string id, int x, int y, GridLayer *glLayer, Game *g);

	void Add(Sprite* s); // insert by ascending zorder
	void Remove(Sprite* s);
	void AddToTypeList(std::string id, Sprite *); // insert by ascending zorder
	void RemoveTypeList(std::string id, Sprite* sprite);
	SpriteList& GetDisplayList(void)
	{
		return dpyList;
	}
	SpriteList& GetTypeList(const std::string& typeId)
	{
		return types[typeId];
	}
	static auto GetSingleton(void) -> SpriteManager&
	{
		return singleton;
	}
	static auto GetSingletonConst(void) -> const SpriteManager&
	{
		return singleton;
	}

	Sprite* GetSprite(std::string id) {
		for (Sprite *s : dpyList) {
			if (s->id == id)
				return s;
		}
	}
};
//template <typename Tnum>
//int number_sign(Tnum x) {
//	return x > 0 ? 1 : x < 0 ? -1 : 0;
//}

class Clipper {
public:
	using View = std::function<const Rect& (void)>;
private:
	View view;
public:
	Clipper& SetView(const View& f)
	{
		view = f; return *this;
	}
	bool Clip(
		const Rect& r,
		const Rect& dpyArea,
		Point* dpyPos,
		Rect* clippedBox
	) const;
	Clipper(void) = default;
	Clipper(const Clipper&) = default;
};


class CollisionChecker final {
public:
	using Action = std::function<void(Sprite* s1, Sprite* s2)>;
	static CollisionChecker singleton;
protected:
	using Entry = std::tuple<Sprite*, Sprite*, Action>;
	std::vector< std::tuple<Sprite*, Sprite*, Action> > entries;
	/*	
		returns iterator to tuples list
		finds if there is a tupple between s1 and s2 
	*/
	auto Find(Sprite* s1, Sprite* s2)->std::vector< std::tuple<Sprite*, Sprite*, Action>>::iterator;
public:
	void Register(Sprite* s1, Sprite* s2, const Action& f)
	{
		//assert(!In(s1, s2)); 
		entries.push_back(std::make_tuple(s1, s2, f));
	}
	void Cancel(Sprite* s1, Sprite* s2);
	void Check(void) const;
	static auto GetSingleton(void) -> CollisionChecker&
	{
		return singleton;
	}
	static auto GetSingletonConst(void) -> const CollisionChecker&
	{
		return singleton;
	}
};


#endif