#ifndef __MAP_H__
#define __MAP_H__

#include "config/Config.h"
#include "engine/Structs.h"
#include <algorithm>
#include <map>

class GridLayer {
private:
	int** vGrid = nullptr;
	unsigned sTotal = 0;
	int iRows, iCols;

	std::map<std::string, std::list<int>> mGridMasks;
	json jConfig;

	

public:

	static const byte GRID_THIN_AIR_MASK = 0x00;
	static const byte GRID_LEFT_SOLID_MASK = 0x01;
	static const byte GRID_RIGHT_SOLID_MASK = 0x02;
	static const byte GRID_TOP_SOLID_MASK = 0x04;
	static const byte GRID_BOTTOM_SOLID_MASK = 0x08;
	static const byte GRID_GROUD_MASK = 0x10;
	static const byte GRID_FLOATING_MASK = 0x20;
	static const byte GRID_EMPTY_TILE = GRID_FLOATING_MASK;
	static const byte GRID_SOLID_TILE = (GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | GRID_BOTTOM_SOLID_MASK);

	/*
	* Grid Tile Setting
	*/
	void SetGridTile(int iCol, int iRow, int iIndex);
	int GetGridTile(int iCol, int iRow);
	void SetSolidGridTile(int iCol, int iRow);
	void SetEmptyGridTile(int iCol, int iRow);
	void SetGridTileFlags(int iCol, int iRow, int fFlags);

	int CanPassGridTile(int iCol, int iRow, int fFlags);
	void FilterGridMotion(Rect rRect, int *dx, int *dy);
	void FilterGridMotionLeft(Rect rRect, int *dx);
	void FilterGridMotionRight(Rect rRect, int* dx);
	void FilterGridMotionDown(Rect rRect, int* dy);
	void FilterGridMotionUp(Rect rRect, int* dy);




	bool IsOnSolidGround(Rect rRect);
	bool CanMoveLeft(Rect rRect);
	bool CanMoveRight(Rect rRect);
	
	int** GetBuffer(void);
	const int** GetBuffer(void) const;

	/*
	* Computation
	*/
	void ComputeTileGridBlocks(int **tlTileMap);
	void InitTileGridBlocks();
	void SetGridTileBlock(int iCol, int iRow, int iFlag);
	void SetSolidGridTop(int iCol, int iRow);

	/*
	* Utils
	*/
	void Print(void);

	GridLayer(int iRows, int iCols, json jGridConfig);
};

class TileLayer {
private:
	Rect rViewWindow;

	/*
	* Tile Map
	*/
	int** iTileMap;
	int iRows, iCols;

	/*
	* Grid
	*/
	GridLayer glLayer;

	std::map<int, Bitmap*> mTileSet; // Tile set

	
	Bitmap* bBuffer;

public:
	/*
	* Scrolling
	*/
	void Scroll(Rect* rViewWindow, int iDx, int iDy);
	bool CanScrollHorizontal(Rect rViewWindow, int iDx);
	bool CanScrollVertical(Rect rViewWindow, int iDy);
	static void FilterScrollDistance(int viewStartCoord, int viewSize, int* d, int maxMapSize);
	void FilterScroll(Rect rViewWindow, int *dx, int *dy);
	void ScrollWithBoundsCheck(Rect* rViewWindow, int dx, int dy);

	void Render(void);

	int** GetTileMap(void) { return this->iTileMap; }

	/*
	* Tile Map
	*/
	void SetTile(int iCol, int iRow, int iIndex);
	int GetTile(int iCol, int iRow);

	/*
	* View Window
	*/
	Rect GetViewWindow(void);
	void SetViewWindow(Rect rRect);

	/*
	* Buffer
	*/
	Bitmap* GetBuffer(void);
	
	GridLayer* GetGridLayer(void);


	/*
	* Parse Tile Map
	*/
	bool ParseCSV(std::string sPath);


	void Display(ALLEGRO_DISPLAY* bDest, const Rect& rDisplayArea);
	void Scroll(float fDx, float fDy);

	TileLayer(int iRows, int iCols, std::map<int, Bitmap*> mTileSet, json jGridConfig);
	//TileLayer(void);
};







class Map {
private:

	/*
	* Metadata
	*/
	int lMapPixelWidth, lMapPixelHeight;

	/*
	* Tile Layer
	*/
	TileLayer* tlLayer;
	TileLayer* tlBackground;

	/*
	* Object Tile Layer
	*/
	int** tlObjectLayer;

	std::map<int, Bitmap *> mTiles;

	Bitmap *bMap;
	int iTileWidth, iTileHeight;

public:

	/*
	* Tile Layer
	*/
	TileLayer *GetTileLayer(void);
	TileLayer* GetBackgroundLayer(void);
	int GetHeightTileSize(void);
	int GetWidthTileSize(void);

	/*
	* Object Layer
	*/
	int** ParseObjects(json jObjectConfig);
	int** GetObjectLayer(void);

	Map(json jConfig);

	void Display(Rect rViewWindow);

	std::map<int, Bitmap *> GetTiles();

	Bitmap *GetMap();
};



#endif