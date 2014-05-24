/*

	入力処理クラスソース

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
		//InputDevice用コールバック宣言？
		BOOL WINAPI DIEnumDevCallback(
		LPCDIDEVICEINSTANCE lpddi,
		LPVOID pvRef
		)
		{
			// 第2引数がDIDEVICEINSTANCE構造体のvecor STLだとします
			std::vector<DIDEVICEINSTANCE>* ptr = (std::vector<DIDEVICEINSTANCE>*)pvRef;
			DIDEVICEINSTANCE tmp = *lpddi;

			// 配列に格納
			ptr->push_back(tmp);

			// 列挙を続ける場合はDIENUM_CONTINUE、止める場合はDIENUM_STOPを指定
			// 全ての列挙が終了すると自動的にコールバックも終了するので、
			// 止める理由が無ければDIENUM_CONTINUEにする。
			return DIENUM_CONTINUE;
		}*/

		//DirectInput用のジョイパッドコールバック関数
		BOOL PASCAL EnumJoyPadCallBack(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
		{
			// ジョイスティックデバイスの作成
			if( FAILED(LpDirectInput->CreateDevice( lpddi->guidInstance,&LpJoypad,NULL ) )) 
			{
				Error("デバイス作成失敗");
				return DIENUM_STOP;
			}

			// 入力データ形式のセット
			
			if( FAILED(LpJoypad->SetDataFormat( &c_dfDIJoystick ) ) )
			{
				Error( "入力データ形式のセット失敗\n" );
				LpJoypad->Release();
				return DIENUM_STOP;
			}

			// 排他制御のセット
			if( FAILED(LpJoypad->SetCooperativeLevel( NatsumeManager::hWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE|DISCL_NOWINKEY )) ) 
			{
				Error( "排他制御のセット失敗\n");
				LpJoypad->Release();
				return DIENUM_STOP;
			}

			// 入力範囲のセット
			DIPROPRANGE     diprg;
			diprg.diph.dwSize = sizeof(diprg);
			diprg.diph.dwHeaderSize = sizeof(diprg.diph);
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMax = 1000;
			diprg.lMin = -1000;

			// X軸
			diprg.diph.dwObj = DIJOFS_X;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// Y軸
			diprg.diph.dwObj = DIJOFS_Y;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// Z軸
			diprg.diph.dwObj = DIJOFS_Z;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RX軸
			diprg.diph.dwObj = DIJOFS_RX;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RY軸
			diprg.diph.dwObj = DIJOFS_RY;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// RZ軸
			diprg.diph.dwObj = DIJOFS_RZ;
			LpJoypad->SetProperty( DIPROP_RANGE,&diprg.diph );

			// 起動準備完了
			LpJoypad->Poll();

			// 構築完了なら
			//DEBUG( "インスタンスの登録名 [%s]\n",lpddi->tszInstanceName );
			//DEBUG( "製品の登録名         [%s]\n",lpddi->tszProductName );
			//DEBUG( "構築完了\n","" );

			// 最初の１つのみで終わる
			return DIENUM_STOP;// 次のデバイスを列挙するにはDIENUM_CONTINUEを返す

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

		}//コンスト}


		InputClass::~InputClass()
		{			
			this->Release();
		}//デスト}

		
		HRESULT InputClass::Acquire()
		{
			{//キーボード
				ZeroMemory(&Key,sizeof(Key));
			
				if(FAILED(
				LpKeyboard->GetDeviceState(sizeof(Key),&Key)
				) )
				{//フォーカスが外れた場合等には再開させもう一度取得する
					LpKeyboard->Acquire();
					LpKeyboard->GetDeviceState(sizeof(Key),&Key);
				}
				for(int i = 0;i < 256; ++i)
				{//フレームの取得
					if(Key[i] & 0x80)
					{
						KeyFrame[i] ++;
					}else{
						KeyFrame[i] = 0;
					}
				}
				
			}

			//ジョイパッド
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
			//ジョイパッドの入力デバイスの数を見ている？
			//ジョイパッド一つなら多分いらない。。。
				
				LpDirectInput->EnumDevices(
					DI8DEVTYPE_JOYSTICK,
					&DIEnumDevCallback,
					&DeviceInfoAry,
					DIEDFL_ATTACHEDONLY
					
			*/}
			
			//キーボードの入力デバイスの設定
			if(FAILED(
			LpDirectInput->CreateDevice(GUID_SysKeyboard,&LpKeyboard,NULL)
				) )
			{
				Error("DirectInput CreateDevice Kebord Error");
				return E_FAIL;
			}
			if(FAILED(//フォーマットを確定させます（キーボード
			LpKeyboard->SetDataFormat( &c_dfDIKeyboard )
				) )
			{
				Error("DirectInput SetDetaFormat Kebord Error");
				return E_FAIL;
			}
			//排他処理（アプリフォーカスが合っていない時には入力処理等を停止する
			if(FAILED(
			LpKeyboard->SetCooperativeLevel( 
				NatsumeManager::hWnd,//第一引数にはウィンドウハンドルを
				DISCL_FOREGROUND|	//第二引数には排他処理のフラグを
				DISCL_NONEXCLUSIVE|	//一番手前にフォーカスがあるときのみと
				DISCL_NOWINKEY )	//その場合、ウィンドウズキーを停止
				))
			{
				Error("DirectInput SetCooperativeLevel(排他処理) Kebord Error");
				return E_FAIL;
			}

			//ジョイパッドの設定
			JoyPadUseFlag = TRUE;
			if(FAILED(
			LpDirectInput->EnumDevices( 
				DI8DEVCLASS_GAMECTRL,
				EnumJoyPadCallBack,
				NULL,DIEDFL_ATTACHEDONLY )))
			{
				Error("ジョイパッドが接続されていません");
				JoyPadUseFlag = FALSE;
			}
			if( !LpJoypad ) 
			{
				// ジョイスティックが１つも見つからない
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
				Error("DirectInput SetCooperativeLevel(排他処理) Joypad Error");
			}*/


			//Keyの初期化
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








