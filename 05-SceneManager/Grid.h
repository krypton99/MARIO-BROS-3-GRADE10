#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH	8
#define CELL_HEIGHT	6
#define BRICK_BBOX 16

class CGrid
{
	int map_width;
	int map_height;

	int num_col;
	int num_row;

	vector<vector<vector<LPGAMEOBJECT>>> cells;

public: 
	CGrid(int map_width, int map_height);
	~CGrid() {}

	void Clear(int num_row, int num_col);
	void UpdateOnGrid(vector<LPGAMEOBJECT> objects);
	void ResetGrid(vector<LPGAMEOBJECT>& objects);
	void GetObjectFromGrid(vector<LPGAMEOBJECT>& objects);
	void PushObjectsIntoGrid(LPGAMEOBJECT obj, int row, int col);
	void PushNewObjIntoGrid(LPGAMEOBJECT obj);
};
typedef CGrid* LPGRID;
