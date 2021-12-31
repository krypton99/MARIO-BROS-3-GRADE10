#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (!mario->GetIsOnPlatform()) {
			mario->SetVy(MARIO_RACOON_FALLING_SLOW_SPEED);
			mario->SetFallLower(true);
		} else 
		if (!mario->GetIsFly()) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		else if (mario->GetIsFly()) { 
			mario->GetFlyTimeOut()->Start(); 
			/*float x, y;
			mario->GetPosition(x, y);
			mario->SetPosition(x, y - 10);*/
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_D:
		mario->SetState(MARIO_STATE_ATTACK);
		break;
	case DIK_F:
		if (mario->GetHolding() == true) {
			mario->SetHolding(false);
		} else
			mario->SetHolding(true);;
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetState() == MARIO_STATE_FLY) break;
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_F:
		mario->SetHolding(true);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_S) && mario->GetIsFly() ) {
		mario->SetIsFlying(true);
		mario->SetState(MARIO_STATE_FLY);
		
	}
	else 
		if (mario->GetState() != MARIO_STATE_PIPE) {
		mario->SetState(MARIO_STATE_IDLE);
	}
}