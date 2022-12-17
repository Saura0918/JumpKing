#pragma once
#ifndef King_h
#define King_h


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////
	class King
	{
	public:
		King();
		int  GetX1();					// 取得左上角 x 座標
		int  GetY1();					// 取得左上角 y 座標
		int  GetX2();					// 取得右下角 x 座標
		int  GetY2();					// 取得右下角 y 座標
		int  GetKingState;				// 取得國王狀態
		void Initialize();				// 設定初始值
		void LoadBitmap();				// 載入圖形
		void LoadAudio();				// 載入音訊
		void OnMove();					// 移動
		void OnShow();					// 將圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetCharging(bool flag);	// 設定是否正在蓄力
		void SetFalling(bool flag);		// 設定是否正在掉落
		void SetXY(int nx, int ny);		// 設定左上角座標
		static void SetIsFalled(bool);  // 設定降落是否已經結束
		void Moving();					// 左右移動
		void MovingToStand();			// 走動至停止切換狀態
		void Charging();				// 跳躍蓄力
		void Jumping(DWORD);			// 跳躍，超過一秒自動跳躍
		void Jumping();					// 跳躍，按鍵釋放跳躍
		void JumpHit();			// 跳躍時撞牆
		void Falling();					// 掉落
		void FallHit();					// 掉落時撞牆
		
		
	protected:
		CAnimation	  rightWalk;	// 向右走圖
		CAnimation	  leftWalk;		// 向左走圖
		CMovingBitmap jumpParticle[5];// 跳躍特效
		CMovingBitmap rightStand;   // 向右站圖
		CMovingBitmap leftStand;    // 向左站圖
		CMovingBitmap rightJump;	// 向右跳圖
		CMovingBitmap leftJump;		// 向左跳圖
		CMovingBitmap rightFall;	// 向右掉圖
		CMovingBitmap leftFall;		// 向左掉圖
		CMovingBitmap rightHit;		// 向右掉圖
		CMovingBitmap leftHit;		// 向左掉圖
		CMovingBitmap kingCharge;	// 蓄力圖
		DWORD chargeTime;			// 蓄力時間
		DWORD startChargeTime;		// 蓄力開始時間
		DWORD jumpTime;				// 跳躍時間
		DWORD startJumpTime;		// 跳躍開始時間
		int x, y;					// 左上角座標
		int kingState;				// 國王狀態，走或站
		int jumpDistance;			// 跳躍距離
		int FaceTo;					// 面向左邊或右邊
		int JumpDirection;			// 跳躍時方向
		int FallDirection;			// 掉落方向
		int FallDistance;			// 下降距離
		int HitDirection;			// 反彈時方向
		int Velocity;				// 上升掉落時，初始速度
		int jumpParticleNo;			// 跳躍特效第幾張圖
		double VelocityRate;		// 上升速度變更
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool isCharged;				// 是否開始蓄力
		bool isCharging;			// 是否正在蓄力
		bool isJumped;				// 按下開始跳躍
		bool isJumping;				// 是否正在跳躍狀態
		static bool isFalled;		// 是否開始掉落
		bool isFalling;				// 是否正在掉落
		bool isJumpUP;				// 是否原地向上跳
		bool isJumpHit;				// 是否跳躍反彈
		bool isJumpHited;			// 開始跳躍反彈
		bool isFallHit;				// 是否撞牆反彈
		bool isFallHited;			// 開始撞牆反彈
	};
}

#endif // !King_h