#pragma once 

#include "engine/animations/Animator.h"
#include "engine/animations/AnimatorManager.h"




void Animator::NotifyStarted(void) {

	AnimatorManager::GetSingleton().MarkAsRunning(this);
	//if (onStart)
	//	onStart(this);
}

void Animator::NotifyAction(void) {

}


void Animator::Finish(bool isForced) {
	if (!HasFinished()) {
		state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
		NotifyStopped();
	}
}

void Animator::Stop(void) {
	Finish(true);
}

void Animator::NotifyStopped(void) {
	AnimatorManager::GetSingleton().MarkAsSuspended(this);
	if (onFinish)
		(onFinish)(this);
}


void Animator::NotifyAction(const Animation& anim) {
	if (onAction)
		(onAction)(this, anim);
}

void Animator::TimeShift(timestamp_t offset) {
	lastTime += offset;
}



void FrameListAnimator::Progress(timestamp_t currTime) {

	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {

		if (IndexInFramesVector >= anim->GetFrames().size() - 1) {
			//assert(anim->IsForever() || currRep < anim->GetReps());
			IndexInFramesVector = 0; // flip to start
		}
		else
			++IndexInFramesVector;
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (IndexInFramesVector == anim->GetFrames().size() - 1) {
			if (!anim->IsForever() && ++curr_rep == anim->GetReps()) {
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}

	}
}