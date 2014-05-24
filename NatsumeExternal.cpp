/*

	External �\�[�X

*/



#include "NatsumeExternal.h"


INT NatsumeGrass::System::ScreenSizeWidth = 800;
INT NatsumeGrass::System::ScreenSizeHeight = 600;
LPCSTR NatsumeGrass::System::AppTitleName = "Title";
BOOL NatsumeGrass::System::WindowMode = true;


// *********************************
// 
// *********************************

namespace NatsumeGrass
{
	namespace External
	{
		
		// *********************************
		// �v���V�[�W���n
		// *********************************
		namespace Proc
		{
			// *******************************
			// WndProc�̏����������ōs���܂��B
			// *******************************
			LRESULT MsgProc(HWND _hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
			{
				switch(uMsg)
				{
				case WM_KEYDOWN:
					switch( (char)wParam )
					{
					case VK_ESCAPE:
						//if(YesNoCheckBox("�I�����܂����H","�I���m�F") == true)
						//{
						//	PostQuitMessage(WM_QUIT);
						//}
						break;
					}

					break;

				case WM_SETCURSOR: // �J�[�\���̐ݒ�
					SetCursor(NULL);
					break;

				case WM_DESTROY:

					PostQuitMessage(0);
					break;
				}
				return DefWindowProc(_hWnd,uMsg,wParam,lParam);

			}
		};



		
		// *********************************
		// �V�[�P���X�����A��������I���������ł��B
		// *********************************
		namespace Sequence
		{
			
			HRESULT SetScreenInfo(INT _ScreenHeight,INT _ScreenWidth,LPCSTR _TitleName,BOOL _WindowMode)
			{
				System::ScreenSizeHeight = _ScreenHeight;
				System::ScreenSizeWidth = _ScreenWidth;
				System::AppTitleName = _TitleName;
				System::WindowMode = _WindowMode;

				return S_OK;
			}

			HRESULT Initialize(HINSTANCE _hInstance)
			{

				CREATE_SINGLETON_INSTANCE(System::NatsumeManager);

				if(FAILED(
				(GET_INSTANCE(System::NatsumeManager))->Initialize(_hInstance)
				))
				{
					return E_FAIL;
				}

				return S_OK;
			}

			HRESULT End()
			{
				DESTROY_SINGLETON_INSTANCE(System::NatsumeManager);

				return S_OK;
			}



		};


		// *********************************
		// �`��֌W
		// *********************************
		namespace Graphic
		{
			HRESULT BeginRender()
			{
				//��ʃN���A��
				if( FAILED(
					( GET_INSTANCE(NatsumeGrass::System::NatsumeManager) )->ClearRender()
					) )
				{
					Error("NatsumeGrass::Draw Clear Error");
					return E_FAIL;
				}
				//BeginRender���Ăяo���܂��B
				if(FAILED(
					( GET_INSTANCE(NatsumeGrass::System::NatsumeManager) )->BeginRender()
					) )
				{
					Error("NatsumeGrass::Draw BeginRender Error");
					return E_FAIL;
				}

				return S_OK;
			}


		

			HRESULT EndRender()
			{
				//��ʂɕ`����s���܂��B
				if(FAILED(
				( GET_INSTANCE(NatsumeGrass::System::NatsumeManager) )->EndRender()
					))
				{
					Error("NatsumeGrass::Draw EndRender Error");
					return E_FAIL;
				}
				return S_OK;
			}
			

			//�F�t�����̕\��
			HRESULT CreateBox(CustomSquare _Obj,DWORD _Color )
			{
				NatsumeGrass::System::Polygon_2D *PolInstance;
				PolInstance = GET_INSTANCE(NatsumeGrass::System::Polygon_2D);

				PolInstance->CreateSquare(_Obj.x,_Obj.y,_Obj.Height,_Obj.Width,_Color);

				return S_OK;
			}

			//�e�N�X�`���̓ǂݍ���
			INT LoadTexture(LPCSTR _GraphicLocation)
			{
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass))->LoadTexture(_GraphicLocation);
			}

			//�e�N�X�`���̕\�� 
			HRESULT DrawTexture(INT _TextureKey,CustomSquare _Square,BOOL _AlphaUse,BYTE _AlphaColor)
			{//(�����Q��ver)
				
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass) )->DrawTexture(_TextureKey,_Square,_AlphaUse,_AlphaColor);
			}

			HRESULT DrawTexture(INT _TextureKey,float _x,float _y,float _Height,float _Width,BOOL _AlphaUse,BYTE _AlphaColor)
			{//(��������ver)
				CustomSquare _Square = CustomSquare(_x,_y,_Height,_Width);
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass) )->DrawTexture(_TextureKey,_Square,_AlphaUse,_AlphaColor);
			}


		};


		// *********************************
		// ���͊֌W
		// *********************************
		namespace Input
		{
			//���̓C���^�[�t�F�[�X�̃A�b�v�f�[�g
			HRESULT UpdateAcquire()
			{
				return (GET_INSTANCE(NatsumeGrass::System::InputClass) )->Acquire();
			}

			BOOL KeyCheck(BYTE _DIK_)
			{
				return ( (GET_INSTANCE(NatsumeGrass::System::InputClass) )->KeyCheck(_DIK_) );
			}

			UINT KeyCheckFrame(BYTE _DIK_)
			{
				return ( (GET_INSTANCE(NatsumeGrass::System::InputClass) )->KeyCheckFrame(_DIK_) );
			}

			BOOL JoyPad_Check(ENUM_JOYPAD _Key,BYTE _Num)
			{
				switch(_Key)
				{
				default :
					return -1;
					break;
				case JOYKEY_BUTTON:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_Button_KeyCheck(_Num);
					break;
				case JOYKEY_POV:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_Pov_KeyCheck(_Num);
					break;
				}
			}

			UINT JoyPad_Frame(ENUM_JOYPAD _Key,BYTE _Num)
			{
				switch(_Key)
				{
				default:
					return -1;
					break;
				case JOYKEY_BUTTON:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_Button_Frame(_Num);
					break;
				case JOYKEY_POV:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_Pov_Frame(_Num);
					break;
				}
			}

			LONG JoyPad_Stick(ENUM_JOYPAD _Stick)
			{
				switch(_Stick)
				{
				default:
					return -1;
					break;
				case JOYSTICK_LEFT_X:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_StickLeftX();
					break;
				case JOYSTICK_LEFT_Y:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_StickLeftY();
					break;
				case JOYSTICK_RIGHT_X:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_StickRightX();
					break;
				case JOYSTICK_RIGHT_Y:
					return (GET_INSTANCE(NatsumeGrass::System::InputClass))->Joy_StickRightY();
					break;

				}
			}


		};


		// *********************************
		// �T�E���h�֌W
		// *********************************
		namespace Sound
		{
			UINT LoadSound(TCHAR *_FileName)
			{
				return ( (GET_INSTANCE(NatsumeGrass::System::NatsumeSound))->SoundDataLoad(_FileName) );
			}

			void Play(UINT _Key)
			{
				(GET_INSTANCE(NatsumeGrass::System::NatsumeSound))->SoundPlay(_Key);
			}

		};




		//�J�X�^���X�N�E�F�A�̃R���X�g���N�^�i�O�������j
		CustomSquare::CustomSquare(float _x,float _y,float _Height,float _Width)
		{
			ZeroMemory(this,sizeof(CustomSquare));
			x = _x; y = _y; Height = _Height,Width = _Width;
		}

	};

	


};



