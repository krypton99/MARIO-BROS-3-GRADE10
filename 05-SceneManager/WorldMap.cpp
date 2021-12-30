#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "WorldMap.h"
#include "Scene.h"
#include "SampleKeyEventHandler.h"
#include "WorldMapObject.h"
using namespace std;

CWorldMap::CWorldMap(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = NULL;
	key_handler = new CWorldMapKeyHandler(this);
}
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAPS	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2
#define ASSETS_SECTION_ANIMATION_SETS 3
#define MAX_SCENE_LINE 1024

void CWorldMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void CWorldMap::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CWorldMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CWorldMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		player->SetStage(this->id);
		if (switchByPortal == false) {
			obj->SetPosition(x, y);
		}
		else {
			obj->SetPosition(player_x, player_y);
			player->isOutPipe = true;
			player->getOutPipe->Start();
			player->OutPipeType = outType;
		}
		listObjects.push_back(obj);
		DebugOut(L"[INFO] Player object has been created!\n");
		hud = new HUD();
		break;
	}
	case OBJECT_TYPE_WORLDMAPOBJ:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CWorldMapObject(x,y, type);

		// General object setup
		obj->SetPosition(x, y);
		listObjects.push_back(obj);
	}
	break;

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		int portal_in_type = atoi(tokens[6].c_str());
		float playerx = (float)atof(tokens[7].c_str());
		float playery = (float)atof(tokens[8].c_str());
		int portal_out_type = (float)atoi(tokens[9].c_str());
		obj = new CPortal(x, y, r, b, scene_id, portal_in_type, playerx, playery, portal_out_type);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (object_type != OBJECT_TYPE_MARIO) {
		obj->SetPosition(x, y);

		listObjects.push_back(obj);
	}
}
void CWorldMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CWorldMap::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());

	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap);
}

void CWorldMap::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line == "[ANIMATION_SETS]") { section = ASSETS_SECTION_ANIMATION_SETS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case ASSETS_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CWorldMap::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[MAPS]") { section = SCENE_SECTION_MAPS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAPS: _ParseSection_MAPS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
	CGame::GetInstance()->SetCamPos(0, 0);
}

void CWorldMap::Update(DWORD dt)
{
	//mario
	player->UpdateWorldMap(dt, &listObjects);

	for (size_t i = 0; i < listObjects.size(); i++)
	{
		listObjects[i]->Update(dt, &listObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

}
void CWorldMap::Render()
{
	map->Render();
	// list objects
	for (int i = 0; i < listObjects.size(); i++)
		listObjects[i]->Render();

	player->Render();
	hud->Render(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY() , player);
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = ((CWorldMap*)scence)->GetPlayer();

	switch (KeyCode)
	{
	/*case DIK_RIGHT:
		if (mario->canWalkRight)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		break;
	case DIK_LEFT:
		if (mario->canWalkLeft)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		break;
	case DIK_UP:
		if (mario->canWalkUp)
			mario->SetState(MARIO_STATE_WALKING_UP);
		break;
	case DIK_DOWN:
		if (mario->canWalkDown)
			mario->SetState(MARIO_STATE_WALKING_DOWN);
		break;*/
	}
}

void CWorldMapKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CWorldMap*)scence)->GetPlayer();
	if (game->IsKeyDown(DIK_RIGHT))
	{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	} else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_WALKING_DOWN);
	}
	else if (game->IsKeyDown(DIK_UP))
	{

		mario->SetState(MARIO_STATE_WALKING_UP);
	} else 
	if (game->IsKeyDown(DIK_S)) {
		mario->SetCanSwitchScene(true);
	}
	else {
		mario->SetCanSwitchScene(false);
		mario->SetState(MARIO_STATE_IDLE);
	}
}
void CWorldMap::Unload()
{
	for (int i = 0; i < listObjects.size(); i++)
		delete listObjects[i];

	listObjects.clear();
	player = NULL;
	delete hud;
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}