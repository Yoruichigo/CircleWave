/*

	External ソース

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
		// プロシージャ系
		// *********************************
		namespace Proc
		{
			// *******************************
			// WndProcの処理を此処で行います。
			// *******************************
			LRESULT MsgProc(HWND _hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
			{
				switch(uMsg)
				{
				case WM_KEYDOWN:
					switch( (char)wParam )
					{
					case VK_ESCAPE:
						//if(YesNoCheckBox("終了しますか？","終了確認") == true)
						//{
						//	PostQuitMessage(WM_QUIT);
						//}
						break;
					}

					break;

				case WM_SETCURSOR: // カーソルの設定
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
		// シーケンス処理、初期化や終了処理等です。
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
		// 描画関係
		// *********************************
		namespace Graphic
		{
			HRESULT BeginRender()
			{
				//画面クリアと
				if( FAILED(
					( GET_INSTANCE(NatsumeGrass::System::NatsumeManager) )->ClearRender()
					) )
				{
					Error("NatsumeGrass::Draw Clear Error");
					return E_FAIL;
				}
				//BeginRenderを呼び出します。
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
				//画面に描画を行います。
				if(FAILED(
				( GET_INSTANCE(NatsumeGrass::System::NatsumeManager) )->EndRender()
					))
				{
					Error("NatsumeGrass::Draw EndRender Error");
					return E_FAIL;
				}
				return S_OK;
			}
			

			//色付き箱の表示
			HRESULT CreateBox(CustomSquare _Obj,DWORD _Color )
			{
				NatsumeGrass::System::Polygon_2D *PolInstance;
				PolInstance = GET_INSTANCE(NatsumeGrass::System::Polygon_2D);

				PolInstance->CreateSquare(_Obj.x,_Obj.y,_Obj.Height,_Obj.Width,_Color);

				return S_OK;
			}

			//テクスチャの読み込み
			INT LoadTexture(LPCSTR _GraphicLocation)
			{
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass))->LoadTexture(_GraphicLocation);
			}

			//テクスチャの表示 
			HRESULT DrawTexture(INT _TextureKey,CustomSquare _Square,BOOL _AlphaUse,BYTE _AlphaColor)
			{//(引数２つver)
				
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass) )->DrawTexture(_TextureKey,_Square,_AlphaUse,_AlphaColor);
			}

			HRESULT DrawTexture(INT _TextureKey,float _x,float _y,float _Height,float _Width,BOOL _AlphaUse,BYTE _AlphaColor)
			{//(引数複数ver)
				CustomSquare _Square = CustomSquare(_x,_y,_Height,_Width);
				return (GET_INSTANCE(NatsumeGrass::System::TextureClass) )->DrawTexture(_TextureKey,_Square,_AlphaUse,_AlphaColor);
			}


		};


		// *********************************
		// 入力関係
		// *********************************
		namespace Input
		{
			//入力インターフェースのアップデート
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
		// サウンド関係
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




		//カスタムスクウェアのコンストラクタ（０初期化）
		CustomSquare::CustomSquare(float _x,float _y,float _Height,float _Width)
		{
			ZeroMemory(this,sizeof(CustomSquare));
			x = _x; y = _y; Height = _Height,Width = _Width;
		}

	};

	


};



