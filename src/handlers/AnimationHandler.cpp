#include "engine/Game.h"
#include "engine/animations/AnimatorManager.h"
#include "engine/animations/SystemClock.h"

void Game::AnimationHandler(void) {
	AnimatorManager::GetSingleton().Progress(SystemClock::Get().getgametime());
}