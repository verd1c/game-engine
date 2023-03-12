#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "display/DisplayTools.h"
#include <game/App.h>
#include "engine/Sprite.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "engine/Animations/Animation.h"
#include "engine/Animations/Animator.h"
#include "engine/Animations/AnimatorManager.h"
#include "engine/Animations/SystemClock.h"


void SuperMario::Initialise(void) {
	json jGameConfig = Config::GetConfig("config/game.json");

	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_DIRECT3D_INTERNAL);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_DIRECT3D_INTERNAL);

	/*
	* Load Game Settings
	*/
	gGameSettings.lWindowWidth = jGameConfig["resolution"]["width"];
	gGameSettings.lWindowHeight = jGameConfig["resolution"]["height"];
	gGameSettings.lFpsLimit = jGameConfig["fps_limit"];
	gGameSettings.fGravity = jGameConfig["physics"]["gravity"];
	gGameSettings.lJumpSpeed = jGameConfig["physics"]["jump_speed"];

	ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	//game.mixer= al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);

	//al_set_default_mixer(game.mixer);


	//al_attach_mixer_to_voice(game.mixer, voice);


	al_reserve_samples(1);



	//Audio::singleton.playSample("config/sounds/main.mp3", ALLEGRO_PLAYMODE_ONCE);

	Audio::singleton.Voice = al_create_voice(
		44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2
	);
	Audio::singleton.Mixer = al_create_mixer(
		44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2
	);


	al_set_default_mixer(Audio::singleton.Mixer);
	al_attach_mixer_to_voice(Audio::singleton.Mixer, Audio::singleton.Voice);


	/*
	this->audio_sample = new Audio;


	audio_sample->playSample("config/sound.mp3");*/

	//al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP | ALLEGRO_MIPMAP);
	display = al_create_display(gGameSettings.lWindowWidth, gGameSettings.lWindowHeight);
	if (!display) {
		printf("Could not create display, exiting\n");
		exit(0);
	}



	this->game.timer = al_create_timer(1.0 / gGameSettings.lFpsLimit);
	this->game.event_queue = al_create_event_queue();
	al_register_event_source(this->game.event_queue, al_get_keyboard_event_source());
	al_register_event_source(this->game.event_queue, al_get_timer_event_source(this->game.timer));
	al_start_timer(this->game.timer);

	/*
	Audio* audio_sample = new Audio;
	audio_sample->playSample("config/sounds/main.mp3");*/

}

CollisionChecker CollisionChecker::singleton;
SpriteManager SpriteManager::singleton;
Audio Audio::singleton;
FilmHolder FilmHolder::holder;

AnimatorManager AnimatorManager::singleton;

Sprite* addItemToTypeList(std::string id, std::string spriteName, int x, int y, int width, int height, Bitmap* pngBitmap, int i, int j) {
	Bitmap* bm = al_create_sub_bitmap(pngBitmap, x, y, width, height);
	Sprite* s = new Sprite(spriteName, i, j, bm, width, height, true);

	SpriteManager::GetSingleton().Add(s);
	SpriteManager::GetSingleton().AddToTypeList(id, s);

	return s;
}

/*
	initialize which characters collides with whom
	and register action after collision
*/


void registerCollisionsActions(GridLayer* glLayer, Game* g) {
	Sprite* mario = SpriteManager::GetSingleton().GetTypeList("main").front();

	for (Sprite* ubaluba : SpriteManager::GetSingleton().GetTypeList("goomba"))
	{
		CollisionChecker::GetSingleton().Register(mario, ubaluba,
			[](Sprite* s1, Sprite* s2) {
				if (s1->GetBox().y < s2->GetBox().y) {
					//SpriteManager::GetSingleton().Remove(s2);
					s2->CallAction("death");
					s1->GetGravityHandler().Jump();


					CollisionChecker::GetSingleton().Cancel(s1, s2);
				}
				else {
					if (s1->bAttacking) {
						//SpriteManager::GetSingleton().Remove(s2);
						s2->CallAction("death");
						CollisionChecker::GetSingleton().Cancel(s1, s2);
						printf("character sliced goomba");
					}
					else {
						printf("mario died from uba luba\n");
						s1->CallAction("damage");
					}

				}
			}
		);
	}


	for (Sprite* enemy_bird : SpriteManager::GetSingleton().GetTypeList("sl"))
	{

		CollisionChecker::GetSingleton().Register(mario, enemy_bird,
			[](Sprite* s1, Sprite* s2) {
				//action if mario collides with uba luba
				//printf("mario collided with birddddddd\n");


				printf("===================================\n");
				if (s1->GetBox().y < s2->GetBox().y) {

					printf("mario smashed bird\n");

					SpriteManager::GetSingleton().RemoveTypeList("enemy_bird", s2);
					SpriteManager::GetSingleton().Remove(s2);
					CollisionChecker::GetSingleton().Cancel(s1, s2);
					s1->GetGravityHandler().Jump();

				}
				else {
					printf("mario died from bird \n");
					s1->CallAction("damage");
				}
				printf("===================================\n");


			}
		);




	}

	for (Sprite* enemy_turtle : SpriteManager::GetSingleton().GetTypeList("slime"))
	{

		CollisionChecker::GetSingleton().Register(mario, enemy_turtle,
			[](Sprite* s1, Sprite* s2) {
				if (s1->GetBox().y < s2->GetBox().y) {
					s2->CallAction("death");
					s1->GetGravityHandler().Jump();


					CollisionChecker::GetSingleton().Cancel(s1, s2);
				}
				else {
					if (s1->bAttacking) {
						//SpriteManager::GetSingleton().Remove(s2);
						s2->CallAction("death");
						CollisionChecker::GetSingleton().Cancel(s1, s2);
						printf("character sliced goomba");
					}
					else {
						printf("mario died from uba luba\n");
						s1->CallAction("damage");
					}

				}
			}
		);
	}

	for (Sprite* zombie : SpriteManager::GetSingleton().GetTypeList("zombie"))
	{
		CollisionChecker::GetSingleton().Register(mario, zombie,
			[](Sprite* s1, Sprite* s2) {
				if (s1->GetBox().y < s2->GetBox().y) {
					printf("mario died from uba luba\n");
					s1->CallAction("damage");
				}
				else {
					if (s1->bAttacking) {
						//SpriteManager::GetSingleton().Remove(s2);
						s2->CallAction("damage");
						//CollisionChecker::GetSingleton().Cancel(s1, s2);
						printf("character sliced goomba");
					}
					else {
						printf("mario died from uba luba\n");
						s1->CallAction("damage");
					}
				}

			}
		);
	}

	for (Sprite* brick : SpriteManager::GetSingleton().GetTypeList("question_brick"))
	{
		CollisionChecker::GetSingleton().Register(mario, brick,
			[glLayer, g](Sprite* s1, Sprite* s2) {

				printf("MARIO HIT A BRICK {%d %d}\n", s2->x - s1->x, s2->y - s1->y);
				if (s1->y <= s2->y + s2->GetBox().h) {
					printf("HIT FROM BELOW {%d %d}\n", s2->x - s1->x, s2->y - s1->y);
					s1->GetGravityHandler().SetJumping(false);
					//s1->GetGravityHandler().SetFalling(true);
					s1->GetGravityHandler().SetJumpSpeed(1);
					s1->Move(0, 4);

					SpriteManager::GetSingleton().SpawnSprite(Config::GetConfig("config/sprites/hppotion.json"), "hppotion", "hppotion", s2->x + 4, s2->y - 32, glLayer, g);

					// make block solid
					glLayer->SetGridTileBlock(s2->x / 16, s2->y / 16, GridLayer::GRID_SOLID_TILE);
					CollisionChecker::GetSingleton().Cancel(s1, s2);
					SpriteManager::GetSingleton().Remove(s2);
				}
				else {
					printf("HIT FROM ELSEWHERE\n");
				}
			}
		);
	}

	for (Sprite* pipe : SpriteManager::GetSingleton().GetTypeList("pipe"))
	{
		CollisionChecker::GetSingleton().Register(mario, pipe,
			[glLayer](Sprite* s1, Sprite* s2) {
				//printf("MARIO HIT A PIPE\n");
				printf("%d %d\n", s2->x, s2->y);
				if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_DOWN)) {
					if (s2->x == 2032 && s2->y == 1488) {
						/*s1->x = 16 * 113;
						s1->y = 16 * 27;*/
						s1->x = 16 * 3;
						s1->y = 16 * 12;
					}
					else if (s2->x == 624 && s2->y == 352) {
						s1->x = 130 * 16;
						s1->y = 91 * 16;
					}

					if (s2->x == 1376 && s2->y == 1472) {
						s1->x = 16 * 1;
						s1->y = 16 * 2;
					}
					else if (s2->x == 624 && s2->y == 352) {
						s1->x = 130 * 16;
						s1->y = 91 * 16;
					}

					if (s2->x == 1104 && s2->y == 96) {
						s1->x = 239 * 16;
						s1->y = 4 * 16;
					}

					if (s2->x == 4720 && s2->y == 176) {
						s1->x = 169 * 16;
						s1->y = 93 * 16;
					}
				}
			}
		);
	}

	for (Sprite* ladder : SpriteManager::GetSingleton().GetTypeList("ladder"))
	{
		CollisionChecker::GetSingleton().Register(mario, ladder,
			[glLayer](Sprite* s1, Sprite* s2) {
				if (al_key_down(&ksKeyboardState, ALLEGRO_KEY_UP)) {
					s1->GetGravityHandler().Disable();
					s1->Move(0, -2);
					if (s1->y == 1392) {
						s1->x = 3568;
						s1->GetGravityHandler().Enable();
					}
				}
			}
		);
	}

	for (Sprite* coin : SpriteManager::GetSingleton().GetTypeList("coin"))
	{
		CollisionChecker::GetSingleton().Register(mario, coin,
			[](Sprite* s1, Sprite* s2) {



				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s2->id + ".idle.right")->Stop();
				SpriteManager::GetSingleton().RemoveTypeList("coin", s2);
				SpriteManager::GetSingleton().Remove(s2);
				CollisionChecker::GetSingleton().Cancel(s1, s2);

			}
		);
	}

	for (Sprite* death : SpriteManager::GetSingleton().GetTypeList("death"))
	{
		CollisionChecker::GetSingleton().Register(mario, death,
			[](Sprite* s1, Sprite* s2) {
				s1->x = 2 * 16;
				s1->y = 90 * 16;
			}
		);
	}



	for (Sprite* turtle : SpriteManager::GetSingleton().GetTypeList("turtle"))
	{
		CollisionChecker::GetSingleton().Register(mario, turtle,
			[glLayer, g](Sprite* s1, Sprite* s2) {

				//CollisionChecker::GetSingleton().Cancel(s1, s2);
				if (s1->GetBox().y < s2->GetBox().y) {
					//SpriteManager::GetSingleton().Remove(s2);
					//s2->CallAction("death");


					s1->GetGravityHandler().Jump();

					//SpriteManager::GetSingleton().SpawnSprite(Config::GetConfig("config/sprites/hppotion.json"), "hppotion", "hppotion", s2->x + 32, s2->y - 32, glLayer, g);


					Sprite* shell = SpriteManager::GetSingleton().SpawnSprite(Config::GetConfig("config/sprites/shell.json"), "shell", "shell", s2->x, s2->y, glLayer, g);

					shell->bDead = true;


					shell->RegisterDefaultActions();



					for (Sprite* zombie : SpriteManager::GetSingleton().GetTypeList("zombie"))
					{
						CollisionChecker::GetSingleton().Register(shell, zombie,
							[](Sprite* s1, Sprite* s2) {
								printf("shell hitted zombie\n");


								s2->CallAction("damage");



							}
						);
					}

					for (Sprite* main : SpriteManager::GetSingleton().GetTypeList("main"))
					{
						CollisionChecker::GetSingleton().Register(shell, main,
							[](Sprite* s1, Sprite* s2) {
								printf("shell hitted main\n");


								s2->CallAction("damage");



							}
						);
					}


					Sprite* main_character = SpriteManager::GetSingleton().GetTypeList("main").front();


					CollisionChecker::GetSingleton().Register(shell, main_character,
						[](Sprite* s1, Sprite* s2) {

							if (s2->GetBox().y < s1->GetBox().y) {
								s2->GetGravityHandler().Jump();
								s1->bDead = true;

							}
							else
								if (s2->GetBox().x < s1->GetBox().x) { //hit from left


									s1->bLooking = true;
									s1->bDead = false;

								}
								else if (s2->GetBox().x > s1->GetBox().x) {//hit from right
									s1->bDead = false;
									s1->bLooking = false;
								}


						}
					);

					CollisionChecker::GetSingleton().Cancel(s1, s2);
					SpriteManager::GetSingleton().Remove(s2);

				}


				/*
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".run.left")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".run.right")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".idle.left")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".idle.right")->Stop();
				s1->id = "herochar";
				s1->CallAction("idle");
				*/
			}
		);
	}
	for (Sprite* mushroom : SpriteManager::GetSingleton().GetTypeList("mushroom"))
	{
		CollisionChecker::GetSingleton().Register(mario, mushroom,
			[](Sprite* s1, Sprite* s2) {
				CollisionChecker::GetSingleton().Cancel(s1, s2);
				SpriteManager::GetSingleton().Remove(s2);

				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".run.left")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".run.right")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".idle.left")->Stop();
				AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s1->id + ".idle.right")->Stop();
				s1->id = "herochar";
				s1->CallAction("idle");

				std::cout << "Mario Ate Shroom" << std::endl;
			}
		);
	}

	for (Sprite* flag : SpriteManager::GetSingleton().GetTypeList("flag"))
	{
		CollisionChecker::GetSingleton().Register(mario, flag,
			[g](Sprite* s1, Sprite* s2) {
				g->bInputAllowed = false;
				g->bIsGameEnding = true;
			}
		);
	}

	for (Sprite* finishsign : SpriteManager::GetSingleton().GetTypeList("finishsign"))
	{
		CollisionChecker::GetSingleton().Register(mario, finishsign,
			[g](Sprite* s1, Sprite* s2) {
				g->bInputAllowed = false;
				g->bIsGameEnding = true;
			}
		);
	}

	for (Sprite* hppotion : SpriteManager::GetSingleton().GetTypeList("hppotion"))
	{
		CollisionChecker::GetSingleton().Register(mario, hppotion,
			[](Sprite* s1, Sprite* s2) {
				CollisionChecker::GetSingleton().Cancel(s1, s2);
				SpriteManager::GetSingleton().Remove(s2);

				s1->health += 5;

				std::cout << "Mario Got 5 HP" << std::endl;
			}
		);
	}

	for (Sprite* breakable_box : SpriteManager::GetSingleton().GetTypeList("breakable_box"))
	{
		CollisionChecker::GetSingleton().Register(mario, breakable_box,
			[](Sprite* s1, Sprite* s2) {
				printf("MARIO HIT A BRICK {%d %d}\n", s2->x - s1->x, s2->y - s1->y);
				if (s1->y + s1->GetBox().h <= s2->y + s2->GetBox().h) {
					int dy = s1->y + s1->GetBox().h - s2->y;
					s1->y -= dy;
				}
				else
					if (s1->y <= s2->y + s2->GetBox().h) {
						printf("HIT FROM BELOW {%d %d}\n", s2->x - s1->x, s2->y - s1->y);
						s1->GetGravityHandler().SetJumping(false);
						s1->GetGravityHandler().SetJumpSpeed(1);
						s1->Move(0, 4);
						// make block solid
						CollisionChecker::GetSingleton().Cancel(s1, s2);
						SpriteManager::GetSingleton().Remove(s2);
					}
					else {
						printf("HIT FROM ELSEWHERE\n");
					}
			}
		);
	}
}


void SuperMario::RegisterCollisionsActions(void) {
	registerCollisionsActions(this->game.mMap->GetTileLayer()->GetGridLayer(), &this->game);
}


void FramList_Action(Sprite* sprite, Animator* animator, const FrameListAnimation& anim) {
	//printf("FramList_Action\n");

	FrameListAnimator* frameListAnimator = (FrameListAnimator*)animator;

	sprite->currFilm = FilmHolder::Get().GetFilm(((Animation&)anim).GetTrimmedID());
	sprite->SetFrame(anim.GetFrames().at(frameListAnimator->GetCurrFrame()));
}



void SuperMario::Load(void) {

	json jGameConfig = Config::GetConfig("config/game.json");
	Bitmap* bm = al_load_bitmap("resources/sprites/marioi.png");
	Bitmap* bm_enemies = al_load_bitmap("resources/sprites/enemies.png");

	int currentMap = 5;

	/*
	* Set Event Handlers
	*/

	this->game.SetRender(std::bind(&Game::RenderHandler, &this->game));
	this->game.SetInput(std::bind(&Game::InputHandler, &this->game));
	this->game.SetPhysics(std::bind(&Game::PhysicsHandler, &this->game));
	this->game.SetCollisionChecking(std::bind(&Game::CollisionHandler, &this->game));
	this->game.SetAI(std::bind(&Game::AIHandler, &this->game));
	this->game.SetProgressAnimations(std::bind(&Game::AnimationHandler, &this->game));

	/*
	* Create Map
	*/
	this->game.SetMap(new Map(Config::GetConfig(Config::GetConfig("config/game.json")["maps"][currentMap]["cfg"])));

	/*
	* Parse Objects
	*/
	json jMapConfig = Config::GetConfig(Config::GetConfig("config/game.json")["maps"][currentMap]["cfg"]);

	this->game.mMap->ParseObjects(jMapConfig["objects"]);
	SpawnObjects(jMapConfig["objects"]);


	this->RegisterCollisionsActions();


	//addItemToTypeList("enemy_piranha_plant", js_enemies["enemy_piranha_plant"][str]["x_pos"], js_enemies["enemy_piranha_plant"][str]["y_pos"], js_enemies["enemy_piranha_plant"][str]["width"], js_enemies["enemy_piranha_plant"][str]["height"], bm_enemies);

	/*
	* Ready Sprites
	*/
	GridLayer* glLayer = this->game.mMap->GetTileLayer()->GetGridLayer();
	for (Sprite* s : SpriteManager::GetSingleton().GetDisplayList()) {
		/*
		* Default Mover
		*/
		s->SetMover(s->MakeSpriteGridLayerMover(glLayer));

		/*
		* Gravity Handlers
		*/
		//s->GetGravityHandler().Enable();
		s->GetGravityHandler().SetOnSolidGroud(
			[glLayer](Rect r) {
				//printf("Fell On: {%d %d %d %d}\n", r.x, r.y, r.w, r.h);
				return glLayer->IsOnSolidGround(r);
			}
		);
		s->GetGravityHandler().SetOnStartFalling(
			[](void) {
				//std::cout << "Mario Starts Falling\n";
				return;
			}
		);
		s->GetGravityHandler().SetOnStopFalling(
			[](void) {
				//std::cout << "Mario Stops Falling\n";
				return;
			}
		);

	}

}

SuperMario::SuperMario(void) {

}

void SuperMario::Clear(void) {
	al_destroy_display(display);
	audio_sample->stopSample();


	for (int i = 0; i < Audio::singleton.samples_vec.size(); ++i) {
		al_destroy_sample(Audio::singleton.samples_vec.at(i));
	}
	for (int i = 0; i < Audio::singleton.samples_instances_vec.size(); ++i) {
		al_destroy_sample_instance(Audio::singleton.samples_instances_vec.at(i));
	}


	al_destroy_mixer(Audio::singleton.Mixer);
	al_destroy_voice(Audio::singleton.Voice);

}


bool SuperMario::SpawnObjects(json jObjectConfig) {
	Map* map = this->game.mMap;
	int** lObjLayer = map->GetObjectLayer();
	int lSpriteCounter = 0;

	/*
	* Load Playable Animations
	*/
	for (auto& js : jObjectConfig["bindings"]) {
		if (js["is_playable"]) {
			json jExternalConfig = Config::GetConfig(js["external_path"]);
			std::string sExternalName(js["external_name"]);

			//bobjbitmap object's bitmap, all the png
			Bitmap* bObjBitmap = al_load_bitmap(std::string(jExternalConfig["spritesheet"]).c_str());

			for (auto& jAnim : jExternalConfig["sprites"][sExternalName]["animations"]) {
				FilmHolder::Get().Load(jAnim["id"], jAnim["animation"], bObjBitmap);

			}

			for (auto& jAnim : jExternalConfig["sprites"][sExternalName]["animations"]) {
				//get the frames list for the current sprite
				std::vector<unsigned> frames;
				for (auto& frame : jAnim["animation_frames"]) {
					frames.push_back(frame);
				}

				int reps = 1, dx = 2, dy = 0;
				unsigned int delay = 75;

				if (jAnim["id"] == "herochar.idle.right" || jAnim["id"] == "herochar.idle.left")
					delay = jAnim["delay"];

				//missleading variable names 
				FrameListAnimation* hero_run = new FrameListAnimation(jAnim["id"], frames, reps, dx, dy, delay);
				FrameListAnimator* hero_run_animator = new FrameListAnimator();
				hero_run_animator->SetOnAction(
					[](Animator* animator, const Animation& anim) {
						std::string animId = ((FrameListAnimation&)anim).id;
						Sprite* s = SpriteManager::GetSingleton().GetSprite(animId.substr(0, animId.find(".")));
						FramList_Action(s, animator, (const FrameListAnimation&)anim);
					}
				);

				hero_run_animator->SetOnFinish([](Animator* anim) {
					anim->setState(ANIMATOR_STOPPED);
					});

				hero_run_animator->SetOnStart([](Animator* anim) {
					anim->setState(ANIMATOR_RUNNING);
					});
				hero_run_animator->setAnimation(hero_run);
				AnimatorManager::GetSingleton().Register(hero_run_animator);
			}
		}
	}

	/*
	* Spawn Sprites
	*/
	for (int i = 0; i < map->GetHeightTileSize(); ++i) {
		for (int j = 0; j < map->GetWidthTileSize(); ++j) {
			for (auto& js : jObjectConfig["bindings"]) {
				if (lObjLayer[i][j] == js["tile"]) {
					lSpriteCounter++;
					//std::cout << "(" << i << ", " << j << ") " << js["name"] << std::endl;

					//is_external is in different tileset
					if (js["is_external"]) {
						json jExternalConfig = Config::GetConfig(js["external_path"]);
						std::string sExternalName(js["external_name"]);

						//bobjbitmap object's bitmap, all the png
						Bitmap* bObjBitmap = al_load_bitmap(std::string(jExternalConfig["spritesheet"]).c_str());


						/*
						* Parse Animations
						* cut animations from object's bitmap
						*/
						for (auto& jAnim : jExternalConfig["sprites"][sExternalName]["animations"]) {
							FilmHolder::Get().Load(jAnim["id"], jAnim["animation"], bObjBitmap);
						}

						/*
						* Get default animation
						*/
						Film* fDefaultFilm = FilmHolder::Get().GetFilm(jExternalConfig["sprites"][sExternalName]["default_animation"]);
						Rect rDefaultBox = fDefaultFilm->GetFrameBox(0);

						// std::string(js["name"]) + std::string("_") + std::to_string(lSpriteCounter)
						Sprite* newSprite;

						if (js["is_playable"])
							newSprite = addItemToTypeList("main", std::string(js["name"]), rDefaultBox.x, rDefaultBox.y, rDefaultBox.w, rDefaultBox.h, bObjBitmap, 16 * j, 16 * i);
						else
							newSprite = addItemToTypeList(js["name"], std::string(js["name"]) + "_" + std::to_string(lSpriteCounter), rDefaultBox.x, rDefaultBox.y, rDefaultBox.w, rDefaultBox.h, bObjBitmap, 16 * j, 16 * i);
						newSprite->dx = 1;
						//all the shits have default left right idle and moving
						newSprite->RegisterDefaultActions();
						newSprite->currFilm = fDefaultFilm;
						newSprite->GetGravityHandler().Enable();
						newSprite->health = js["health"];

						if (js["has_gravity"] == false)
							newSprite->GetGravityHandler().Disable();

						// test


						for (auto& jAnim : jExternalConfig["sprites"][sExternalName]["animations"]) {
							//get the frames list for the current sprite
							std::vector<unsigned> frames;
							for (auto& frame : jAnim["animation_frames"]) {
								frames.push_back(frame);
							}

							int reps = 1, dx = 2, dy = 0;
							unsigned int delay = 75;

							if (jAnim["id"] == "herochar.idle.right" || jAnim["id"] == "herochar.idle.left" || jAnim["id"] == "coin.idle.right" || jAnim["id"] == "coin.idle.left")
								delay = jAnim["delay"];

							// make id
							std::string id = std::string(jAnim["id"]);
							std::string anim_id = id.substr(0, id.find(".")) + "_" + std::to_string(lSpriteCounter);
							id.erase(0, id.find("."));
							anim_id += id;

							//missleading variable names 
							FrameListAnimation* hero_run = new FrameListAnimation(anim_id, frames, reps, dx, dy, delay);
							FrameListAnimator* hero_run_animator = new FrameListAnimator();
							hero_run_animator->setAnimation(hero_run);

							hero_run_animator->SetOnAction(
								[](Animator* animator, const Animation& anim) {
									std::string animId = ((FrameListAnimation&)anim).id;
									Sprite* s = SpriteManager::GetSingleton().GetSprite(animId.substr(0, animId.find(".")));
									FramList_Action(s, animator, (const FrameListAnimation&)anim);
								}
							);

							hero_run_animator->SetOnFinish([](Animator* anim) {
								anim->setState(ANIMATOR_STOPPED);
								});

							hero_run_animator->SetOnStart([](Animator* anim) {
								anim->setState(ANIMATOR_RUNNING);
								});
							AnimatorManager::GetSingleton().Register(hero_run_animator);
						}

						if (js["external_name"] == "goomba" || js["external_name"] == "slime") {
							newSprite->RegisterAction("death", [](Sprite* s) {
								s->bDead = true;
								Animator* pAnim = AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".death");

								AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".run.left")->Stop();
								AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".run.right")->Stop();
								AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".idle.right")->Stop();
								AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".idle.left")->Stop();

								if (pAnim->HasFinished()) {
									((FrameListAnimator*)pAnim)->Start(((FrameListAnimator*)pAnim)->getAnimation(), SystemClock::Get().getgametime());
									AnimatorManager::GetSingleton().MarkAsRunning(pAnim);
									((FrameListAnimator*)pAnim)->SetOnFinish(
										[](Animator* anim) {
											std::string animId = ((FrameListAnimator*)anim)->getAnimation()->GetId();
											Sprite* s = SpriteManager::GetSingleton().GetSprite(animId.substr(0, animId.find(".")));
											SpriteManager::GetSingleton().Remove(s);
										}
									);
								}
								});
						}

						if (js["is_playable"]) {
							newSprite->dx = 2;

							//specific callbacks for the main character
							Game* game = &this->game;
							newSprite->RegisterAction("attack.sword", [game](Sprite* s) {
								s->bAttacking = true;

								Animator* pAnim, * pAnimBoomer = NULL;

								if (s->bLooking)
								{
									pAnim = AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".attack.sword.right");

								}
								else
									pAnim = AnimatorManager::GetSingleton().GetAnimatorByAnimationID(s->id + ".attack.sword.left");


								if (pAnim->HasFinished()) {
									//sMario->currFilm = FilmHolder::Get().GetFilm("mario.walking.right");
									if (s->bLooking)
										s->currFilm = FilmHolder::Get().GetFilm(s->GetTrimmedID() + ".attack.sword.right");
									else
										s->currFilm = FilmHolder::Get().GetFilm(s->GetTrimmedID() + ".attack.sword.left");
									((FrameListAnimator*)pAnim)->Start(((FrameListAnimator*)pAnim)->getAnimation(), SystemClock::Get().getgametime());
									AnimatorManager::GetSingleton().MarkAsRunning(pAnim);
								}
								});
						}


						// test



					}
					else {
						//pre registered map tiles
						Bitmap* bm = this->game.mMap->GetTiles()[js["tile"]];

						Sprite* s = new Sprite(js["name"], j * 16, i * 16, bm, 16, 16, false);
						s->GetGravityHandler().Disable();

						SpriteManager::GetSingleton().Add(s);
						SpriteManager::GetSingleton().AddToTypeList(js["name"], s);
					}

				}
			}
		}
	}

	return true;
}