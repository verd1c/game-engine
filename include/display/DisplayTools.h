#ifndef __DISPLAY_TOOLS_H__
#define __DISPLAY_TOOLS_H__

#include "engine/Structs.h"

void Blit(Bitmap *dest, unsigned int destX, unsigned int destY, Bitmap *src, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height);
void Blit(ALLEGRO_DISPLAY *dest, unsigned int destX, unsigned int destY, Bitmap *src, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height);





void Blit(Bitmap* dDest, Rect& rDest, Bitmap* dSrc, Rect& rSrc);
void Blit(Display* dest, const Rect& rDest, Bitmap* src, const Rect& rSrc);


void BlitNoRefresh(Display* dDest, Rect& rDest, Bitmap* src, Rect& rSrc);
void BlitNoRefreshRotated(Display* dDest, Rect& rDest, Bitmap* src, Rect& rSrc);
void RefreshDisplay(Display *dDest);


void DrawViewWindow(Display *dDisplay, unsigned int iDestX, unsigned int iDestY, Bitmap *bSource, unsigned int iSrcX, unsigned int iSrcY, unsigned int iWidth, unsigned int iHeight);

void RenderHandler(void);

#endif