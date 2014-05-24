/*

	���͏����N���X�w�b�_

*/


#pragma once


#include "NatsumeSystem.h"

#include "NatsumeManager.h"



namespace NatsumeGrass
{
	namespace System
	{
		//LPDIRECTINPUTDEVICE8 LpJoypad;
		class InputClass : public Natsume_Core_Single<InputClass>
		{
		public:
			friend class Natsume_Core_Single<InputClass>;

			//���͎�t���m�F���A�t�H�[�J�X���O��Ă��瓙��
			//���͎�t���ĊJ����
			HRESULT Acquire();


			inline BOOL KeyCheck(BYTE _DIK_){
				if(Key[_DIK_] & 0x80){return true;}
				return false;
			}
			inline UINT KeyCheckFrame(BYTE _DIK_){
				return KeyFrame[_DIK_];
			}

			inline BOOL Joy_Button_KeyCheck(BYTE _Num){
				if(_Num > 32){return FALSE;}
				return (JoyPadData.rgbButtons[_Num] & 0x80);
			}
			inline UINT Joy_Button_Frame(BYTE _Num){
				if(_Num >= 32){return -1;}
				return JoyPadButtonFrame[_Num];
			}
			inline BOOL Joy_Pov_KeyCheck(BYTE _Num){
				if(_Num >= 4){return FALSE;}
				return (JoyPadData.rgdwPOV[_Num] & 0x80);
			}
			inline UINT Joy_Pov_Frame(BYTE _Num){
				if(_Num >= 4){return FALSE;}
				return JoyPadPovKeyFrame[_Num];
			}

			inline LONG Joy_StickLeftX(){
				return (JoyPadData.lX);
			}
			inline LONG Joy_StickLeftY(){
				return (JoyPadData.lY);
			}
			inline LONG Joy_StickRightX(){
				return (JoyPadData.lZ);
			}
			inline LONG Joy_StickRightY(){
				return (JoyPadData.lRz);
			}

		private:
			InputClass();
			~InputClass();

			HRESULT Initialize();
			void Release();

			BYTE Key[256];
			UINT KeyFrame[256];

			BOOL JoyPadUseFlag;
			DIJOYSTATE JoyPadData;
			UINT JoyPadButtonFrame[32];
			UINT JoyPadPovKeyFrame[4];

			//LPDIRECTINPUT8 LpDirectInput;	//DirectInput�̃I�u�W�F�N�g
			//std::vector<DIDEVICEINSTANCE> DeviceInfoAry;	//���݂̓��̓f�o�C�X���H�H
		
			//LPDIRECTINPUTDEVICE8 LpKeyboard;	//���̓f�o�C�X�F�L�[�{�[�h
			
		};


	};
};






