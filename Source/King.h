#pragma once
#ifndef King_h
#define King_h


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////
	class King
	{
	public:
		King();
		int  GetX1();					// ���o���W�� x �y��
		int  GetY1();					// ���o���W�� y �y��
		int  GetX2();					// ���o�k�U�� x �y��
		int  GetY2();					// ���o�k�U�� y �y��
		int  GetKingState;				// ���o������A
		void Initialize();				// �]�w��l��
		void LoadBitmap();				// ���J�ϧ�
		void LoadAudio();				// ���J���T
		void OnMove();					// ����
		void OnShow();					// �N�ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetCharging(bool flag);	// �]�w�O�_���b�W�O
		void SetFalling(bool flag);		// �]�w�O�_���b����
		void SetXY(int nx, int ny);		// �]�w���W���y��
		static void SetIsFalled(bool);  // �]�w�����O�_�w�g����
		void Moving();					// ���k����
		void MovingToStand();			// ���ʦܰ���������A
		void Charging();				// ���D�W�O
		void Jumping(DWORD);			// ���D�A�W�L�@��۰ʸ��D
		void Jumping();					// ���D�A����������D
		void JumpHit();			// ���D�ɼ���
		void Falling();					// ����
		void FallHit();					// �����ɼ���
		
		
	protected:
		CAnimation	  rightWalk;	// �V�k����
		CAnimation	  leftWalk;		// �V������
		CMovingBitmap jumpParticle[5];// ���D�S��
		CMovingBitmap rightStand;   // �V�k����
		CMovingBitmap leftStand;    // �V������
		CMovingBitmap rightJump;	// �V�k����
		CMovingBitmap leftJump;		// �V������
		CMovingBitmap rightFall;	// �V�k����
		CMovingBitmap leftFall;		// �V������
		CMovingBitmap rightHit;		// �V�k����
		CMovingBitmap leftHit;		// �V������
		CMovingBitmap kingCharge;	// �W�O��
		DWORD chargeTime;			// �W�O�ɶ�
		DWORD startChargeTime;		// �W�O�}�l�ɶ�
		DWORD jumpTime;				// ���D�ɶ�
		DWORD startJumpTime;		// ���D�}�l�ɶ�
		int x, y;					// ���W���y��
		int kingState;				// ������A�A���ί�
		int jumpDistance;			// ���D�Z��
		int FaceTo;					// ���V����Υk��
		int JumpDirection;			// ���D�ɤ�V
		int FallDirection;			// ������V
		int FallDistance;			// �U���Z��
		int HitDirection;			// �ϼu�ɤ�V
		int Velocity;				// �W�ɱ����ɡA��l�t��
		int jumpParticleNo;			// ���D�S�ĲĴX�i��
		double VelocityRate;		// �W�ɳt���ܧ�
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool isCharged;				// �O�_�}�l�W�O
		bool isCharging;			// �O�_���b�W�O
		bool isJumped;				// ���U�}�l���D
		bool isJumping;				// �O�_���b���D���A
		static bool isFalled;		// �O�_�}�l����
		bool isFalling;				// �O�_���b����
		bool isJumpUP;				// �O�_��a�V�W��
		bool isJumpHit;				// �O�_���D�ϼu
		bool isJumpHited;			// �}�l���D�ϼu
		bool isFallHit;				// �O�_����ϼu
		bool isFallHited;			// �}�l����ϼu
	};
}

#endif // !King_h