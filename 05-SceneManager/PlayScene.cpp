#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Funnel.h"
#include "Platform.h"
#include "AnimationSet.h"
#include "VenusFireTrap.h"
#include "Koopas.h"
#include "SampleKeyEventHandler.h"
#include "Item.h"
#include "Mushroom.h"
#include "Ghost.h"
#include "Leaf.h"
#include "Pswitch.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

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
		if (switchByPortal==false) {
			obj->SetPosition(x, y);
		}
		else {
			obj->SetPosition(player_x, player_y);
			player->isOutPipe = true;
			player->getOutPipe->Start();
			player->OutPipeType = outType;
		}
		objects.push_back(obj);
		player->tail->GetInstance(x, y);
		/*player->tail->SetPosition(x, y);*/
		objects.push_back(player->tail);
		DebugOut(L"[INFO] Player object has been created!\n");
		hud = new HUD();
		break;
	}
	case OBJECT_TYPE_GOOMBA: 
	{
		float type = (float)atof(tokens[3].c_str());
		int top = atoi(tokens[4].c_str());
		int bot = atoi(tokens[5].c_str());
		int left = atoi(tokens[6].c_str());
		int right = atoi(tokens[7].c_str());
		obj = new CGoomba(x, y,type); 
		obj->SetPosition(x, y);
		listMoving.push_back(obj);
		for (int row = top; row < bot; row++) {
			for (int col = left; col < right; col++)
				grid->PushObjectsIntoGrid(obj, row, col);
		}
		/*objects.push_back(obj);*/
		break;
	}
	case OBJECT_TYPE_BRICK: {
		float brickType = (float)atof(tokens[3].c_str());
		float itemType = (float)atof(tokens[4].c_str());
		obj = new CBrick(x, y, brickType,itemType); break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_FUNNEL: {
		float funnelType = (int)atof(tokens[3].c_str());
		obj = new CFunnel(x, y, funnelType); break;
	}
	case OBJECT_TYPE_VENUS:obj = new CVenusFireTrap(x, y); break;
	case OBJECT_TYPE_TROOPA:
	{
		float type= (float)atof(tokens[3].c_str());
		int top = atoi(tokens[4].c_str());
		int bot = atoi(tokens[5].c_str());
		int left = atoi(tokens[6].c_str());
		int right = atoi(tokens[7].c_str());
		obj = new CKoopas(x, y, type);
		obj->SetPosition(x, y);
		listMoving.push_back(obj);
		for (int row = top; row < bot; row++) {
			for (int col = left; col < right; col++)
				grid->PushObjectsIntoGrid(obj, row, col);
		}
		/*objects.push_back(obj);*/

		break;
	}
	
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		bool isThrough = atoi(tokens[9].c_str());
		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, isThrough
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		int portal_in_type = atoi(tokens[6].c_str());
		float playerx = (float) atof(tokens[7].c_str());
		float playery = (float) atof(tokens[8].c_str());
		int portal_out_type = (float)atoi(tokens[9].c_str());
		obj = new CPortal(x, y, r, b, scene_id,portal_in_type,playerx,playery,portal_out_type);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (object_type != OBJECT_TYPE_GOOMBA && object_type != OBJECT_TYPE_TROOPA && object_type !=OBJECT_TYPE_MARIO) {
		obj->SetPosition(x, y);

		objects.push_back(obj);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
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
void CPlayScene::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int map_id = atoi(tokens[0].c_str());
	wstring matrix_path = ToWSTR(tokens[1]);
	int widthMap = atoi(tokens[2].c_str());
	int heightMap = atoi(tokens[3].c_str());

	map = new CMap(map_id, matrix_path.c_str(), widthMap, heightMap);
	CMaps::GetInstance()->Add(map_id, map);
	if (map) {
		grid = new CGrid(map->getWidthMap(), map->getHeighthMap());
	}
}

void CPlayScene::Load()
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
}
void CPlayScene::GetObjectToGrid() {
	// xoa het cac object
	//listItems.clear();
	//objects.clear();
	listGrid.clear();
	listObject.clear();
	grid->GetObjectFromGrid(listGrid,player);

	for (UINT i = 0; i < listGrid.size(); i++) {
		listObject.push_back(listGrid[i]);
	}
}
void CPlayScene::Update(DWORD dt)
{
	grid->ResetGrid(listMoving);
	GetObjectToGrid();
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i]->GetType() == OBJECT_TYPE_PSWITCH) {
			CPswitch* p_switch = dynamic_cast<CPswitch*>(objects[i]);
			p_switch->Update(dt, &objects, &listItems);
		} else
		if (objects[i]->GetType() == OBJECT_TYPE_BRICK) {
			CBrick* brick = dynamic_cast<CBrick*>(objects[i]);


			if (brick->isFallingItem) {
				//CREATE ITEM FOLLOW MARIO LEVEL
				Item* item=NULL;
				CGameObject* obj = NULL;
				if (brick->GetItemType() == CONTAIN_MUSHROOM) {
				if (player->GetLevel() == MARIO_LEVEL_BIG)
						item = new CLeaf({ brick->GetPosX(), brick->GetPosY() - ITEM_BBOX*3 , ITEM_LEAF });
				else item = new CMushroom(brick->GetPosX(), brick->GetPosY() - ITEM_BBOX, ITEM_RED_MUSHROOM);
				}
				else if (brick->GetItemType() == CONTAIN_PSWITCH) {
					obj = new CPswitch(brick->GetPosX(), brick->GetPosY());
				}
				
				if (item != NULL) {
					listItems.push_back(item);
				}
				if (obj != NULL) {
					objects.push_back(obj);
				} /*else return;*/
				brick->isFallingItem = false;
			}
		}
	}

	// check collision of platform when mario is jump on top
	for (size_t i = 0; i < objects.size(); i++) {

		if (objects[i]->GetType() == OBJECT_TYPE_PLATFORM) {
			CPlatform* platform = dynamic_cast<CPlatform*>(objects[i]);


			if (platform->isThrough) {
				//CREATE ITEM FOLLOW MARIO LEVEL
				//CMushroom* item = NULL;
				float x, y;
				float l, r, b, t;
				platform->GetBoundingBox(l,t,r,b);
				player->GetPosition(x, y);
				if (y < t ) {
					platform->isBlocking = 1;
				}
				/*else if (x > l && x < r) {
					platform->isBlocking = 1;
				}*/
				
				/*else {
					if (player->GetLevel() >= MARIO_LEVEL_BIG)
						item = new SuperLeaf({ brick->x, brick->y - BRICK_BBOX_SIZE });
					else
						item = new SuperMushroom({ brick->x, brick->y - BRICK_BBOX_SIZE }, ITEM_RED_MUSHROOM);
				}*/
			}
		} 
		if (objects[i]->GetType() == OBJECT_TYPE_TROOPA) {
			CKoopas* koopas = dynamic_cast<CKoopas*>(objects[i]);
			float x, y;
			koopas->GetPosition(x, y);
			if (koopas->isGhostFollow) {
				koopas->ghost = new CGhost(x + 16, y);
				if (koopas->ghost != NULL) {
					objects.push_back(koopas->ghost);
				}
				else return;
				koopas->isGhostFollow = false;
			}
		}
		
	}
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		/*if (objects[i]->GetType() == OBJECT_TYPE_PLATFORM ) {
			CPlatform* brick = dynamic_cast<CPlatform*>(objects[i]);
			if (brick->isThrough) { 
				return; 
			}
			else coObjects.push_back(objects[i]);

		} else*/
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < listItems.size(); i++)
	{
		coObjects.push_back(listItems[i]);
	}
	for (size_t i = 0; i < listObject.size(); i++)
	{
		coObjects.push_back(listObject[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < listObject.size(); i++)
	{
		listObject[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < listItems.size(); i++) {
		listItems[i]->Update(dt, &coObjects);
	}
	
	for (size_t i = 0; i < listItems.size(); i++) {
		if (listItems[i]->GetState() == STATE_ERASE) {
			listItems.erase(listItems.begin() + i);
			i--;
		}
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 
	
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;
	if (player->GetLevel() == MARIO_LEVEL_RACOON && cy< 286-100)
	{
		CGame::GetInstance()->SetCamPos(cx, cy /*cy*/);
	} else
	CGame::GetInstance()->SetCamPos(cx, 286 /*cy*/);
	DebugOutTitle(L"listMoving: %d", int(listMoving.size()));
	grid->UpdateOnGrid(listMoving);

	for (size_t i = 0; i < listObject.size(); i++) {
		if (listObject[i]->GetState() == STATE_ERASE) {

			listObject.erase(listObject.begin() + i);
			i--;
		}
	}

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	map->Render();
	DebugOut(L"objects %d \n", objects.size());
	for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
	for (int i = 0; i < listItems.size(); i++)
		listItems[i]->Render();
	for (int i = 0;i < listObject.size();i++)
		listObject[i]->Render();
	hud->Render(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY() , player);
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	delete hud;
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	/*for (it = listObject.begin(); it != listObject.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}*/
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	for (it = listItems.begin(); it != listItems.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}
	
	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	//LPGAMEOBJECT* temp = CPlayScene::IsGameObjectDeleted;
	/*listObject.erase(
		std::remove_if(listObject.begin(), listObject.end(), CPlayScene::IsGameObjectDeleted),
		listObject.end());*/
	objects.erase(
			std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
			objects.end());
	listItems.erase(
			std::remove_if(listItems.begin(), listItems.end(), CPlayScene::IsGameObjectDeleted),
		listItems.end());

}