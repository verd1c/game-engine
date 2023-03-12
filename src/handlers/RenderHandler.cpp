#include "display/DisplayTools.h"
#include "engine/Game.h"
#include "engine/Sprite.h"
#include <math.h>

void Game::RenderHandler(void) {
	Rect vw = this->mMap->GetTileLayer()->GetViewWindow();
	
	//this->mMap->Display(vw);
	//for (auto obj : SpriteManager::GetSingleton().GetDisplayList())
	//	obj->Display(this->mMap->GetMap(), obj->GetBox());
	//Blit(display, Rect{0, 0, 0, 0}, this->mMap->GetMap(), vw);


	//Sprite* mario = SpriteManager::GetSingleton().GetTypeList("mario").front();
	//Rect r = mario->currFilm->GetFrameBox(mario->frameNo);
	//printf("CHAR {%d %d %d %d} {%d %d %d %d}\n", mario->GetBox().x, mario->GetBox().y, mario->GetBox().w, mario->GetBox().h, r.x, r.y, r.w, r.h);


	al_set_target_backbuffer(display);

	BlitNoRefresh(display, (Rect&)Rect { 0, 0, 640, 480 }, this->mMap->GetBackgroundLayer()->GetBuffer(), (Rect&)Rect { (int)floor(0.5 * vw.x), vw.y, vw.w, vw.h });
	BlitNoRefresh(display, (Rect&)Rect { 0, 0, 640, 480 }, this->mMap->GetTileLayer()->GetBuffer(), vw);

	for (auto obj : SpriteManager::GetSingleton().GetDisplayList()) {
		Rect sb = obj->GetBox();
	/*	Blit(
			dest,
			(Rect&)displayArea,
			this->currFilm->GetBitmap(),
			(Rect&)this->currFilm->GetFrameBox(this->frameNo)
		);*/

		if (obj->currFilm != nullptr && obj->currFilm->GetTotalFrames() > 0) {
			//al_set_target_bitmap(obj->currFilm->GetBitmap());
			//al_draw_rotated_bitmap(obj->currFilm->GetBitmap(), 8, 8, 16, 16, 3.14159265358979323846 / 4, 0);
			//al_set_target_backbuffer(display);
			BlitNoRefresh(display, (Rect&)Rect { sb.x - vw.x, sb.y - vw.y, sb.w, sb.h }, obj->currFilm->GetBitmap(), (Rect&)obj->currFilm->GetFrameBox(obj->frameNo));
		}
		else {
			BlitNoRefresh(display, (Rect&)Rect { sb.x - vw.x, sb.y - vw.y, sb.w, sb.h }, obj->getBitamp(), (Rect&)Rect {0, 0, 16, 16});
		}
	}
	al_flip_display();

	return;
}






// backup

//al_set_target_backbuffer(display);
//BlitNoRefresh(display, (Rect&)Rect{0, 0, 640, 480}, this->mMap->GetTileLayer()->GetBuffer(), vw);
//for (auto obj : SpriteManager::GetSingleton().GetDisplayList()) {
//	Rect sb = obj->GetBox();
///*	Blit(
//		dest,
//		(Rect&)displayArea,
//		this->currFilm->GetBitmap(),
//		(Rect&)this->currFilm->GetFrameBox(this->frameNo)
//	);*/
//	BlitNoRefresh(display, (Rect&)Rect {sb.x - vw.x, sb.y - vw.y, sb.w, sb.h}, obj->currFilm->GetBitmap(), (Rect&)obj->currFilm->GetFrameBox(obj->frameNo));
//}
//al_flip_display();