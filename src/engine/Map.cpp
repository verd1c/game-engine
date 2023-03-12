#include <fstream>
#include <stdlib.h>
#include "engine/Map.h"
#include "tools/json.h"
#include "tools/csv_parser.h"
#include "display/DisplayTools.h"
#include "engine/Sprite.h"
#include <list>
#include <map>
#include <algorithm>


void Map::Display(Rect rViewWindow)
{
	Blit(this->bMap, rViewWindow, this->GetTileLayer()->GetBuffer(), rViewWindow);
}

TileLayer *Map::GetTileLayer(void)
{
	return this->tlLayer;
}

TileLayer* Map::GetBackgroundLayer(void)
{
	return this->tlBackground;
}

Map::Map(json jConfig) {
	Bitmap *bm = al_load_bitmap(std::string(jConfig["tiles"]["path"]).c_str());

	this->bMap = al_create_bitmap(300 * 16, 100 * 16);
	this->lMapPixelWidth = 300 * 16;
	this->lMapPixelHeight = 100 * 16;

	/*
	* Create tile set
	*/
	this->iTileHeight = jConfig["tiles"]["height"];
    this->iTileWidth = jConfig["tiles"]["width"];
	int counter = 0;
	for (int i = 0; i < al_get_bitmap_height(bm) / this->iTileHeight; ++i) {
		for (int j = 0; j < al_get_bitmap_width(bm) / this->iTileWidth; ++j) {
			this->mTiles.insert({ counter, al_create_sub_bitmap(bm, j * this->iTileHeight, i * this->iTileWidth, this->iTileHeight, this->iTileWidth) });
			counter++;
		}
	}

	/*
	* Create Tile Layer and parse layers
	*/
	TileLayer *tlLayer = new TileLayer(100, 300, this->mTiles, jConfig["grid"]);
	tlLayer->SetViewWindow(Rect{0, 100 * 16 - 480, gGameSettings.lWindowWidth, gGameSettings.lWindowHeight });
	tlLayer->GetGridLayer()->InitTileGridBlocks();
	json jMaps = jConfig["map"];
	for (auto& itMap : jMaps) {
		tlLayer->ParseCSV(itMap["path"]);
	}
	this->tlLayer = tlLayer;

	/*
	* Create Tile Layer and parse layers
	*/
	TileLayer* tlBackground = new TileLayer(100, 300, this->mTiles, jConfig["grid"]);
	tlBackground->SetViewWindow(Rect{ 0, 100 * 16 - 480, gGameSettings.lWindowWidth, gGameSettings.lWindowHeight });
	tlBackground->GetGridLayer()->InitTileGridBlocks();
	jMaps = jConfig["background"];
	for (auto& itMap : jMaps) {
		tlBackground->ParseCSV(itMap["path"]);
	}
	this->tlBackground = tlBackground;
}


std::map<int, Bitmap *> Map::GetTiles() {
	return this->mTiles;
}

Bitmap *Map::GetMap() {
	return this->bMap;
}

void TileLayer::SetTile(int iCol, int iRow, int iIndex)
{
	this->iTileMap[iRow][iCol] = iIndex;
}

int TileLayer::GetTile(int iCol, int iRow)
{
	return this->iTileMap[iRow][iCol];
}

Rect TileLayer::GetViewWindow(void)
{
	Sprite* sMario = nullptr;
	for (auto obj : SpriteManager::GetSingleton().GetTypeList("main")) {
		sMario = obj;
	}


	// HALF FIXED TODO: FIX Y

	int dx = sMario->x - (this->rViewWindow.x + this->rViewWindow.w / 2);
	int dy = sMario->y - (this->rViewWindow.y + this->rViewWindow.h / 2);
	if (sMario->x <= 640 / 2) {
		dx = -this->rViewWindow.x;
	}
	if (sMario->y <= 640 / 2) {
		dy = -this->rViewWindow.y;
	}

	//printf("Mario|%d %d| ViewWindow|%d %d| New|%d %d|\n", sMario->x, sMario->y, (this->rViewWindow.x + this->rViewWindow.w / 2), (this->rViewWindow.y + this->rViewWindow.h / 2), dx, dy);
	ScrollWithBoundsCheck(&this->rViewWindow, dx, dy);

	return this->rViewWindow;
}

void TileLayer::SetViewWindow(Rect rRect)
{
	//printf("IMMA SET VW TO {%d %d %d %d}!\n", rRect.x, rRect.y, rRect.w, rRect.h);
	this->rViewWindow = rRect;
}

Bitmap* TileLayer::GetBuffer(void)
{
	return this->bBuffer;
}

GridLayer* TileLayer::GetGridLayer(void)
{
	return &this->glLayer;
}

bool TileLayer::ParseCSV(std::string sPath)
{
	std::ifstream iFile(sPath);
	std::string sIndex = "";

	int i = 0, j = 0;
	while (std::getline(iFile, sIndex, ',')) {
		this->iTileMap[i][j] = stoi(sIndex);

		if (sIndex.find("\n") != std::string::npos) {
			i++;
			j = 0;
		} 
		else {
			j++;
		}
	}

	Render();
	this->GetGridLayer()->ComputeTileGridBlocks(this->iTileMap);

	
	//this->glLayer.Print();
	return false;
}

void TileLayer::Scroll(Rect* rViewWindow, int iDx, int iDy)
{
	rViewWindow->x += iDx;
	rViewWindow->y += iDy;
	return;
}

bool TileLayer::CanScrollHorizontal(Rect rViewWindow, int iDx)
{
	return rViewWindow.x >= -iDx &&
			(rViewWindow.x + rViewWindow.w + iDx) <= 300 * 16;
}

bool TileLayer::CanScrollVertical(Rect rViewWindow, int iDy)
{
	return rViewWindow.y >= -iDy &&
		(rViewWindow.y + rViewWindow.h + iDy) <= 100 * 16;
}

void TileLayer::FilterScrollDistance(int viewStartCoord, int viewSize, int* d, int maxMapSize)
{
	auto val = *d + viewStartCoord;
	if (val < 0)
		*d = viewStartCoord;
	else if (viewSize >= maxMapSize)
		*d = 0;
	else if ((val + viewSize) >= maxMapSize)
		*d = maxMapSize - (viewStartCoord + viewSize);

}

void TileLayer::FilterScroll(Rect rViewWindow, int* dx, int* dy)
{
	FilterScrollDistance(rViewWindow.x, rViewWindow.w, dx, 300 * 16);
	FilterScrollDistance(rViewWindow.y, rViewWindow.h, dy, 100 * 16);
}

void TileLayer::ScrollWithBoundsCheck(Rect* rViewWindow, int dx, int dy)
{
	FilterScroll(*rViewWindow, &dx, &dy);
	Scroll(rViewWindow, dx, dy);
}

void TileLayer::Render(void) {

	for (int i = 0; i < this->iRows; ++i) {
		for (int j = 0; j < this->iCols; ++j) {
			if (this->iTileMap[i][j] != -1)
				Blit(this->bBuffer, j * 16, i * 16, this->mTileSet[this->iTileMap[i][j]], 0, 0, 16, 16);
		}
	}
}

void TileLayer::Display(ALLEGRO_DISPLAY* bDest, const Rect& rDisplayArea)
{
	// todo: blit display area to bDest
}

void TileLayer::Scroll(float fDx, float fDy)
{
	// todo: move vwindow
}

TileLayer::TileLayer(int iRows, int iCols, std::map<int, Bitmap*> mTileSet, json jGridConfig) :
	rViewWindow(Rect{ 0, 0, 0, 0 }),
	glLayer(iRows * 4, iCols * 4, jGridConfig),
	mTileSet(mTileSet),
	iRows(iRows),
	iCols(iCols)
{
	// Allocate
	this->iTileMap = new int* [iRows];
	for (int i = 0; i < iRows; ++i) {
		this->iTileMap[i] = new int[iCols];
		memset(this->iTileMap[i], -1, iCols * sizeof(int)); // not sure if needed
	}
	this->bBuffer = al_create_bitmap(300 * 16, 100 * 16); // screen buffer (to zwgrafismeno to-be map)
}



void GridLayer::ComputeTileGridBlocks(int** tlTileMap)
{
	std::list<int> lEmptyList = mGridMasks.find(std::string("empty"))->second;
	std::list<int> lSolidList = mGridMasks.find(std::string("solid"))->second;
	std::list<int> lTopSolidList = mGridMasks.find(std::string("top_solid"))->second;

	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 300; ++j) {
			
			int tile = tlTileMap[i][j];

			if (tile == -1) continue;
			this->SetGridTileBlock(j, i, GridLayer::GRID_EMPTY_TILE);


			if (std::find(lSolidList.begin(), lSolidList.end(), tile) != lSolidList.end()) {
				this->SetGridTileBlock(j, i, GridLayer::GRID_SOLID_TILE);
			}else if (std::find(lTopSolidList.begin(), lTopSolidList.end(), tile) != lTopSolidList.end()) {
				this->SetSolidGridTop(j, i);
				std::cout << "Found " << tile << " in topsolid\n";
			}
		}
	}
}

void GridLayer::InitTileGridBlocks() {
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 300; ++j) {
			this->SetGridTileBlock(j, i, GridLayer::GRID_EMPTY_TILE);
		}
	}
}

void GridLayer::SetGridTileBlock(int iCol, int iRow, int iFlag)
{
	iCol *= 4;
	iRow *= 4;
	for (int i = iRow; i < iRow + 4; ++i) {
		for (int j = iCol; j < iCol + 4; ++j) {
			this->SetGridTile(j, i, iFlag);
		}
	}
}

void GridLayer::SetSolidGridTop(int iCol, int iRow)
{
	iCol *= 4;
	iRow *= 4;
	for (int j = iCol; j < iCol + 4; ++j) {
		this->SetGridTile(j, iRow, GridLayer::GRID_SOLID_TILE);
	}
}

void GridLayer::Print(void)
{
	for (int i = 0; i < this->iRows; ++i) {
		std::cout << "[";
		for (int j = 0; j < this->iCols; ++j) {
			if(this->vGrid[i][j] == GridLayer::GRID_SOLID_TILE) std::cout << this->vGrid[i][j] << ", ";
		}
		std::cout << "]" << std::endl;
	}
}

GridLayer::GridLayer(int iRows, int iCols, json jGridConfig) :
	iRows(iRows),
	iCols(iCols),
	jConfig(jGridConfig)
{
	// Allocate
	this->sTotal = iRows * iCols;
	this->vGrid = new int* [iRows];
	for (int i = 0; i < iRows; ++i) {
		this->vGrid[i] = new int[iCols];
		//memset(this->vGrid[i], GridLayer::GRID_EMPTY_TILE, iCols * sizeof(int)); // not sure if needed
	}

	/*
	* Parse Config
	*/
	json j;
	
	for (json::iterator it = jConfig.begin(); it != jConfig.end(); ++it) {
		std::list<int> lConf;
		for (auto b : *it) {
			lConf.push_back((int)b);
		}
		mGridMasks.insert({ it.key(), lConf });
	}
}

void GridLayer::SetGridTile(int iCol, int iRow, int iIndex)
{
	this->vGrid[iRow][iCol] = iIndex;
	return;
}

int GridLayer::GetGridTile(int iCol, int iRow)
{
	return this->vGrid[iRow][iCol];
}

void GridLayer::SetSolidGridTile(int iCol, int iRow)
{
	this->SetGridTile(iCol, iRow, GridLayer::GRID_SOLID_TILE);
}

void GridLayer::SetGridTileFlags(int iCol, int iRow, int fFlags)
{
	this->SetGridTile(iCol, iRow, fFlags);
}

int GridLayer::CanPassGridTile(int iCol, int iRow, int fFlags)
{
	return GetGridTile(iCol, iRow) & fFlags;
}

void GridLayer::FilterGridMotion(Rect rRect, int* dx, int* dy)
{
	/*if (this->IsOnSolidGround(rRect))
		std::cout << "ON SOLID GROUND\n";*/
	if (*dx < 0)
		FilterGridMotionLeft(rRect, dx);
	else if (*dx > 0)
		FilterGridMotionRight(rRect, dx);
	if (*dy < 0)
		FilterGridMotionUp(rRect, dy);
	else if (*dy > 0)
		FilterGridMotionDown(rRect, dy);
}

bool GridLayer::IsOnSolidGround(Rect rRect) {
	int dy = 1;

	int currPixelY = rRect.y + rRect.h - 1; // Check feet
	int nextPixelY = currPixelY + dy;
	int currGridY = currPixelY >> 2;
	int nextGridY = nextPixelY >> 2;

	if (currGridY == nextGridY) {
		return false;
	}
	int currPixelX = rRect.x;
	int trgtPixelX = currPixelX + rRect.w - 1;
	int currGridX = currPixelX >> 2;
	int trgtGridX = trgtPixelX >> 2;

	for (int x = currGridX; x <= trgtGridX; ++x) {
		if (CanPassGridTile(x, nextGridY, GRID_SOLID_TILE)) {
			return true;
		}
	}
	return false;
}

bool GridLayer::CanMoveLeft(Rect rRect) {
	int dx = -1;
	auto x1_next = rRect.x + dx;

	if (x1_next < 0) {
		//*dx = -rRect.x;
	}
	else {
		auto newCol = x1_next >> 2; // 4
		auto currCol = rRect.x >> 2; // 4
		if (newCol != currCol) {
			auto startRow = rRect.y >> 2;
			auto endRow = (rRect.y + rRect.h - 1) >> 2;

			bool bCanPass = true;
			for (auto row = startRow; row <= endRow; ++row) {
				//printf("Comparing {%d, %d = %d}\n", GetGridTile(newCol, row), GRID_RIGHT_SOLID_MASK, CanPassGridTile(newCol, row, GRID_RIGHT_SOLID_MASK));
				if (CanPassGridTile(newCol, row, GRID_SOLID_TILE)) {
					return false;
				}
			}
		}
	}

	return IsOnSolidGround(Rect{rRect.x - 4, rRect.y, rRect.w, rRect.h});
}

bool GridLayer::CanMoveRight(Rect rRect) {
	int dx = 1;
	auto x2 = rRect.x + rRect.w - 1;
	auto x2_next = x2 + dx;
	if (x2_next >= 300 * 4 * 16) {
		//*dx = ((300 * 4 – 1) - x2);
		//*dx = ((300 * 4 - 1) - x2);
	}
	else {
		auto newCol = x2_next >> 2;
		auto currCol = x2 >> 2;
		if (newCol != currCol) {
			//assert(newCol - 1 == currCol); // we really move right
			auto startRow = rRect.y >> 2;
			auto endRow = (rRect.y + rRect.h - 1) >> 2;

			bool bCanPass = true;
			for (auto row = startRow; row <= endRow; ++row) {
				if (CanPassGridTile(newCol, row, GRID_SOLID_TILE)) {
					return false;
				}
				
			}


		}
	}
	return true;
}

void GridLayer::FilterGridMotionLeft(Rect rRect, int* dx)
{
	auto x1_next = rRect.x + *dx;
	if (x1_next < 0) {
		//*dx = -rRect.x;
	}
	else {
		auto newCol = x1_next >> 2; // 4
		auto currCol = rRect.x >> 2; // 4
		if (newCol != currCol) {
			auto startRow = rRect.y >> 2;
			auto endRow = (rRect.y + rRect.h - 1) >> 2;

			bool bCanPass = true;
			for (auto row = startRow; row <= endRow; ++row) {
				//printf("Comparing {%d, %d = %d}\n", GetGridTile(newCol, row), GRID_RIGHT_SOLID_MASK, CanPassGridTile(newCol, row, GRID_RIGHT_SOLID_MASK));
				if (!CanPassGridTile(newCol, row, GRID_SOLID_TILE)) {
					*dx = (newCol << 2) - rRect.x;
				}
				else {
					bCanPass = false;
				}
			}

			if (!bCanPass)
				*dx = 0;
		}
	}
}

void GridLayer::FilterGridMotionRight(Rect rRect, int* dx)
{
	auto x2 = rRect.x + rRect.w - 1;
	auto x2_next = x2 + *dx;
	if (x2_next >= 300 * 4 * 16) {
		//*dx = ((300 * 4 – 1) - x2);
		//*dx = ((300 * 4 - 1) - x2);
	}
	else {
		auto newCol = x2_next >> 2;
		auto currCol = x2 >> 2;
		if (newCol != currCol) {
			//assert(newCol - 1 == currCol); // we really move right
			auto startRow = rRect.y >> 2;
			auto endRow = (rRect.y + rRect.h - 1) >> 2;

			bool bCanPass = true;
			for (auto row = startRow; row <= endRow; ++row) {
				if (!CanPassGridTile(newCol, row, GRID_SOLID_TILE)) {
					*dx = (((newCol << 2)) - x2);
					//*dx = ((newCol << 2) – 1) - x2;
				}
				else {
					bCanPass = false;
	
				}
			}

			if (!bCanPass)
				*dx = 0;
				
		}
	}
}

void GridLayer::FilterGridMotionDown(Rect rRect, int* dy)
{
	int currPixelY = rRect.y + rRect.h - 1; // Check feet
	int nextPixelY = currPixelY + *dy;
	int currGridY = currPixelY >> 2;
	int nextGridY = nextPixelY >> 2;

	if (nextPixelY >= 100 * 16) {
		std::cout << "Fell Off\n";
		//*dy = ((nextGridY << 2) - 1) - currPixelY;
		*dy = 0;
		return;
	}

	if (currGridY == nextGridY) {
		return;
	}
	int currPixelX = rRect.x;
	int trgtPixelX = currPixelX + rRect.w - 1;
	int currGridX = currPixelX >> 2;
	int trgtGridX = trgtPixelX >> 2;

	bool bCanPass = true;
	for (int x = currGridX; x <= trgtGridX; ++x) {
		if (CanPassGridTile(x, nextGridY, GRID_SOLID_TILE)) {
			*dy = ((nextGridY << 2) - 1) - currPixelY;
			break;
		}
	}
}


void GridLayer::FilterGridMotionUp(Rect rRect, int* dy) {
	auto y1_next = rRect.y + *dy;
	if (y1_next < 0)
		*dy = -rRect.y;
	else {
		auto newRow = y1_next >> 2; // 4
		auto currRow = rRect.y >> 2; // 4
		if (newRow != currRow) {
			auto startCol = rRect.x >> 2;
			auto endCol = (rRect.x + rRect.w - 1) >> 2;

			bool bCanPass = true;
			for (auto col = startCol; col <= endCol; ++col) {
				//printf("Comparing {%d, %d = %d}\n", GetGridTile(col, newRow), GRID_RIGHT_SOLID_MASK, CanPassGridTile(col, newRow, GRID_RIGHT_SOLID_MASK));
				if (!CanPassGridTile(col, newRow, GRID_SOLID_TILE)) {
					*dy = (newRow << 2) - rRect.y;
				}
				else {
					bCanPass = false;
				}
			}

			if (!bCanPass)
				*dy = 0;
		}
	}
}



int **Map::ParseObjects(json jObjectConfig) {
	std::string sSpritesheetPath = jObjectConfig["path"];
	std::ifstream iFile(sSpritesheetPath);
	std::string sIndex = "";

	tlObjectLayer = (int**)malloc(GetHeightTileSize() * sizeof(int*));
	for (int i = 0; i < GetHeightTileSize(); ++i) {
		tlObjectLayer[i] = (int*)malloc(GetWidthTileSize() * sizeof(int));
		for (int j = 0; j < GetWidthTileSize(); ++j) {
			tlObjectLayer[i][j] = -1;
		}
	}

	int i = 0, j = 0;
	while (std::getline(iFile, sIndex, ',')) {
		int index = stoi(sIndex);

		//tlObjectLayer[i][j] = index;
		
	/*	if (index == 43) {
			std::cout << "FOUND GOUMBA AT {" << i << ", " << j << "}\n";
		}*/
		for (auto& js : jObjectConfig["bindings"]) {
			if (index == js["tile"]) {
				tlObjectLayer[i][j] = index;
			}
		}

		if (sIndex.find("\n") != std::string::npos) {
			i++;
			j = 0;
		}
		else {
			j++;
		}
	}


	return tlObjectLayer;
}

int Map::GetHeightTileSize(void) {
	return (int)floor(lMapPixelHeight / iTileHeight);
}
int Map::GetWidthTileSize(void) {
	return (int)floor(lMapPixelWidth / iTileWidth);
}

int** Map::GetObjectLayer(void) {
	return tlObjectLayer;
}