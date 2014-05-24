/*

	�O�������̊֐��w�b�_�ł��B

*/



#pragma once

#include "NatsumeSystem.h"

#include "NatsumeManager.h"

#include "NatsumeSound.h"

#include "Natsume2DGraphics.h"

#include "NatsumeInput.h"

#include "Error.h"




namespace NatsumeGrass
{
	namespace External
	{
		//�l�p�`�̍\���̏��
		struct CustomSquare{
			CustomSquare(float _x = 0.0f,float _y = 0.0f,float _Height = 0.0f,float _Width = 0.0f);		//�R���X�g���N�^
			float x,y;			//����̒��_���
			float Height,Width;	//x,y����E���ɂ����Ă̑傫��
		};

		// *********************************
		// �V�[�P���X
		// *********************************
		namespace Sequence
		{
			//�������O�ɌĂԏ����A�E�B���h�E�̃T�C�Y����ݒ�ł��܂��B
			HRESULT SetScreenInfo(INT _ScreenHeight,INT _ScreenWidth,LPCSTR _TitleName,BOOL _WindowMode);
			
			//�������֐�
			HRESULT Initialize(HINSTANCE _hInstance);
			
			//�I���֐�
			HRESULT End();

		};

		// *********************************
		// �`��
		// *********************************
		namespace Graphic
		{
			//��ʂ̃N���A�A����ʏ��������s���Ă����֐�
			HRESULT BeginRender();

			//��ʂ̍X�V�A�X�N���[���t�B���b�v�����s���֐�
			HRESULT EndRender();


			//�{�b�N�X�̍쐬
			HRESULT CreateBox(CustomSquare _Obj,DWORD _Color = 0xffaaaaaa);

			//�e�N�X�`���̓ǂݍ���
			INT LoadTexture(LPCSTR _GraphicLocation);

			//�e�N�X�`���̕\��
			HRESULT DrawTexture(INT _TextureKey,CustomSquare _Square,BOOL _AlphaUse = false,BYTE _AlphaColor = 0xff);
			HRESULT DrawTexture(INT _TextureKey,float _x,float _y,float _Height,float _Width,BOOL _AlphaUse = false,BYTE _AlphaColor = 0xff);


		};

		// *********************************
		// ����
		// *********************************
		namespace Input
		{
			//Input�n�̃A�b�v�f�[�g�֐��A���t���[���s���Ă��������B
			HRESULT UpdateAcquire();

			//�L�[�̓��̓`�F�b�N�֐� _DIK_��Define��n���Ă��������B
			BOOL KeyCheck(BYTE _DIK_);

			//�L�[�̃t���[���`�F�b�N�֐� _DIK_��Define��n���Ă��������B
			//�A�b�v�f�[�g���s�����ۂɍX�V�����̂�
			//���͂̍X�V����ʍX�V��葽����΋@�\���܂���B
			UINT KeyCheckFrame(BYTE _DIK_);

			//�W���C�p�b�h�̃L�[���͂̃`�F�b�N������ۂɎg���Ă��������B
			enum ENUM_JOYPAD
			{
				JOYKEY_BUTTON	,//�{�^��
				JOYKEY_POV		,//POV�L�[
				JOYSTICK_LEFT_X	,//���X�e�B�b�NX
				JOYSTICK_LEFT_Y	,//���X�e�B�b�NY
				JOYSTICK_RIGHT_X,//�E�X�e�B�b�NX
				JOYSTICK_RIGHT_Y,//�E�X�e�B�b�NY
			};

			//�W���C�p�b�h�̓��̓`�F�b�N
			BOOL JoyPad_Check(ENUM_JOYPAD _Key,BYTE _Num);
			
			//�W���C�p�b�h�̃t���[���`�F�b�N
			UINT JoyPad_Frame(ENUM_JOYPAD _Key,BYTE _Num);

			//�W���C�p�b�h�̃X�e�B�b�N�`�F�b�N
			LONG JoyPad_Stick(ENUM_JOYPAD _Stick);


		};
		
		// *********************************
		// �T�E���h
		// *********************************
		namespace Sound
		{
			//�T�E���h�̓ǂݍ��݁A�Ԃ�l��Key�ɓn���Ă��������B
			UINT LoadSound(TCHAR *_FileName);
			
			//�Đ�
			void Play(UINT _Key);

			//��~
			void Stop(UINT _Key);
		};

		// *********************************
		// �v���V�[�W��
		// *********************************
		namespace Proc
		{
			LRESULT MsgProc(HWND _hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

		};



	};

	namespace System
	{	
		extern INT ScreenSizeWidth;
		extern INT ScreenSizeHeight;
		extern LPCSTR AppTitleName;
		extern BOOL WindowMode;
	};
};




