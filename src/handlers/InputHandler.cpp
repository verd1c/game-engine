#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "display/DisplayTools.h"
#include "engine/Game.h"

#include "engine/Animations/AnimatorManager.h"
#include "game/audio.h"
#include "engine/Animations/SystemClock.h"
void Game::InputHandler(void) {
	ALLEGRO_EVENT events;
	al_wait_for_event(event_queue, &events);
	al_get_keyboard_state(&ksKeyboardState);
	Sprite* sMario = nullptr;
	for (auto obj : SpriteManager::GetSingleton().GetTypeList("main")) {
		sMario = obj;

	}

	if (events.type == ALLEGRO_EVENT_TIMER) {

		if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_ESCAPE)) {
			this->SetRunning(false);
		}

		bool bDidSomething = false;

		if (bIsGameEnding) {

			sMario->CallAction("run.right");

			if (sMario->x == 296 * 16 && sMario->y == 88 * 16) {	//flag
				bIsGameEnding = false;
				sMario->bLooking = false;
				sMario->CallAction("idle");
			}
			else if (sMario->x == 296 * 16 && sMario->y == 94 * 16) { //finishsign
				bIsGameEnding = false;
				sMario->bLooking = false;
				sMario->CallAction("idle");
			}

		}
		else if (bInputAllowed) {

			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_Z)) {
				sMario->dx = 3;
				sMario->GetGravityHandler().lBaseJumpSpeed = gGameSettings.lJumpSpeed * 1.15;
				sMario->GetGravityHandler().lGravity = gGameSettings.fGravity * 0.90;
				bDidSomething = true;



			}
			else {
				sMario->GetGravityHandler().lBaseJumpSpeed = gGameSettings.lJumpSpeed;
				sMario->GetGravityHandler().lGravity = gGameSettings.fGravity;
				sMario->dx = 2;
				bDidSomething = false;
			}



			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_SPACE)) {
				sMario->CallAction("jump");
				bDidSomething = true;
			}
			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_RIGHT)) {
				sMario->CallAction("run.right");
				bDidSomething = true;
			}
			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_LEFT)) {
				sMario->CallAction("run.left");
				bDidSomething = true;
			}
			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_DOWN)) {
				sMario->Move(0, 2);
				bDidSomething = true;
			}
			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_UP)) {
				//sMario->SetFrame((sMario->GetFrame() + 1 ) % sMario->currFilm->GetTotalFrames());
				bDidSomething = true;
			}

			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_X)) {
				if (sMario->id == "herochar") {

					//Audio::singleton.playSample("config/sounds/sword-hit.mp3", ALLEGRO_PLAYMODE_ONCE);
					sMario->CallAction("attack.sword");
					bDidSomething = true;

				}
			}


			if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_C)) {
				if (sMario->id == "herochar") {

					//Audio::singleton.playSample("config/sounds/sword-hit.mp3", ALLEGRO_PLAYMODE_ONCE);
					//sMario->CallAction("attack.sword");
					Sprite* boomerang = SpriteManager::GetSingleton().SpawnSprite(Config::GetConfig("config/sprites/boomerang.json"), "boomerang", "boomerang", sMario->x + 4, sMario->y, this->mMap->GetTileLayer()->GetGridLayer(), this);

					boomerang->bDead = false;

					//boomerang->GetGravityHandler().Disable();
					boomerang->RegisterDefaultActions();


					for (Sprite* obj : SpriteManager::GetSingleton().GetDisplayList()) {
						if (obj->id.find("boomerang") != std::string::npos) {
							continue;
						}
						if (obj == boomerang || obj == sMario) continue;
						CollisionChecker::GetSingleton().Register(obj, boomerang,
							[](Sprite* s1, Sprite* s2) {

								s2->bDead = true;
								SpriteManager::GetSingleton().Remove(s2);
								CollisionChecker::GetSingleton().Cancel(s1, s2);



								if (s1->id.find("turtle") != std::string::npos || s1->id.find("goomba") != std::string::npos || s1->id.find("slime") != std::string::npos) {
									std::cout << "collision damaged id: " << s1->id << std::endl;
									s1->CallAction("damage");
								}






							}
						);
					}

					/*Animator* pAnim, * pAnimBoomer = NULL;


					pAnimBoomer = AnimatorManager::GetSingleton().GetAnimatorByAnimationID("boomerang.run.right");


					if (pAnimBoomer && pAnimBoomer->HasFinished()) {

						Sprite * boomerang=SpriteManager::GetSingleton().SpawnSprite(Config::GetConfig("config/sprites/boomerang.json"), "boomerang", "boomerang", sMario->x + 4, sMario->y, this->mMap->GetTileLayer()->GetGridLayer(), this);

						//sMario->currFilm = FilmHolder::Get().GetFilm("mario.walking.right");
						if (sMario->bLooking)
							boomerang->currFilm = FilmHolder::Get().GetFilm("boomerang.run.right");
						//else
							//s->currFilm = FilmHolder::Get().GetFilm(s->GetTrimmedID() + ".attack.sword.left");
						((FrameListAnimator*)pAnimBoomer)->Start(((FrameListAnimator*)pAnimBoomer)->getAnimation(), SystemClock::Get().getgametime());
						AnimatorManager::GetSingleton().MarkAsRunning(pAnimBoomer);
					}
					*/


					bDidSomething = true;

				}
			}
			if (!bDidSomething) {
				sMario->CallAction("idle");
			}
		}
	}

	return;
}