/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"


#include "Mario.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_TILE 30

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc.png"
#define TEXTURE_PATH_ENEMIES TEXTURES_DIR "\\enemies.png"
#define TEXTURE_PATH_TILE TEXTURES_DIR "\\tile.png"

CMario *mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.1f

CBrick *brick;
CGoomba *goomba;
vector<LPGAMEOBJECT> objects;
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMIES);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_TILE, TEXTURE_PATH_TILE);

	CSprites * sprites = CSprites::GetInstance();
	LPTEXTURE texGoomba = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 59, 12, 79, 36, texGoomba);
	sprites->Add(30002, 82, 17, 102, 36, texGoomba);

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// readline => id, left, top, right 

	sprites->Add(10001, 246, 154, 259, 181, texMario);
	sprites->Add(10002, 275, 154, 290, 181, texMario);
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 200, 181, texMario);
	sprites->Add(10012, 155, 154, 171, 181, texMario);
	sprites->Add(10013, 125, 154, 141, 181, texMario);

	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 300, 117, 316, 133, texMisc);
	sprites->Add(20002, 318, 117, 334, 133, texMisc);
	sprites->Add(20003, 336, 117, 352, 133, texMisc);
	sprites->Add(20004, 354, 117, 370, 133, texMisc);
	
	LPTEXTURE texTile = textures->Get(ID_TEX_TILE);
	sprites->Add(20011, 443, 154, 459, 170, texTile);// platform left 
	sprites->Add(20012, 477, 154, 493, 170, texTile);// platform right
	sprites->Add(20013, 460, 154, 476, 170, texTile);// platform middle
	sprites->Add(20014, 307, 307, 323, 323, texTile);// tree 1
	sprites->Add(20015, 324, 307, 340, 323, texTile);// tree 2
	sprites->Add(20016, 307, 324, 323, 340, texTile);// tree 3
	sprites->Add(20017, 324, 324, 340, 340, texTile);// tree 4
	sprites->Add(20018, 341, 324, 357, 340, texTile);// tree 5
	sprites->Add(20019, 307, 341, 324, 357, texTile);// tree 6
	sprites->Add(20020, 324, 341, 340, 357, texTile);// tree 7
	sprites->Add(20021, 375, 324, 391, 340, texTile);// bush
	sprites->Add(20022, 1140, 1, 1156, 17, texTile);// question brick
	sprites->Add(20023, 919, 137, 935, 153, texTile);// block top left
	sprites->Add(20024, 953, 137, 969, 153, texTile);// block top right
	sprites->Add(20025, 919, 171, 935, 187, texTile);// block bot left
	sprites->Add(20022, 953, 171, 969, 187, texTile);// block bot right

	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(502, ani);
	
	ani = new CAnimation(100);
	ani->Add(20001,1000);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(510, ani);

	ani = new CAnimation(100);
	ani->Add(20011);
	animations->Add(601, ani);
	ani = new CAnimation(100);
	ani->Add(20012);
	animations->Add(602, ani);
	ani = new CAnimation(100);
	ani->Add(20013);
	animations->Add(603, ani);
	ani = new CAnimation(100);
	ani->Add(20014);
	animations->Add(604, ani);
	ani = new CAnimation(100);
	ani->Add(20015);
	animations->Add(605, ani);
	ani = new CAnimation(100);
	ani->Add(20016);
	animations->Add(606, ani);
	ani = new CAnimation(100);
	ani->Add(20017);
	animations->Add(607, ani);
	ani = new CAnimation(100);
	ani->Add(20018);
	animations->Add(608, ani);
	ani = new CAnimation(100);
	ani->Add(20019);
	animations->Add(609, ani);
	ani = new CAnimation(100);
	ani->Add(20020);
	animations->Add(610, ani);
	ani = new CAnimation(100);
	ani->Add(20021);
	animations->Add(611, ani);
	ani = new CAnimation(100);
	ani->Add(20022);
	animations->Add(612, ani);
	
	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX);
	brick = new CBrick(100.0f, 100.0f);
	goomba = new CGoomba(MARIO_START_X, MARIO_START_Y + 20, MARIO_START_VX);
	CTile* platform_left = new CTile(8.0f, 8.0f + 16 * 10);
	platform_left->setTileid(1);
	platform_left->SetAnimationSet(ani);
	objects.push_back(platform_left);

	CTile* platform_right = new CTile(8.0f + 16.0f * 7, 8.0f+16*10);
	platform_right->setTileid(2);
	platform_right->SetAnimationSet(ani);
	objects.push_back(platform_right);

	
	for (int i = 0;i < 20;i++)
	{
		CTile* platform_middle = new CTile(8.0f+16.0f*(i+1), 8.0f+ 16 * 10);
		platform_middle->setTileid(2);
		platform_middle->SetAnimationSet(ani);
		objects.push_back(platform_middle);
	}
	
	CTile* tree_3;
	tree_3 = new CTile(8.0f + 16.0f * 1, 8.0f + 16 * 9);
	tree_3->setTileid(6);
	tree_3->SetAnimationSet(ani);
	objects.push_back(tree_3);
	tree_3 = new CTile(8.0f + 16.0f * 1, 8.0f + 16 * 8);
	tree_3->setTileid(6);
	tree_3->SetAnimationSet(ani);
	objects.push_back(tree_3);
	tree_3 = new CTile(8.0f + 16.0f * 6, 8.0f + 16 * 8);
	tree_3->setTileid(6);
	tree_3->SetAnimationSet(ani);
	objects.push_back(tree_3);

	CTile* tree_1;
	tree_1 = new CTile(8.0f + 16.0f * 1, 8.0f + 16 * 7);
	tree_1->setTileid(4);
	tree_1->SetAnimationSet(ani);
	objects.push_back(tree_1);
	tree_1 = new CTile(8.0f + 16.0f * 4, 8.0f + 16 * 7);
	tree_1->setTileid(4);
	tree_1->SetAnimationSet(ani);
	objects.push_back(tree_1);
	tree_1 = new CTile(8.0f + 16.0f * 2, 8.0f + 16 * 6);
	tree_1->setTileid(4);
	tree_1->SetAnimationSet(ani);
	objects.push_back(tree_1);
	tree_1 = new CTile(8.0f + 16.0f * 6, 8.0f + 16 * 7);
	tree_1->setTileid(4);
	tree_1->SetAnimationSet(ani);
	objects.push_back(tree_1);

	CTile* tree_2;
	tree_2 = new CTile(8.0f + 16.0f * 3, 8.0f + 16 * 6);
	tree_2->setTileid(5);
	tree_2->SetAnimationSet(ani);
	objects.push_back(tree_2);
	tree_2 = new CTile(8.0f + 16.0f * 5, 8.0f + 16 * 7);
	tree_2->setTileid(5);
	tree_2->SetAnimationSet(ani);
	objects.push_back(tree_2);
	tree_2 = new CTile(8.0f + 16.0f * 7, 8.0f + 16 * 7);
	tree_2->setTileid(5);
	tree_2->SetAnimationSet(ani);
	objects.push_back(tree_2);
	tree_2 = new CTile(8.0f + 16.0f * 8, 8.0f + 16 * 8);
	tree_2->setTileid(5);
	tree_2->SetAnimationSet(ani);
	objects.push_back(tree_2);

	CTile* tree_4;
	tree_4 = new CTile(8.0f + 16.0f * 3, 8.0f + 16 * 7);
	tree_4->setTileid(7);
	tree_4->SetAnimationSet(ani);
	objects.push_back(tree_4);
	tree_4 = new CTile(8.0f + 16.0f * 5, 8.0f + 16 * 8);
	tree_4->setTileid(7);
	tree_4->SetAnimationSet(ani);
	objects.push_back(tree_4);
	tree_4 = new CTile(8.0f + 16.0f * 8, 8.0f + 16 * 9);
	tree_4->setTileid(7);
	tree_4->SetAnimationSet(ani);
	objects.push_back(tree_4);
	
	CTile* tree_5;
	tree_5 = new CTile(8.0f + 16.0f * 2, 8.0f + 16 * 9);
	tree_5->setTileid(8);
	tree_5->SetAnimationSet(ani);
	objects.push_back(tree_5);
	tree_5 = new CTile(8.0f + 16.0f * 3, 8.0f + 16 * 9);
	tree_5->setTileid(8);
	tree_5->SetAnimationSet(ani);
	objects.push_back(tree_5);
	tree_5 = new CTile(8.0f + 16.0f * 4, 8.0f + 16 * 9);
	tree_5->setTileid(8);
	tree_5->SetAnimationSet(ani);
	objects.push_back(tree_5);
	tree_5 = new CTile(8.0f + 16.0f * 4, 8.0f + 16 * 8);
	tree_5->setTileid(8);
	tree_5->SetAnimationSet(ani);
	objects.push_back(tree_5);
	tree_5 = new CTile(8.0f + 16.0f * 7, 8.0f + 16 * 9);
	tree_5->setTileid(8);
	tree_5->SetAnimationSet(ani);
	objects.push_back(tree_5);

	CTile* tree_6;
	tree_6 = new CTile(8.0f + 16.0f * 2, 8.0f + 16 * 8);
	tree_6->setTileid(9);
	tree_6->SetAnimationSet(ani);
	objects.push_back(tree_6);
	tree_6 = new CTile(8.0f + 16.0f * 7, 8.0f + 16 * 8);
	tree_6->setTileid(9);
	tree_6->SetAnimationSet(ani);
	objects.push_back(tree_6);
	tree_6 = new CTile(8.0f + 16.0f * 5, 8.0f + 16 * 9);
	tree_6->setTileid(9);
	tree_6->SetAnimationSet(ani);
	objects.push_back(tree_6);

	CTile* tree_7;
	tree_7 = new CTile(8.0f + 16.0f * 2, 8.0f + 16 * 7);
	tree_7->setTileid(10);
	tree_7->SetAnimationSet(ani);
	objects.push_back(tree_7);
	tree_7 = new CTile(8.0f + 16.0f * 3, 8.0f + 16 * 8);
	tree_7->setTileid(10);
	tree_7->SetAnimationSet(ani);
	objects.push_back(tree_7);
	tree_7 = new CTile(8.0f + 16.0f * 6, 8.0f + 16 * 9);
	tree_7->setTileid(10);
	tree_7->SetAnimationSet(ani);
	objects.push_back(tree_7);
	
	for (int i = 0;i < 3;i++)
	{
		CTile* bush = new CTile(8.0f + 16.0f * 9 +  16.0f * (i + 1), 8.0f + 16 * 9);
		bush->setTileid(11);
		bush->SetAnimationSet(ani);
		objects.push_back(bush);
	}
	
	for (int i = 0;i < 2;i++)
	{
		CTile* question = new CTile(8.0f + 16.0f * 10 + 16.0f * (i + 1), 8.0f + 16 * 6);
		question->setTileid(12);
		question->SetAnimationSet(ani);
		objects.push_back(question);
	}

	for (int i = 0;i < 2;i++)
	{
		CTile* question = new CTile(8.0f + 16.0f * 13 + 16.0f * (i + 1), 8.0f + 16 * 3);
		question->setTileid(12);
		question->SetAnimationSet(ani);
		objects.push_back(question);
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	goomba->Update(dt);
	
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		brick->Render();
		mario->Render();
		goomba->Render();
		for (int i = 0;i < 54;i++) {
			objects[i]->Render();
		}
		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame *game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}