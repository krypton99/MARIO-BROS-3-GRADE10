#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float vx):CGameObject(x, y)
{
	this->vx = vx;
};

void CMario::Update(DWORD dt)
{
	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx>0) ani = CAnimations::GetInstance()->Get(500);
	else ani = CAnimations::GetInstance()->Get(501);

	ani->Render(x, y);
}

CGoomba::CGoomba(float x, float y, float vx) :CGameObject(x, y)
{
	this->vx = vx;
};

void CGoomba::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - GOOMBA_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - GOOMBA_WIDTH)
		{
			x = (float)(BackBufferWidth - GOOMBA_WIDTH);
		}
	}
}

void CGoomba::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(502);
	else ani = CAnimations::GetInstance()->Get(502);

	ani->Render(x, y);
}

void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);

}
void CTile::Render() {
	switch (tileid)
	{
	case ID_PLATFORM_LEFT:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(601);
		ani->Render(x, y);
	}
	break;
	case ID_PLATFORM_MIDDLE:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(603);
		ani->Render(x, y);
	}
	break;
	case ID_PLATFORM_RIGHT:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(602);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_1:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(604);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_2:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(605);
		ani->Render(x, y);
	}
	break;
	
	case ID_TREE_3:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(606);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_4:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(607);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_5:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(608);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_6:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(609);
		ani->Render(x, y);
	}
	break;
	case ID_TREE_7:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(610);
		ani->Render(x, y);
	}
	break;
	case ID_BUSH:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(611);
		ani->Render(x, y);
	}
	break;
	case ID_BRICK_QUESTION:
	{
		LPANIMATION ani = CAnimations::GetInstance()->Get(612);
		ani->Render(x, y);
	}
	break;
}
}
