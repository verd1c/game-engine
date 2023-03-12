#include "engine/Game.h"
#include "engine/Sprite.h"


void Game::CollisionHandler() {
	/*for (Sprite * s : SpriteManager::GetSingleton().GetDisplayList()) {
		CollisionChecker::GetSingleton().Check();
	}*/
	CollisionChecker::GetSingleton().Check();






}