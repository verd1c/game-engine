#include "engine/Game.h"
#include "engine/Animations/AnimatorManager.h"
#include "engine/Animations/SystemClock.h"

void Game::AnimationHandler(void) {
	AnimatorManager::GetSingleton().Progress(SystemClock::Get().getgametime());
}