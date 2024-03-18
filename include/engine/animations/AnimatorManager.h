#ifndef __ANIMATORMANAGER_H__

#define __ANIMATORMANAGER_H__

#include "Animator.h"
#include <set>

class AnimatorManager {
private:
	std::set<Animator*> running, suspended;

	static AnimatorManager singleton;
	AnimatorManager(void) = default;
	AnimatorManager(const AnimatorManager&) = delete;
	AnimatorManager(AnimatorManager&&) = delete;
public:
	Animator* getFirstRunning() {
		return (*running.begin());
	}
	void Register(Animator* a)
	{
		suspended.insert(a);
	}
	void Cancel(Animator* a)
	{
		suspended.erase(a);
	}
	void MarkAsRunning(Animator* a)
	{
		suspended.erase(a); running.insert(a);
	}
	void MarkAsSuspended(Animator* a)
	{
		running.erase(a); suspended.insert(a);
	}

	Animator* GetAnimatorByAnimationID(std::string id) {
		for (Animator *anim : running) {
			if (dynamic_cast<FrameListAnimator*>(anim) != nullptr) {
				FrameListAnimation* mv = ((FrameListAnimator*)anim)->getAnimation();
				if (mv->GetId() == id) {
					return anim;
				}
				
			}
		}
		for (Animator* anim : suspended) {
			if (dynamic_cast<FrameListAnimator*>(anim) != nullptr) {
				Animation* mv = ((FrameListAnimator*)anim)->getAnimation();
				//std::cout << mv->id << std::endl;
				if (mv->id == id) {
					return anim;
				}

			}
		}
	}

	void Progress(timestamp_t currTime) {

		auto copied(running);
		for (auto* a : copied)
		{
			a->Progress(currTime);

		}
	}

	static auto GetSingleton(void) -> AnimatorManager& { return singleton; }
	static auto GetSingletonConst(void) -> const AnimatorManager& { return singleton; }
};


#endif