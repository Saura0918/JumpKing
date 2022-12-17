#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "audio.h"
#include "gamelib.h"
#include "Map.h"
namespace game_framework {

	short Map::MapEdge[MAX_MAP_COUNT][45][60];
	int Map::CurrentMap = 0;

	Map::Map()
	{

	}
	void Map::Initialize()
	{
		mapChanged = true;
		fstream fileIn;
		string inputStr, tempStr;						// Ū���ɮר�a�ϰ}�C
		int MapCount = 0, readLine = 0, readChar = 0;
		fileIn.open("Map.txt", ios::in);
		if (!fileIn) exit(0);
		while (fileIn >> inputStr)
		{
			if (inputStr[0] == '-') {					// ���Φۦ�
				MapCount++;
				readLine = 0;
				continue;
			}
			stringstream ssin(inputStr);
			readChar = 0;
			while (getline(ssin, tempStr, ',')) {
				MapEdge[MapCount][readLine][readChar] = stoi(tempStr);
				readChar++;
			}
			readLine++;
		}
		LoadAudio();

	}

	void Map::LoadBitmap()
	{
		MapBackground[0].LoadBitmap(IDB_BG01);
		MapBackground[1].LoadBitmap(IDB_BG02);
		MapBackground[2].LoadBitmap(IDB_BG03);
		SetTopLeft();
	}

	void Map::LoadAudio()
	{
		CAudio::Instance()->Load(MAP_AUDIO_MAP01, "Sounds\\Map\\Nature_bg.wav");	// ���J�n��
		//CAudio::Instance()->Load(MAP_AUDIO_MAP01, "Sounds\\King\\king_jump.wav");	// ���J�n��
	}

	void Map::OnShow()
	{
		OnPlay();
		MapBackground[CurrentMap].ShowBitmap();
	}

	void Map::OnPlay()
	{
		if (mapChanged) {
			CAudio::Instance()->Play(MAP_AUDIO_MAP01, true);			// ���� WAVE
			mapChanged = false;
		}
			
	}

	void Map::SetTopLeft()
	{
		for (int i = 0; i < MAX_MAP_COUNT; i++)
			MapBackground[i].SetTopLeft(0, 0);
	}

	void Map::ChangeMap(King *king)
	{
		int kingX1 = king->GetX1();
		int kingY1 = king->GetY1();
		int kingY2 = king->GetY2();
		if (kingY1 + 20 < 0) {					// �H���W�t�W�L�a����ɡA����
			CurrentMap++;
			if (CurrentMap >= MAX_MAP_COUNT)	// �C������
			{
				CurrentMap = 0;
				king->SetXY(457,603);
			}
			else
			{
				king->SetXY(kingX1, 710);
			}
			
		}
		else if (kingY1 > SIZE_Y)				// �H���U�t�W�L�a�����
		{
			CurrentMap--;
			king->SetXY(kingX1, -10);	
		}
	}

	bool Map::HitWall(King * king)
	{
		int kingX1 = king->GetX1();
		int kingX2 = king->GetX2();
		int kingY1 = king->GetY1();
		int kingY2 = king->GetY2();
		kingX1 = (kingX1 - (kingX1 % 16)) / 16;
		kingX2 = (kingX2 - (kingX2 % 16)) / 16;
		kingY1 = (kingY1 - (kingY1 % 16)) / 16;
		kingY2 = (kingY2 - (kingY2 % 16)) / 16 - 1;
		//TRACE(_T("TRACE X1, Y1.%d, %d\n"), kingX1, kingY1);
		if (kingX1 < 0 || kingX2 < 0 || kingY1 < 0)
		{
			return false;
		}
		if (MapEdge[CurrentMap][kingY1][kingX1]) return true;
		if (MapEdge[CurrentMap][kingY2][kingX1]) return true;
		if (MapEdge[CurrentMap][kingY1][kingX2]) return true;
		if (MapEdge[CurrentMap][kingY2][kingX2]) return true;
		return false;
	}

	bool Map::HitHead(King * king)
	{
		int kingX1 = king->GetX1();
		int kingX2 = king->GetX2();
		int kingY1 = king->GetY1();
		int kingX1s = kingX1 + 6;						// ���Y����Ӿa�񨤸����I
		int kingX2s = kingX2 - 6;
		kingX1 = (kingX1 - (kingX1 % 16)) / 16;
		kingX2 = (kingX2 - (kingX2 % 16)) / 16;
		kingY1 = (kingY1 - (kingY1 % 16)) / 16;
		kingX1s = (kingX1s - (kingX1s % 16)) / 16;
		kingX2s = (kingX2s - (kingX2s % 16)) / 16;
		if (kingY1 < 0)									// �W�L��� ���a��
		{
			return false;
		}
		if (MapEdge[CurrentMap][kingY1][kingX1] &&		// �Y���a�񨤸�������I�Ҭ�1�A�h���Y���I��
			MapEdge[CurrentMap][kingY1][kingX1s]) return true;
		if (MapEdge[CurrentMap][kingY1][kingX2] && 
			MapEdge[CurrentMap][kingY1][kingX2s]) return true;
		return false;
	}

	bool Map::HitSide(King * king)
	{
		int kingX1 = king->GetX1();
		int kingX2 = king->GetX2();
		int kingY1 = king->GetY1();
		int kingY2 = king->GetY2();
		int kingY1s = kingY1 + 10;
		int kingY2s = kingY2 - 10;
		kingX1 = (kingX1 - (kingX1 % 16)) / 16;
		kingX2 = (kingX2 - (kingX2 % 16)) / 16;
		kingY1 = (kingY1 - (kingY1 % 16)) / 16;
		kingY2 = (kingY2 - (kingY2 % 16)) / 16;
		kingY1s = (kingY1s - (kingY1s % 16)) / 16;
		kingY2s = (kingY2s - (kingY2s % 16)) / 16;
		
		if (kingY1 >= 0 && kingY2 <= 44) {
			if (MapEdge[CurrentMap][kingY1][kingX1] &&
				MapEdge[CurrentMap][kingY1s][kingX1])
			{
				TRACE(_T("TRACE MapEdge TopLeft %d, %d.\n"), kingX1, kingY1);
				return true;
			}
			if (MapEdge[CurrentMap][kingY1][kingX2] &&
				MapEdge[CurrentMap][kingY1s][kingX2])
			{
				TRACE(_T("TRACE MapEdge TopRight %d, %d.\n"), kingX2, kingY1);
				return true;
			}
		}
		if (kingY2 >= 0 && kingY2 <= 44)
		{
			if (MapEdge[CurrentMap][kingY2][kingX1] &&
				MapEdge[CurrentMap][kingY2s][kingX1])
			{
				TRACE(_T("TRACE MapEdge DownLeft %d, %d.\n"), kingX1, kingY2);
				return true;
			}
			if (MapEdge[CurrentMap][kingY2][kingX2] &&
				MapEdge[CurrentMap][kingY2s][kingX2])
			{
				TRACE(_T("TRACE MapEdge DownRight %d, %d.\n"), kingX2, kingY2);
				return true;
			}
		}
		return false;
	}

	bool Map::isFalling(King * king)
	{
		int kingX1 = king->GetX1() + 6;					// ���oX1��m�A�èϧP�w�I�ܤp
		int kingX2 = king->GetX2() - 6;					// ���oX2��m�A�èϧP�w�I�ܤp
		int kingY2 = king->GetY2() + 6;					// ���oY2��m�A+1�H�P�w�}�U�O�_���a
		
		kingX1 = (kingX1 - (kingX1 % 16)) / 16;			// �}�C�j�p�P��ڦa�Ϯy���ഫ
		kingX2 = (kingX2 - (kingX2 % 16)) / 16;
		kingY2 = (kingY2 - (kingY2 % 16)) / 16;
		if (kingY2 > 44) return true;					// �W�L�}�C�d��A���b������
		if (MapEdge[CurrentMap][kingY2][kingX1]) {		// �ˬd�}�U�O�_���a��
			King::SetIsFalled(false);
			return false;
		}
		if (MapEdge[CurrentMap][kingY2][kingX2]) {
			King::SetIsFalled(false);
			return false;
		}
		return true;
	}


	
}