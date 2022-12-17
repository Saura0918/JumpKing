#pragma once
#ifndef Map_h
#define Map_h
#include "King.h"
namespace game_framework {
	class Map
	{
	public:
		Map();
		void Initialize();							// �]�w��l��
		void LoadBitmap();							// Ū���a�ϹϤ�
		void LoadAudio();							// Ū�����T��
		void OnShow();								// ��ܦa��
		void OnPlay();								// ���񭵰T
		void SetTopLeft();							// �]�w�a�ϥ��W��
		static void ChangeMap(King *king);			// �ˬd�O�_�󴫦a��
		static bool HitWall(King * king);			// �O�_����
		static bool HitHead(King * king);			// �O�_���D�ɼ��쳻��
		static bool HitSide(King * king);			// �O�_���D�ɼ���ⰼ
		static bool isFalling(King * king);			// �O�_��Ĳ�a��
	protected:
		static short MapEdge[MAX_MAP_COUNT][45][60];			// �a����t
		CMovingBitmap   MapBackground[MAX_MAP_COUNT];			// �a�ϹϤ�
		static int CurrentMap;									// �{�b�a�Ϧ�m
		bool mapChanged;
	};
}
#endif