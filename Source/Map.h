#pragma once
#ifndef Map_h
#define Map_h
#include "King.h"
namespace game_framework {
	class Map
	{
	public:
		Map();
		void Initialize();							// 設定初始值
		void LoadBitmap();							// 讀取地圖圖片
		void LoadAudio();							// 讀取音訊檔
		void OnShow();								// 顯示地圖
		void OnPlay();								// 播放音訊
		void SetTopLeft();							// 設定地圖左上角
		static void ChangeMap(King *king);			// 檢查是否更換地圖
		static bool HitWall(King * king);			// 是否撞牆
		static bool HitHead(King * king);			// 是否跳躍時撞到頂部
		static bool HitSide(King * king);			// 是否跳躍時撞到兩側
		static bool isFalling(King * king);			// 是否接觸地面
	protected:
		static short MapEdge[MAX_MAP_COUNT][45][60];			// 地圖邊緣
		CMovingBitmap   MapBackground[MAX_MAP_COUNT];			// 地圖圖片
		static int CurrentMap;									// 現在地圖位置
		bool mapChanged;
	};
}
#endif