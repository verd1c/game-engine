#ifndef __PHYSICS_H__
#define __PHYSICS_H_
#include <functional>
#include "engine/Structs.h"

class Sprite;
class GravityHandler {
private:
	bool bGravityAddicted = false;
	bool bIsFalling = false;
	bool bIsJumping = false;

	std::function<bool(Rect)> fOnSolidGroundCallback;
	std::function<void(void)> fOnStartFallingCallback;
	std::function<void(void)> fOnStopFallingCallback;

public:
	float lGravity = 1;
	float lJumpSpeed = -12;
	int lBaseJumpSpeed = 12;
	int lYVelocity = 0;

	void SetOnSolidGroud(std::function<bool(Rect)> f);
	void SetOnStartFalling(std::function<void(void)> f);
	void SetOnStopFalling(std::function<void(void)> f);

	void Jump(void);

	//static void ProgressPhysics(Sprite *s);
	bool IsJumping(void);
	bool IsFalling(void);
	float GetGravity(void);
	float GetJumpSpeed(void);
	int GetYVelocity(void);
	int GetBaseSpeed(void);

	void SetJumpSpeed(float);
	void SetYVelocity(int);
	void SetFalling(bool);
	void SetJumping(bool);

	bool IsGravityAddicted(void);

	void Enable(void);
	void Disable(void);
	void Reset(void);
	void Check(Rect r);

	GravityHandler();
};

class MotionQuantizer {
public:
	using Mover = std::function<void(Rect r, int *dx, int *dy)>;

private:
	int lMaxHorizontal = 4, lMaxVertical = 4;
	Mover mover;
	bool bUsed = false;

public:
	MotionQuantizer& SetUsed(bool b);
	MotionQuantizer& SetRange(int h, int v);

	MotionQuantizer& SetMover(Mover& f);

	void Move(Rect r, int *dx, int *dy);
};

//class MotionQuantizer {
//public:
//	using Mover = std::function<void(const Rect& r, int* dx, int* dy)>;
//protected:
//	int horizMax = 0, vertMax = 0;
//	Mover mover; // filters requested motion too!
//	bool used = false;
//public:
//	MotionQuantizer& SetUsed(bool val);
//	MotionQuantizer& SetRange(int h, int v)
//	{
//		horizMax = h, vertMax = v; used = true; return *this;
//	}
//	MotionQuantizer& SetMover(const Mover& f)
//	{
//		mover = f; return *this;
//	}
//	void Move(const Rect& r, int* dx, int* dy);
//	MotionQuantizer(void) = default;
//	MotionQuantizer(const MotionQuantizer&) = default;
//};
#endif