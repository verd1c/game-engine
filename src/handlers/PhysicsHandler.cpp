#include "engine/Game.h"
#include "engine/Sprite.h"

void Game::PhysicsHandler(void) {


	
	/*
	* Progress Gravity
	*/
	for (Sprite *s : SpriteManager::GetSingleton().GetDisplayList()) {
		GravityHandler &gh = s->GetGravityHandler();
		if (!gh.IsGravityAddicted())
			continue;
		Sprite* mario = SpriteManager::GetSingleton().GetTypeList("main").front();
		bool bSolidGround = this->mMap->GetTileLayer()->GetGridLayer()->IsOnSolidGround(s->GetBox());

		if (bSolidGround) {
			gh.SetFalling(false);
		}

		if (!gh.IsJumping() && !bSolidGround && !gh.IsFalling()) {
			gh.SetFalling(true);
			gh.SetJumping(false);
			gh.SetJumpSpeed(1);
		}

		if ((gh.IsJumping() && bSolidGround) || (gh.IsJumping() && gh.IsFalling())) {
			gh.SetJumpSpeed(-gh.GetBaseSpeed());
		}

		if (gh.IsJumping()) {
			gh.SetFalling(false);
			gh.SetJumpSpeed(gh.GetJumpSpeed() + gh.GetGravity());

			if (gh.GetJumpSpeed() > 0) {
				gh.SetJumping(false);
				gh.SetJumpSpeed(1);
				gh.SetFalling(true);
			}
			gh.SetYVelocity(gh.GetJumpSpeed());
		}
		
		if (gh.IsFalling()) {
			gh.SetJumpSpeed(gh.GetJumpSpeed() + gh.GetGravity());
			if (gh.GetYVelocity() > -gh.GetBaseSpeed()) {
				gh.SetYVelocity(gh.GetBaseSpeed());
			}
			gh.SetYVelocity(gh.GetJumpSpeed());
		}



		s->Move(0, gh.GetYVelocity());

	}

}