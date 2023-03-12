#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "engine/Structs.h"
#include "engine/Sprite.h"

class Animation {
public:
	std::string id;
	const std::string& GetId(void) { return id; }
	std::string GetTrimmedID(void) {
		std::string idd(GetId());
		if (idd.find("_") == std::string::npos)
			return idd;
		std::string trimmed_id = idd.substr(0, idd.find("_"));
		idd.erase(0, idd.find("."));
		trimmed_id += idd;
		return trimmed_id;
	}
	void SetId(std::string& _id);
	virtual Animation* Clone(void) const = 0;
	Animation(const std::string& _id) : id(_id) {}
	virtual ~Animation() {}
};


class MovingAnimation : public Animation {
protected:
	unsigned reps = 1; // 0=forever
	int dx = 0, dy = 0;
	unsigned delay = 0;

public:
	using Me = MovingAnimation;
	int GetDx(void);
	Me& SetDx(int v);
	int GetDy(void);
	Me& SetDy(int v);
	unsigned GetDelay(void);
	Me& SetDelay(unsigned v);
	unsigned GetReps(void);
	Me& SetReps(unsigned n);
	bool IsForever(void);
	Me& SetForever(void);
	Animation* Clone(void) const override {
		return new MovingAnimation(id, reps, dx, dy, delay);
	}
	MovingAnimation(
		const std::string& _id, unsigned _reps, int _dx, int _dy, unsigned _delay
	);
	//this method moves the sprite

	void Sprite_MoveAction(Sprite* sprite, MovingAnimation& anim) {
		sprite->Move(anim.GetDx(), anim.GetDy());
	}

};

/*
animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		assert(dynamic_cast<const MovingAnimation*>(&anim));
		Sprite_MoveAction(
			sprite,
			(const MovingAnimation&)anim
		);
	}
);
*/

class FrameRangeAnimation : public MovingAnimation {
protected:
	unsigned start = 0, end = 0;
public:
	using Me = FrameRangeAnimation;
	unsigned GetStartFrame(void) const { return start; }
	Me& SetStartFrame(unsigned v) { start = v; return *this; }
	unsigned GetEndFrame(void) const { return end; }
	Me& SetEndFrame(unsigned v) { end = v; return *this; }
	Animation* Clone(void) const override {
		/*return new FrameRangeAnimation(
			id, start, end, GetReps(), GetDx(), GetDy(), GetDelay()
		);*/
		return nullptr;
	}
	FrameRangeAnimation(
		const std::string& _id,
		unsigned s, unsigned e,
		unsigned _reps, int dx, int dy, int _delay
	) : start(s), end(e), MovingAnimation(id, _reps, dx, dy, _delay) {}



};


class FrameListAnimation : public MovingAnimation {
public:
	using Frames = std::vector<unsigned>;

protected:
	Frames frames;
public:
	const Frames& GetFrames(void) const
	{
		return frames;
	}
	void SetFrames(const Frames& f)
	{
		frames = f;
	}
	Animation* Clone(void) const override {
		/*return new FrameListAnimation(
			id, frames, GetReps(), GetDx(), GetDy(), GetDelay()
		);*/
		return nullptr;
	}
	FrameListAnimation(
		const std::string& _id,
		const Frames& _frames,
		unsigned _reps, int dx, int dy, unsigned _delay
	) : frames(_frames), MovingAnimation(_id, _reps, dx, dy, _delay) {}



};


#endif