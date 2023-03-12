#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__
#include "Animation.h"
#include "engine/Sprite.h"

typedef uint64_t timestamp_t;

enum animatorstate_t {
	ANIMATOR_FINISHED = 0,
	ANIMATOR_RUNNING = 1,
	ANIMATOR_STOPPED = 2
};


class Animator {
public:
	//function called on animation finish
	using OnFinish = std::function<void(Animator*)>;
	//function called on animation start
	using OnStart = std::function<void(Animator*)>;
	//function call on animation action
	using OnAction = std::function<void(Animator*, const Animation&)>;

protected:

	timestamp_t		lastTime = 0;//h teleutaia xronikh stigmh poy exei efarmostei ena bhma toy animation, teleutaia energeia toy animation

	animatorstate_t state = ANIMATOR_FINISHED;
	OnFinish		onFinish;
	OnStart			onStart;
	std::function<void(Animator*, const Animation&)> onAction;
	void			NotifyStopped(void);
	void			NotifyStarted(void);
	void			NotifyAction(void);
	void			Finish(bool isForced = false);

public:
	void			Stop(void);
	bool			HasFinished(void) { return state != ANIMATOR_RUNNING; }

	void setState(animatorstate_t state) { this->state = state; }

	virtual void	TimeShift(timestamp_t offset);//



	virtual void	Progress(timestamp_t currTime) = 0;
	void NotifyAction(const Animation& anim);

	//call set functions to set finish,start,action functions
	void SetOnFinish(std::function<void(Animator*)> f) { onFinish = f; }
	template <typename Tfunc> void SetOnStart(const Tfunc& f) { onStart = f; }
	void SetOnAction(std::function<void(Animator*, const Animation&)> f) { onAction = f; }
	Animator(void) {  }
	Animator(const Animator&) = delete;
	Animator(Animator&&) = delete;
};



class MovingAnimator : public Animator {
protected:
	MovingAnimation* anim = nullptr;
	unsigned		 currRep = 0;

public:
	void			 Progress(timestamp_t currTime);
	MovingAnimation& GetAnim(void)  { return *anim; }
	void			 Start(MovingAnimation* a, timestamp_t t)
	{
		anim = a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		currRep = 0;
		NotifyStarted();
	}

	MovingAnimator(void) = default;
};







/*
class FrameRangeAnimator : public Animator {
protected:
	FrameRangeAnimation* anim = nullptr;
	unsigned				currFrame = 0; // animation state
	unsigned				currRep = 0; // animation state
public:
	void					Progress(timestamp_t currTime);
	unsigned				GetCurrFrame(void) const { return currFrame; }
	unsigned				GetCurrRep(void) const { return currRep; }
	void					Start(FrameRangeAnimation* a, timestamp_t t) {
		anim = a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		currFrame = anim->GetStartFrame();
		currRep = 0;
		NotifyStarted();
		NotifyAction(*anim);
	}
	FrameRangeAnimator(void) = default;






};
*/



class FrameListAnimator : public Animator {
protected:
	FrameListAnimation* anim = nullptr;
	unsigned				IndexInFramesVector = 0; // animation state
	unsigned				curr_rep = 0; // animation state


public:
	void					Progress(timestamp_t currTime);
	FrameListAnimation*		getAnimation() { return anim; }

	void					setAnimation(FrameListAnimation* anim) { this->anim = anim; }
	unsigned				GetCurrFrame(void) const { return IndexInFramesVector; }
	unsigned				GetCurrRep(void) const { return curr_rep; }

	void					Start(FrameListAnimation* a, timestamp_t t) {
		anim = a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		IndexInFramesVector = 0;
		NotifyStarted();
		curr_rep = 0;
		NotifyAction(*anim);
	}
	FrameListAnimator(void) = default;

	//void FramList_Action(Sprite* sprite, Animator* animator, const FrameListAnimation& anim);




};







/*
void FrameRange_Action(Sprite* sprite, Animator* animator, const FrameRangeAnimation& anim) {
		auto* frameRangeAnimator = (FrameRangeAnimator*)animator;
		if (frameRangeAnimator->GetCurrFrame() != anim.GetStartFrame() ||
			frameRangeAnimator->GetCurrRep())
			sprite->Move(anim.GetDx(), anim.GetDy());
		sprite->SetFrame(frameRangeAnimator->GetCurrFrame());
	}
*/




/*
animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		FrameRange_Action(sprite, animator, (const FrameRangeAnimation&)anim);
	}
);
*/


#endif
