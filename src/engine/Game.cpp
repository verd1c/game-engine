#include "engine/Game.h"
#include "config/Config.h"
#include "engine/Map.h"
#include "display/DisplayTools.h"

#include"engine/animations/SystemClock.h"
SystemClock SystemClock::singleton;
Display *display;
ALLEGRO_KEYBOARD_STATE ksKeyboardState;

void Game::MainLoop(void) {
	while(IsRunning())
		MainLoopIteration();
}

void Game::MainLoopIteration(void) {
	SystemClock::Get().setgametime();
	Render();
	Input();
	ProgressAnimations();
	AI();
	Physics();
	CollisionChecking();
	//UserCode(); // hook for custom code at end
	//CommitDestructions();
}