#pragma once

#include "KeyEventHandler.h"
#define WORLD_MAP_SCENE 4
/*
*  Abstract class for a game scene
*/
class CScene
{
protected:
	LPKEYEVENTHANDLER key_handler;
	int id;
	LPCWSTR sceneFilePath;

public: 
	CScene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		this->key_handler = NULL;
	}
	float player_x = NULL;
	float player_y = NULL;
	BOOLEAN switchByPortal = false;
	int inType=NULL;
	int outType=NULL;
	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
};
typedef CScene * LPSCENE;


class CSceneKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CSceneKeyHandler(LPSCENE s) :CKeyEventHandler() { scence = s; }
};

typedef CSceneKeyHandler* LPSCENEKEYHANDLER;