/*

	外部向けの関数ヘッダです。

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
		//四角形の構造体情報
		struct CustomSquare{
			CustomSquare(float _x = 0.0f,float _y = 0.0f,float _Height = 0.0f,float _Width = 0.0f);		//コンストラクタ
			float x,y;			//左上の頂点情報
			float Height,Width;	//x,yから右下にかけての大きさ
		};

		// *********************************
		// シーケンス
		// *********************************
		namespace Sequence
		{
			//初期化前に呼ぶ処理、ウィンドウのサイズ等を設定できます。
			HRESULT SetScreenInfo(INT _ScreenHeight,INT _ScreenWidth,LPCSTR _TitleName,BOOL _WindowMode);
			
			//初期化関数
			HRESULT Initialize(HINSTANCE _hInstance);
			
			//終了関数
			HRESULT End();

		};

		// *********************************
		// 描画
		// *********************************
		namespace Graphic
		{
			//画面のクリア、裏画面処理等を行ってくれる関数
			HRESULT BeginRender();

			//画面の更新、スクリーンフィリップ等を行う関数
			HRESULT EndRender();


			//ボックスの作成
			HRESULT CreateBox(CustomSquare _Obj,DWORD _Color = 0xffaaaaaa);

			//テクスチャの読み込み
			INT LoadTexture(LPCSTR _GraphicLocation);

			//テクスチャの表示
			HRESULT DrawTexture(INT _TextureKey,CustomSquare _Square,BOOL _AlphaUse = false,BYTE _AlphaColor = 0xff);
			HRESULT DrawTexture(INT _TextureKey,float _x,float _y,float _Height,float _Width,BOOL _AlphaUse = false,BYTE _AlphaColor = 0xff);


		};

		// *********************************
		// 入力
		// *********************************
		namespace Input
		{
			//Input系のアップデート関数、毎フレーム行ってください。
			HRESULT UpdateAcquire();

			//キーの入力チェック関数 _DIK_のDefineを渡してください。
			BOOL KeyCheck(BYTE _DIK_);

			//キーのフレームチェック関数 _DIK_のDefineを渡してください。
			//アップデートを行った際に更新されるので
			//入力の更新が画面更新より多ければ機能しません。
			UINT KeyCheckFrame(BYTE _DIK_);

			//ジョイパッドのキー入力のチェックをする際に使ってください。
			enum ENUM_JOYPAD
			{
				JOYKEY_BUTTON	,//ボタン
				JOYKEY_POV		,//POVキー
				JOYSTICK_LEFT_X	,//左スティックX
				JOYSTICK_LEFT_Y	,//左スティックY
				JOYSTICK_RIGHT_X,//右スティックX
				JOYSTICK_RIGHT_Y,//右スティックY
			};

			//ジョイパッドの入力チェック
			BOOL JoyPad_Check(ENUM_JOYPAD _Key,BYTE _Num);
			
			//ジョイパッドのフレームチェック
			UINT JoyPad_Frame(ENUM_JOYPAD _Key,BYTE _Num);

			//ジョイパッドのスティックチェック
			LONG JoyPad_Stick(ENUM_JOYPAD _Stick);


		};
		
		// *********************************
		// サウンド
		// *********************************
		namespace Sound
		{
			//サウンドの読み込み、返り値をKeyに渡してください。
			UINT LoadSound(TCHAR *_FileName);
			
			//再生
			void Play(UINT _Key);

			//停止
			void Stop(UINT _Key);
		};

		// *********************************
		// プロシージャ
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




