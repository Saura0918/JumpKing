#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <windows.h>
#include "audio.h"
#include "gamelib.h"
#include "King.h"
#include "Map.h"
namespace game_framework {
	King::King()
	{
		
	}
	bool King::isFalled = false;
	int King::GetX1()
	{
		return x;
	}

	int King::GetY1()
	{
		return y;
	}

	int King::GetX2()
	{
		return x + rightStand.Width();
	}

	int King::GetY2()
	{
		return y + rightStand.Height();
	}

	void King::Initialize()
	{
		const int X_POS = 457;
		const int Y_POS = 603;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		isCharged = isCharging  = isFalling = false;
		isJumped  = isJumpUP = isJumping = false;
		isFallHit = isFallHited = isJumpHit = isJumpHited = false;
		kingState = KING_STATE_RIGHT_STAND;					// 初始狀態
		jumpTime = startJumpTime = chargeTime = startChargeTime = 0;
		FaceTo = KING_FACE_TO_RIGHT;
		Velocity = FallDistance = 0;
		LoadAudio();
	}

	void King::LoadBitmap()
	{
		rightStand.LoadBitmap(IDB_KINGSTAND01, RGB(255, 255, 255));
		leftStand.LoadBitmap(IDB_KINGSTAND02, RGB(255, 255, 255));
		rightJump.LoadBitmap(IDB_KINGRIGHTJUMP, RGB(255, 255, 255));
		leftJump.LoadBitmap(IDB_KINGLEFTJUMP, RGB(255, 255, 255));
		rightFall.LoadBitmap(IDB_KINGRIGHTFALL, RGB(255, 255, 255));
		leftFall.LoadBitmap(IDB_KINGLEFTFALL, RGB(255, 255, 255));
		rightHit.LoadBitmap(IDB_KINGRIGHTHIT, RGB(255, 255, 255));
		leftHit.LoadBitmap(IDB_KINGLEFTHIT, RGB(255, 255, 255));
		kingCharge.LoadBitmap(IDB_KINGCHARGE, RGB(255, 255, 255));
		rightWalk.AddBitmap(IDB_KingRightWalk01, RGB(255, 255, 255));
		rightWalk.AddBitmap(IDB_KingRightWalk02, RGB(255, 255, 255));
		rightWalk.AddBitmap(IDB_KingRightWalk03, RGB(255, 255, 255));
		rightWalk.AddBitmap(IDB_KingRightWalk02, RGB(255, 255, 255));
		leftWalk.AddBitmap(IDB_KingLeftWalk01, RGB(255, 255, 255));
		leftWalk.AddBitmap(IDB_KingLeftWalk02, RGB(255, 255, 255));
		leftWalk.AddBitmap(IDB_KingLeftWalk03, RGB(255, 255, 255));
		leftWalk.AddBitmap(IDB_KingLeftWalk02, RGB(255, 255, 255));
		jumpParticle[0].LoadBitmap(IDB_JumpParticle01, RGB(255, 255, 255));
		jumpParticle[1].LoadBitmap(IDB_JumpParticle02, RGB(255, 255, 255));
		jumpParticle[2].LoadBitmap(IDB_JumpParticle03, RGB(255, 255, 255));
		jumpParticle[3].LoadBitmap(IDB_JumpParticle04, RGB(255, 255, 255));
		jumpParticle[4].LoadBitmap(IDB_JumpParticle05, RGB(255, 255, 255));
		rightWalk.SetDelayCount(5);					// 設定動畫速度
		leftWalk.SetDelayCount(5);					// 設定動畫速度
	}

	void King::LoadAudio()
	{
		CAudio::Instance()->Load(KING_AUDIO_JUMP, "Sounds\\King\\king_jump.wav");	// 載入聲音
		CAudio::Instance()->Load(KING_AUDIO_HIT, "Sounds\\King\\king_hit.wav");		// 載入聲音
		CAudio::Instance()->Load(KING_AUDIO_LAND, "Sounds\\King\\king_land.wav");	// 載入聲音
		
	}

	void King::OnMove()
	{
		int FunctionIn;
		isFalling = Map::isFalling(this);
		if (!isFalling) isFallHit = isJumpHit = false;
		if (!(isCharging || isJumping || isFalling)) {  // 無法在蓄力跳躍掉落時左右移動
			Moving();									// 沒在移動時也會進入，以更換為站立圖
			isCharged = false;
			FunctionIn = 1;
		}
		else if (isCharging && !isJumping &&			// 僅能在非跳躍 非掉落時進入蓄力
				!isFalling) {							// 跳躍鍵沒放開無法再次進入蓄力
			Charging();
			FunctionIn = 2;
		}
		else if (isJumping && !isJumpHit) {				// 跳躍
			Jumping();
			FunctionIn = 3;
		}
		else if (isJumpHit) {							// 跳躍反彈
			JumpHit();
			FunctionIn = 4;
		}
		else if (isFallHit)								// 降落反彈
		{
			FallHit();
			FunctionIn = 5;
		}
		else if (isFalling) {							// 未在跳躍則為掉落
			Falling();
			FunctionIn = 6;
		} 
		TRACE(_T("TRACE FunctionIn, isJumpHit. %d, %d.\n"), FunctionIn, isFalling);

	}

	void King::OnShow()
	{
		if (kingState == KING_STATE_RIGHT_STAND) {
			rightStand.SetTopLeft(x, y);
			rightStand.ShowBitmap();
		}
		else if (kingState == KING_STATE_LEFT_STAND) {
			leftStand.SetTopLeft(x, y);
			leftStand.ShowBitmap();
		}
		else if (kingState == KING_STATE_RIGHT_WALK) {
			rightWalk.SetTopLeft(x, y);
			rightWalk.OnShow();
		}
		else if (kingState == KING_STATE_LEFT_WALK) {
			leftWalk.SetTopLeft(x, y);
			leftWalk.OnShow();
		}
		else if (kingState == KING_STATE_CHARGE) {
			kingCharge.SetTopLeft(x, y);
			kingCharge.ShowBitmap();
		}
		else if (kingState == KING_STATE_RIGHT_JUMP)
		{
			rightJump.SetTopLeft(x, y);
			rightJump.ShowBitmap();
		}
		else if (kingState == KING_STATE_LEFT_JUMP)
		{
			leftJump.SetTopLeft(x, y);
			leftJump.ShowBitmap();
		}
		else if (kingState == KING_STATE_RIGHT_FALL)
		{
			rightFall.SetTopLeft(x, y);
			rightFall.ShowBitmap();
		}
		else if (kingState == KING_STATE_LEFT_FALL)
		{
			leftFall.SetTopLeft(x, y);
			leftFall.ShowBitmap();
		}
		else if (kingState == KING_STATE_RIGHT_HIT)
		{
			rightHit.SetTopLeft(x, y);
			rightHit.ShowBitmap();
		}
		else if (kingState == KING_STATE_LEFT_HIT)
		{
			leftHit.SetTopLeft(x, y);
			leftHit.ShowBitmap();
		}
		if (isJumping) {
			int No = int(VelocityRate * 5 - 1);
			jumpParticle[No].ShowBitmap();
		}
			
	}

	void King::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void King::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void King::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void King::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void King::SetCharging(bool flag)
	{
		if (!isJumping && !isFalling) isCharging = flag;	// 未在跳躍或掉落才能蓄力
		if (chargeTime<1000 && !flag && !isFalling) {		// 如果已經跳躍則不跳躍 掉落中無法跳躍
			isJumping = true;
		}
	}

	void King::SetFalling(bool flag)
	{
		isFalling = flag;
	}


	void King::SetXY(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	void King::SetIsFalled(bool flag)
	{
		isFalled = flag;
	}


	void King::Moving()
	{
		MovingToStand();
		if (isMovingLeft && isMovingRight) {		// 左右鍵同時觸發時不移動
			if (kingState == KING_STATE_LEFT_WALK)
				kingState = KING_STATE_LEFT_STAND;
			if (kingState == KING_STATE_RIGHT_WALK)
				kingState = KING_STATE_RIGHT_STAND;
		}
		else if (isMovingLeft) {					// 向左
			x -= KING_STEP_SIZE;
			kingState = KING_STATE_LEFT_WALK;
			FaceTo = KING_FACE_TO_LEFT;
			leftWalk.OnMove();
		}
		else if (isMovingRight) {					// 向右
			x += KING_STEP_SIZE;
			kingState = KING_STATE_RIGHT_WALK;
			FaceTo = KING_FACE_TO_RIGHT;
			rightWalk.OnMove();
		}
		
		if (Map::HitWall(this))						// 撞牆，回復原狀
		{
			if (isMovingRight) x -= KING_STEP_SIZE;
			if (isMovingLeft) x += KING_STEP_SIZE;
		}
	}

	void King::MovingToStand()
	{
		TRACE(_T("TRACE Before kingState.%d\n"), kingState);
		if (kingState == KING_STATE_RIGHT_WALK ||
			kingState == KING_STATE_RIGHT_FALL ||
			kingState == KING_STATE_RIGHT_HIT) 
		{
			if(kingState != KING_STATE_RIGHT_WALK)
				CAudio::Instance()->Play(KING_AUDIO_LAND, false);			// 撥放 WAVE
			kingState = KING_STATE_RIGHT_STAND;
		}	
		else if (kingState == KING_STATE_LEFT_WALK ||
				 kingState == KING_STATE_LEFT_FALL ||
				 kingState == KING_STATE_LEFT_HIT) 
		{
			if (kingState != KING_STATE_LEFT_WALK)
				CAudio::Instance()->Play(KING_AUDIO_LAND, false);			// 撥放 WAVE
			kingState = KING_STATE_LEFT_STAND;
		}
	}

	void King::Charging()
	{
		if (!isCharged) {							// 紀錄開始蓄力時間
			startChargeTime = GetTickCount();
			kingState = KING_STATE_CHARGE;
			isCharged = true;
			chargeTime = 0;
		}else {	
			chargeTime = GetTickCount() - startChargeTime;							// 紀錄經過時間
			if (!isMovingRight && isMovingLeft) FaceTo = KING_FACE_TO_LEFT;			// 調整跳躍方向
			else if (isMovingRight && !isMovingLeft) FaceTo = KING_FACE_TO_RIGHT;
		}
		if (chargeTime >= 1000) {					// 蓄力超過 1 秒，自動跳躍
			isJumping = true;
			Jumping(1000);
		}
		
	}
	
	void King::Jumping(DWORD jumpPower)
	{
		DWORD jumpElapsedTime = GetTickCount() - startJumpTime;
		jumpElapsedTime *= 3;
		if (!isJumped) {										// 紀錄開始跳躍時間
			CAudio::Instance()->Play(KING_AUDIO_JUMP, false);	// 撥放跳躍音效
			for (int i = 0; i < 5;i++)							// 跳躍特效座標
				jumpParticle[i].SetTopLeft(x, y);					
			startJumpTime = GetTickCount();
			jumpTime = jumpPower *2;
			isJumped = true;
			isCharging = isCharged = false;
			Velocity = KING_JUMP_MAX_Y;
			VelocityRate = 0.2;
			TRACE(_T("TRACE jumpPower.%d\n"), jumpPower);
			if (isMovingLeft && !isMovingRight) {				// 設定跳躍方向
				JumpDirection = KING_FACE_TO_LEFT;
				kingState = KING_STATE_LEFT_JUMP;
			}
			else if (!isMovingLeft && isMovingRight) {
				JumpDirection = KING_FACE_TO_RIGHT;
				kingState = KING_STATE_RIGHT_JUMP;
			}
			else {												// 向上跳
				JumpDirection = KING_FACE_TO_UP;
				if (FaceTo == KING_FACE_TO_RIGHT)				// 設定跳躍圖
					kingState = KING_STATE_RIGHT_JUMP;
				else
					kingState = KING_STATE_LEFT_JUMP;
			}
		}
		else if (jumpElapsedTime < jumpTime) {	// 跳躍中
			if (JumpDirection == KING_FACE_TO_LEFT) {
				x -= KING_JUMP_SIZE_X;
				y -= Velocity;
			}
			else if (JumpDirection == KING_FACE_TO_RIGHT) {
				x += KING_JUMP_SIZE_X;
				y -= Velocity;
			}
			else if (JumpDirection == KING_FACE_TO_UP)
				y -= Velocity;
			if (jumpElapsedTime > jumpTime * VelocityRate)		// 速度設定，每經過0.2就改變上升速度
			{
				VelocityRate += 0.2;
				Velocity -= 2;
			}
			
			Map::ChangeMap(this);
			if (Map::HitHead(this))
			{
				if (JumpDirection == KING_FACE_TO_LEFT) {
					x += 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				else if (JumpDirection == KING_FACE_TO_RIGHT) {
					x -= 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				else if (JumpDirection == KING_FACE_TO_UP)
					y += Velocity;
				isJumping = false;								// 結束跳躍
				isJumped = false;
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				if (JumpDirection == KING_FACE_TO_UP)
					isJumpUP = true;
				TRACE(_T("TRACE Jumping HitHead.\n"));
				Falling();
			}
			else if (Map::HitSide(this)) {
				if (JumpDirection == KING_FACE_TO_LEFT) {
					x += 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				else if (JumpDirection == KING_FACE_TO_RIGHT) {
					x -= 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				TRACE(_T("TRACE Jumping HitSide.\n"));
				JumpHit();				// 反彈為高度為剩餘時間
			}
		}
		else {													// 結束跳躍
			isJumping = false;
			isJumped = false;
			if (JumpDirection == KING_FACE_TO_UP)
				isJumpUP = true;
		}
	}

	void King::Jumping() {						// 按鍵釋放
		Jumping(chargeTime);
	}

	void King::JumpHit()						// 跳躍時撞牆
	{
		DWORD jumpElapsedTime = GetTickCount() - startJumpTime;
		jumpElapsedTime *= 3;
		if (!isJumpHit)
		{
			isJumping = false;
			isJumped = false;
			isJumpHit = true;
			FallDistance = 0;
			if (JumpDirection == KING_FACE_TO_LEFT) {				// 反彈方向相反
				HitDirection = KING_FACE_TO_RIGHT;
				kingState = KING_STATE_LEFT_HIT;
			}
			else if (JumpDirection == KING_FACE_TO_RIGHT) {
				HitDirection = KING_FACE_TO_LEFT;
				kingState = KING_STATE_RIGHT_HIT;
			}
		}
		else if (jumpElapsedTime < jumpTime) {						// 反彈高度為剩餘跳躍時間
			if (HitDirection == KING_FACE_TO_LEFT) {
				x += KING_JUMP_SIZE_X;
				y -= Velocity;
			}
			else if (HitDirection == KING_FACE_TO_RIGHT) {
				x -= KING_JUMP_SIZE_X;
				y -= Velocity;
			}

			if (jumpElapsedTime > jumpTime * VelocityRate)		// 速度設定，每經過0.2就改變上升速度
			{
				VelocityRate += 0.2;
				Velocity -= 2;
			}

			Map::ChangeMap(this);
			if (Map::HitHead(this))								// 反彈跳躍頭頂撞牆時
			{
				if (HitDirection == KING_FACE_TO_LEFT) {
					x -= 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				else if (HitDirection == KING_FACE_TO_RIGHT) {
					x += 2 * KING_JUMP_SIZE_X;
					y += Velocity;
				}
				TRACE(_T("TRACE JumpHit HitHead.\n"));
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				isJumpHit = false;								// 直接落下
				Falling();
			}
			else if (Map::HitSide(this)) {						// 反彈跳躍碰撞兩側
				if (HitDirection == KING_FACE_TO_LEFT) {
					x -= 2 * KING_JUMP_SIZE_X;
					y += Velocity;
					HitDirection = KING_FACE_TO_RIGHT;			// 往反方向移動
				}
				else if (HitDirection == KING_FACE_TO_RIGHT) {
					x += 2 * KING_JUMP_SIZE_X;
					y += Velocity;
					HitDirection = KING_FACE_TO_LEFT;
				}
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				TRACE(_T("TRACE JumpHit HitSide.\n"));
			}
		}
		else {													// 反彈後開始開始下降
			if (FallDistance == 0) Velocity = 10;				// 初始下降速度
								
			if (HitDirection == KING_FACE_TO_LEFT)
			{
				x += KING_HIT_SIZE_X;
				y += Velocity;
			}
			else if (HitDirection == KING_FACE_TO_RIGHT)
			{
				x -= KING_HIT_SIZE_X;
				y += Velocity;
			}
			if (FallDistance > 400 * VelocityRate)				// 速度設定，每經過80就改變下降速度
			{
				VelocityRate += 0.2;
				if (Velocity <= KING_JUMP_MAX_Y)
				{
					Velocity += 2;
				}
			}
			TRACE(_T("TRACE Velocity. %d.\n"), Velocity);
			FallDistance += Velocity;
			Map::ChangeMap(this);
			if (Map::HitSide(this)) {							// 反彈掉落碰撞兩側
				if (HitDirection == KING_FACE_TO_LEFT) {
					x -= 2 * KING_HIT_SIZE_X;
					y -= Velocity;
					HitDirection = KING_FACE_TO_RIGHT;			// 往反方向移動
				}
				else if (HitDirection == KING_FACE_TO_RIGHT) {
					x += 2 * KING_HIT_SIZE_X;
					y -= Velocity;
					HitDirection = KING_FACE_TO_LEFT;
				}
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				TRACE(_T("TRACE JumpHit-Fall HitSide.\n"));
			}
		}
	}

	void King::Falling()
	{
		if (!isFalled) {
			isFalled = true;
			FallDistance = 0;
			Velocity = 10;
			VelocityRate = 0.2;
			if (isJumpUP)
			{
				if (FaceTo == KING_FACE_TO_RIGHT)				// 設定掉落圖
					kingState = KING_STATE_RIGHT_FALL;
				else
					kingState = KING_STATE_LEFT_FALL;
				FallDirection = KING_FACE_TO_UP;
				isJumpUP = false;
			}
			else if (FaceTo == KING_FACE_TO_LEFT)
			{
				kingState = KING_STATE_LEFT_FALL;
				FallDirection = KING_FACE_TO_LEFT;
			}
			else if (FaceTo == KING_FACE_TO_RIGHT)
			{
				kingState = KING_STATE_RIGHT_FALL;
				FallDirection = KING_FACE_TO_RIGHT;
			}
		}
		else
		{
			if (FallDirection == KING_FACE_TO_UP)
			{
				y += Velocity;
			}
			else if (FallDirection == KING_FACE_TO_LEFT)
			{
				x -= KING_JUMP_SIZE_X;
				y += Velocity;
			}
			else if (FallDirection == KING_FACE_TO_RIGHT)
			{
				x += KING_JUMP_SIZE_X;
				y += Velocity;
			}
			if (FallDistance > 400 * VelocityRate)		// 速度設定，每經過80就改變下降速度
			{
				VelocityRate += 0.2;
				if (Velocity <= 16)						// 最高速度
				{
					Velocity += 2;
				}
			}
			FallDistance += Velocity;
			Map::ChangeMap(this);
			if (Map::HitSide(this))
			{
				isFallHit = true;
				if (FallDirection == KING_FACE_TO_LEFT)
				{
					x += 2 * KING_JUMP_SIZE_X;
					y -= Velocity;
				}
				else if (FallDirection == KING_FACE_TO_RIGHT)
				{
					x -= 2 * KING_JUMP_SIZE_X;
					y -= Velocity;
				}
				CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
				TRACE(_T("TRACE Falling HitSide.\n"));
			}
			
		}
		
	}

	void King::FallHit()
	{
		if (kingState == KING_STATE_LEFT_FALL) {
			kingState = KING_STATE_LEFT_HIT;
			HitDirection = KING_FACE_TO_RIGHT;
		}
		else if (kingState == KING_STATE_RIGHT_FALL) {
			kingState = KING_STATE_RIGHT_HIT;
			HitDirection = KING_FACE_TO_LEFT;
		}

		if (HitDirection == KING_FACE_TO_LEFT)
		{
			x -= KING_HIT_SIZE_X;
			y += Velocity;
		}
		else if (HitDirection == KING_FACE_TO_RIGHT)
		{
			x += KING_HIT_SIZE_X;
			y += Velocity;
		}
		if (FallDistance > 400 * VelocityRate)		// 速度設定，每經過80就改變下降速度
		{
			VelocityRate += 0.2;
			if (Velocity <= KING_JUMP_MAX_Y)						// 最高速度
			{
				Velocity += 2;
			}
		}

		if (Map::HitSide(this)) {
			TRACE(_T("TRACE FallHit HitSide.\n"));
			if (HitDirection == KING_FACE_TO_LEFT) {
				x += 2 * KING_HIT_SIZE_X;
				y -= Velocity;
				HitDirection = KING_FACE_TO_RIGHT;			// 往反方向移動
			}
			else if (HitDirection == KING_FACE_TO_RIGHT) {
				x -= 2 * KING_HIT_SIZE_X;
				y -= Velocity;
				HitDirection = KING_FACE_TO_LEFT;
			}
			CAudio::Instance()->Play(KING_AUDIO_HIT, false);			// 撥放 WAVE
		}
		Map::ChangeMap(this);
	}



}