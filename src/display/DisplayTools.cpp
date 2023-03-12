#include <allegro5/allegro.h>
#include "display/DisplayTools.h"

void Blit(Bitmap *dest, unsigned int destX, unsigned int destY, Bitmap *src, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height) {
	al_set_target_bitmap(dest);
	al_draw_bitmap_region(src, srcX, srcY, width, height,destX, destY, 0);

	return;
}

void Blit(Display *dest, unsigned int destX, unsigned int destY, Bitmap *src, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height) {
	al_set_target_backbuffer(dest);
	al_draw_bitmap_region(src, srcX, srcY, width, height, destX, destY, 0);
	al_flip_display();

	return;
}


void Blit(Display* dest, const Rect& rDest, Bitmap* src, const Rect& rSrc) {
	al_set_target_backbuffer(dest);
	al_draw_bitmap_region(src, rSrc.x, rSrc.y, rSrc.w, rSrc.h, rDest.x, rDest.y, 0);
	al_flip_display();

	return;
}

void BlitNoRefresh(Display* dDest, Rect& rDest, Bitmap* src, Rect& rSrc) {
	al_draw_bitmap_region(src, rSrc.x, rSrc.y, rSrc.w, rSrc.h, rDest.x, rDest.y, 0);
}

void BlitNoRefreshRotated(Display* dDest, Rect& rDest, Bitmap* src, Rect& rSrc) {
	al_draw_bitmap_region(src, rSrc.x, rSrc.y, rSrc.w, rSrc.h, rDest.x, rDest.y, 0);
}

void Blit(Bitmap *dDest, Rect& rDest,
		Bitmap *dSrc, Rect& rSrc)
{
	al_set_target_bitmap(dDest);
	al_draw_bitmap_region(dSrc, rSrc.x, rSrc.y, rSrc.w, rSrc.h, rDest.x, rDest.y, 0);
	//al_flip_display();
	return;
}

void DrawViewWindow(Display *dDisplay, unsigned int iDestX, unsigned int iDestY, Bitmap *bSource, unsigned int iSrcX, unsigned int iSrcY, unsigned int iWidth, unsigned int iHeight) {
	Blit(dDisplay, 0, 0, bSource, iSrcX, iSrcY, gGameSettings.lWindowWidth, gGameSettings.lWindowHeight);
}

