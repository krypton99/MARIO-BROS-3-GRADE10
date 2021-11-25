#include "Grid.h"
#include "Map.h"
#include "debug.h"
#include "AssetIDs.h"

CGrid::CGrid(int map_width, int map_height) {
	this->map_height = map_height;
	this->map_width = map_width;
	
	this->num_col = ceil(float(this->map_width / BRICK_BBOX) / CELL_WIDTH);
	this->num_row = ceil(float(this->map_height / BRICK_BBOX) / CELL_HEIGHT);
	cells.resize(num_row);
	for (int i = 0;i < num_row;i++) {
		cells[i].resize(num_col);
	}
}

void CGrid::Clear(int num_row, int num_col) {
	for (int i = 0;i < num_row;i++) {
		for (int j = 0;j < num_col;j++) {
			cells[i][j].clear();
		}
	}
}

void CGrid::UpdateOnGrid(vector<LPGAMEOBJECT> objects) {
	Clear(num_row, num_col);

	for (int i = 0; i < objects.size(); i++)
	{
		float l, t, r, b;
		objects[i]->GetBoundingBox(l, t, r, b);
	
		if (b > map_height|| l < 0 || r >map_width)
		{
			objects[i]->SetState(STATE_REMOVE);
		}

		int top = int(t / (CELL_HEIGHT * BRICK_BBOX));
		int bottom = ceil(b / (CELL_HEIGHT * BRICK_BBOX));
		int left = int(l / (CELL_WIDTH * BRICK_BBOX));
		int right = ceil(r / (CELL_WIDTH * BRICK_BBOX));

		if (objects[i]->GetState() != STATE_REMOVE) {
			for (int row = top; row < bottom; row++)
				for (int col = left; col < right; col++) {
				/*	DebugOut(L"COL=====  %d \n", col);
					DebugOut(L"ROW=====  %d \n", row);*/

					cells[row][col].push_back(objects[i]);

				}
		}
	}
}
void CGrid::ResetGrid(vector<LPGAMEOBJECT>& objects) {
	for (int i = 0; i < objects.size(); i++)
		objects[i]->isInGrid = false;
}

void CGrid::GetObjectFromGrid(vector<LPGAMEOBJECT>& objects) {

	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / (CELL_WIDTH * BRICK_BBOX));
	int lastCol = ceil((CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH) / (CELL_WIDTH * BRICK_BBOX));

	// check firstCol co nam o dau map? ( 1 : 0) 
	firstCol = (firstCol >= 1) ? firstCol - 1 : firstCol;
	lastCol = (lastCol < num_col) ? lastCol + 1 : lastCol;

	DebugOut(L"first Col 2 -======== = %d \n", firstCol);
	DebugOut(L"last Col 2 -======== = %d \n", lastCol);

	for (int i = 0; i < num_row; i++) {
		for (int j = firstCol; j < lastCol; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (!cells[i][j][k]->isInGrid
					&& cells[i][j][k]->GetState() != STATE_REMOVE)
				{
					cells[i][j][k]->isInGrid = true;
					objects.push_back(cells[i][j][k]);
				}
			}
		}
	}

}

void CGrid::PushObjectsIntoGrid(LPGAMEOBJECT obj, int row, int col) {

	cells[row][col].push_back(obj);
}


void CGrid::PushNewObjIntoGrid(LPGAMEOBJECT obj) {

	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	int top = int(t / (CELL_HEIGHT * BRICK_BBOX));
	int bottom = ceil(b / (CELL_HEIGHT * BRICK_BBOX));
	int left = int(l / (CELL_WIDTH * BRICK_BBOX));
	int right = ceil(r / (CELL_WIDTH * BRICK_BBOX));

	if (obj->GetState() != STATE_REMOVE) {
		for (int row = top; row < bottom; row++)
			for (int col = left; col < right; col++)
				cells[row][col].push_back(obj);	
	}
}