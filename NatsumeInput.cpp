/*

	���͏����N���X�\�[�X

*/


#include "NatsumeInput.h"




namespace NatsumeGrass
{
	namespace System
	{
		
		static LPDIRECTINPUT8 LpDirectInput = nullptr;
		//std::vector<DIDEVICEINSTANCE> DeviceInfoAry;
		static LPDIRECTINPUTDEVICE8 LpKeyboard = nullptr;
		static LPDIRECTINPUTDEVICE8 LpJoypad = nullptr;
		
		/*
		//InputDevice�p�R�[���o�b�N�錾�H
		BOOL WINAPI DIEnumDevCallback(
		LPCDIDEVICEINSTANCE lpddi,
		LPVOID pvRef
		)
		{
			// ��2������DIDEVICEINSTANCE�\���̂�vecor STL���Ƃ��܂�
			std::vector<DIDEVICEINSTANCE>* ptr = (std::vector<DIDEVICEINSTANCE>*)pvRef;
			DIDEVICEINSTANCE tmp = *lpddi;

			// �z��Ɋi�[
			ptr->push_back(tmp);

			// �񋓂𑱂���ꍇ��DIENUM_CONTINUE�A�~�߂�ꍇ��DIENUM_STOP���w��
			// �S�Ă̗񋓂��I������Ǝ����I�ɃR�[���o�b�N���I������̂ŁA
			// �~�߂闝�R���������DIENUM_CONTINUE�ɂ���B
			return DIENUM_CONTINUE;
		}*/

		//DirectInput�p�̃W���C�p�b�h�R�[���o�b�N�֐�
		BOOL PASCAL EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
		{
			// �W���C�X�e�B�b�N�f�o�C�X�̍쐬
			if( FAILED(LpDirectInput->CreateDevice( lpddi->guidInstance,&LpJoypad,NULL ) )) 
			{
				Error("�f�o�C�X�쐬���s");
				return DIENUM_STOP;
			}

			// ���̓f�[�^�`���̃Z�b�g
			
			if( FAILED(LpJoypad->SetDataFormat( &c_dfDIJoystick ) ) )
			{
				Error( "���̓f�[�^�`���̃Z�b�g���s\n" );
				LpJoypad->Release();
				return DIENUM_STOP;
			}

			// �r������̃Z�b�g
			if( FAILED(LpJoypad->SetCooperativeLevel( NatsumeManager::hWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE|DISCL_NOWINKEY )) ) 
			{
				Error( "�r������̃Z�b�g���s\n");
				LpJoypad->Release();
				return DIENUM_STOP;
			}

			// ���͔͈͂̃Z�b�g
			DIPROPRANGE     diprg;
			diprg.diph.dwSize = sizeof(diprg);
			diprg.diph.dwHeaderSize = sizeof(diprg.diph);
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMax = 1000;
			diprg.lMin = -1000;

			// X��
			diprg.diph.dwObj = DIJOFS_X;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// Y��
			diprg.diph.dwObj = DIJOFS_Y;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// Z��
			diprg.diph.dwObj = DIJOFS_Z;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RX��
			diprg.diph.dwObj = DIJOFS_RX;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RY��
			diprg.diph.dwObj = DIJOFS_RY;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RZ��
			diprg.diph.dwObj = DIJOFS_RZ;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// �N����������
			LpJoypad->Poll();

			// �\�z�����Ȃ�
			//DEBUG( "�C���X�^���X�̓o�^�� [%s]\n",lpddi->tszInstanceName );
			//DEBUG( "���i�̓o�^��         [%s]\n",lpddi->tszProductName );
			//DEBUG( "�\�z����\n","" );

			// �ŏ��̂P�݂̂ŏI���
			return DIENUM_STOP;// ���̃f�o�C�X��񋓂���ɂ�DIENUM_CONTINUE��Ԃ�

		}

		InputClass::InputClass()
		{
			//LpDirectInput	= nullptr;
			//LpKeyboard		= nullptr;
			//LpJoypad		= nullptr;

			if(FAILED(this->Initialize()))
			{
				Error("InputClass Initialize Error");
				Release();
			}

		}//�R���X�g}


		InputClass::~InputClass()
		{			
			this->Release();
		}//�f�X�g}

		
		HRESULT InputClass::Acquire()
		{
			{//�L�[�{�[�h
				ZeroMemory(&Key,sizeof(Key));
			
				if(FAILED(
				LpKeyboard->GetDeviceState(sizeof(Key),&Key)
				) )
				{//�t�H�[�J�X���O�ꂽ�ꍇ���ɂ͍ĊJ����������x�擾����
					LpKeyboard->Acquire();
					LpKeyboard->GetDeviceState(sizeof(Key),&Key);
				}
				for(int i = 0;i < 256; ++i)
				{//�t���[���̎擾
					if(Key[i] & 0x80)
					{
						KeyFrame[i] ++;
					}else{
						KeyFrame[i] = 0;
					}
				}
				
			}

			//�W���C�p�b�h
			if(JoyPadUseFlag == TRUE)
			{
				ZeroMemory(&JoyPadData,sizeof(JoyPadData));
				if(FAILED(LpJoypad->GetDeviceState( sizeof(JoyPadData),&JoyPadData)))
				{
					LpJoypad->Acquire();
					LpJoypad->GetDeviceState(sizeof(JoyPadData),&JoyPadData);
				}
				for(int i = 0;i < 32;++i)
				{
					if(JoyPadData.rgbButtons[i] & 0x80)
					{
						JoyPadButtonFrame[i] ++;
					}
					else
					{
						JoyPadButtonFrame[i] = 0;
					}
				}
				for(int i = 0 ; i < 4 ; ++i)
				{
					if(JoyPadData.rgdwPOV[i] & 0x80)
					{
						JoyPadPovKeyFrame[i] ++;
					}
					else 
					{
						JoyPadPovKeyFrame[i] = 0;
					}
				}
			}

			return S_OK;
		}


		HRESULT InputClass::Initialize()
		{
			if(FAILED(
			DirectInput8Create(
				NatsumeManager::hInstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(LPVOID*)&LpDirectInput,
				NULL) 
				))
			{
				Error("Directinput Initialize Error");
				return E_FAIL;
			}

			
			{/*
			//�W���C�p�b�h�̓��̓f�o�C�X�̐������Ă���H
			//�W���C�p�b�h��Ȃ瑽������Ȃ��B�B�B
				
				LpDirectInput->EnumDevices(
					DI8DEVTYPE_JOYSTICK,
					&DIEnumDevCallback,
					&DeviceInfoAry,
					DIEDFL_ATTACHEDONLY
					
			*/}
			
			//�L�[�{�[�h�̓��̓f�o�C�X�̐ݒ�
			if(FAILED(
			LpDirectInput->CreateDevice(GUID_SysKeyboard,&LpKeyboard,NULL)
				) )
			{
				Error("DirectInput CreateDevice Kebord Error");
				return E_FAIL;
			}
			if(FAILED(//�t�H�[�}�b�g���m�肳���܂��i�L�[�{�[�h
			LpKeyboard->SetDataFormat( &c_dfDIKeyboard )
				) )
			{
				Error("DirectInput SetDetaFormat Kebord Error");
				return E_FAIL;
			}
			//�r�������i�A�v���t�H�[�J�X�������Ă��Ȃ����ɂ͓��͏��������~����
			if(FAILED(
			LpKeyboard->SetCooperativeLevel( 
				NatsumeManager::hWnd,//�������ɂ̓E�B���h�E�n���h����
				DISCL_FOREGROUND|	//�������ɂ͔r�������̃t���O��
				DISCL_NONEXCLUSIVE|	//��Ԏ�O�Ƀt�H�[�J�X������Ƃ��݂̂�
				DISCL_NOWINKEY )	//���̏ꍇ�A�E�B���h�E�Y�L�[���~
				))
			{
				Error("DirectInput SetCooperativeLevel(�r������) Kebord Error");
				return E_FAIL;
			}

			//�W���C�p�b�h�̐ݒ�
			JoyPadUseFlag = TRUE;
			if(FAILED(
			LpDirectInput->EnumDevices( 
				DI8DEVCLASS_GAMECTRL,
				EnumJoyPadCallBack,
				NULL,DIEDFL_ATTACHEDONLY )))
			{
				Error("�W���C�p�b�h���ڑ�����Ă��܂���");
				JoyPadUseFlag = FALSE;
			}
			if( !LpJoypad ) 
			{
				// �W���C�X�e�B�b�N���P��������Ȃ�
				JoyPadUseFlag = FALSE;
			}
			/*
			if(FAILED(
				LpDirectInput->CreateDevice(GUID_SysKeyboard,&LpKeyboard,NULL)
				) )
			{
				Error("DirectInput CreateDevice JoyPad Error");
			}
			if(FAILED(
			LpKeyboard->SetDataFormat( &c_dfDIKeyboard )
				) )
			{
				Error("DirectInput SetDetaFormat JoyPad Error");
			}
			if(FAILED(
			LpKeyboard->SetCooperativeLevel( 
				hWnd,
				DISCL_FOREGROUND|	
				DISCL_NONEXCLUSIVE|	
				DISCL_NOWINKEY )
				))
			{
				Error("DirectInput SetCooperativeLevel(�r������) Joypad Error");
			}*/


			//Key�̏�����
			ZeroMemory(&Key,sizeof(Key));
			ZeroMemory(&KeyFrame,sizeof(KeyFrame));
			ZeroMemory(&JoyPadData,sizeof(JoyPadData));
			ZeroMemory(&JoyPadButtonFrame,sizeof(JoyPadButtonFrame));
			ZeroMemory(&JoyPadPovKeyFrame,sizeof(JoyPadPovKeyFrame));
			LpKeyboard->Acquire();
			if(JoyPadUseFlag == TRUE)
			{
				LpJoypad->Acquire();
			}
			return S_OK;
		}

		void InputClass::Release()
		{
			SAFE_RELEASE(LpKeyboard);
			SAFE_RELEASE(LpJoypad);
			SAFE_RELEASE(LpDirectInput);
		}

	};
};








