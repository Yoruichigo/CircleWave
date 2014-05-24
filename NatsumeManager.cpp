/*

	Natsumeマネージャソース

*/


#include "NatsumeManager.h"


// **********************
// ウィンドウプロシージャ
// **********************

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return NatsumeGrass::External::Proc::MsgProc(hWnd,uMsg,wParam,lParam);
}





namespace NatsumeGrass
{
	namespace System
	{
		HINSTANCE			NatsumeManager::hInstance	=	nullptr;
		HWND				NatsumeManager::hWnd		=	nullptr;
		LPDIRECT3DDEVICE9	NatsumeManager::LpD3DDevice	=	nullptr;
		LPDIRECT3D9			NatsumeManager::LpD3D		=	nullptr;


		NatsumeManager::NatsumeManager()
		{




		}

		NatsumeManager::~NatsumeManager()
		{
			Release();

		}


		HRESULT NatsumeManager::Initialize(HINSTANCE _hInstance)
		{
			hInstance = _hInstance;

			if(FAILED(
				InitWindow(_hInstance)
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				InitD3D()
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				OtherInitialize()
				))
			{
				return E_FAIL;
			}

			return S_OK;
		}

		void NatsumeManager::Release()
		{
			OtherRelease();

			SAFE_RELEASE(LpD3D);
			SAFE_RELEASE(LpD3DDevice);
		}

		//サブクラスの初期化です。
		HRESULT NatsumeManager::OtherInitialize()
		{
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::InputClass);
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::Polygon_2D);
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::NatsumeSound);
			return S_OK;
		}

		//サブクラスのリリースです。
		void NatsumeManager::OtherRelease()
		{
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::InputClass);
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::Polygon_2D);
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::NatsumeSound);

		}


		HRESULT NatsumeManager::InitWindow(HINSTANCE _hInstance)
		{
			//ウィンドウタイトルの保存
			LPCSTR WindowName = AppTitleName;

			WNDCLASSEX wc;
			ZeroMemory(&wc,sizeof(wc));
			wc.cbSize = sizeof(wc);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = _hInstance;
			wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			wc.lpszClassName = WindowName;
			wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
			RegisterClassEx(&wc);

			//ウィンドウハンドルに登録
			hWnd = CreateWindow(WindowName,WindowName,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				0,
				ScreenSizeWidth,
				ScreenSizeHeight,
				NULL,NULL,hInstance,NULL);

			if(! hWnd)
			{
				return E_FAIL;
			}
	
			//ValidateRect(hWnd,0);

			//ウィンドウの表示
			ShowWindow(hWnd,SW_SHOW);
			//UpdateWindow(hWnd);

			return S_OK;
		}

		HRESULT NatsumeManager::InitD3D()
		{
			
			LpD3D = NULL;
			LpD3DDevice = NULL;

			//まず最初にD3D9のコンポーネントを取得します。
			if(NULL == (
			LpD3D = Direct3DCreate9(D3D_SDK_VERSION)
				) )
			{
				Error("Direct3DCreate() FAILED");
				return E_FAIL;
			}

			D3DPRESENT_PARAMETERS D3DParam;
			//パラメータの設定
			D3DParam.BackBufferWidth = ScreenSizeWidth;
			D3DParam.BackBufferHeight = ScreenSizeHeight;

			//画面のフォーマット情報、沢山のモードがあるらしいので使い分けてみる
			//D3DParam.BackBufferFormat = D3DFMT_A8R8G8B8;
			D3DParam.BackBufferFormat = D3DFMT_UNKNOWN;//楽な奴？

			//バックバッファーの数、特別（？）な事をしない場合1枚で良いらしい
			//D3DParam.BackBufferCount = 2;
			D3DParam.BackBufferCount = 1;

			//マルチサンプルタイプ、ジャギ消し、アンチエイリアスを掛ける事が出来たりする。
			//ビデオカードが対応していない場合もあるらしいのでD3DMULTISAMPLE_NONEを
			//デフォルト設定しておいたほうが良いとの事。
			//IDirect3D9::CheckDeviceMultiSampleType関数でアンチエイリアスの情報を取得する事も可能
			D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;
			//上のマルチサンプルタイプのクオリティ、 D3DMULTISAMPLE_NONE;
			//を設定した場合は0で構わない。上に書いた関数での引数で返される値となっている
			D3DParam.MultiSampleQuality = 0;

			//SwapEffectはバックバッファとフロントバッファを切り替える方法を定義する物
			//D3DSWAPEFFECT_DISCARDを定義すると自動で判断してくれます。
			//ですが、アルファブレンディング等の設定が保障されていないとかんとか
			D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

			//画面描画するウィンドウハンドルの指定、NULLの場合現在フォーカスしているウィンドウを使用
			D3DParam.hDeviceWindow = NULL;

			//TRUEでウィンドウモード、FALSEでフルスクリーン
			D3DParam.Windowed = WindowMode;

			//深度ステンシルバッファの有無の設定、３Ｄを扱うのならTRUEで
			D3DParam.EnableAutoDepthStencil = TRUE;
			//上のステンシルバッファのフォーマットの設定
			//D3DFORMAT列挙型のマニュアルを見るとよい
			//D3DParam.AutoDepthStencilFormat = D3DFMT_D24X8;
			D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;

			//Flagsはバックバッファからフロントバッファに転送するときの
			//機能のオプションの設定、とりあえず０指定（後々追記予定）
			D3DParam.Flags = 0;

			//フルスクリーン時のリフレッシュレートの指定
			//ウィンドウモードの場合なら０にする必要があり、
			//フルスクリーン時にも適当な値を入れることは出来ない。
			D3DParam.FullScreen_RefreshRateInHz =0;

			//スワップの書き換えのタイミングの設定
			//最速にするとテアリングが発生したり等きちんと設定しておかなければならない
			D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			//D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			//D3DParam.PresentationInterval = 0;

			//デバイスの生成、4回やる理由は前半でサポートしていない物だった場合の事を考えての事
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			{
				Release();
				return E_FAIL;
			}

			return S_OK;
		}



		//画面のクリア
		HRESULT NatsumeManager::ClearRender()
		{
			if(FAILED(
				LpD3DDevice->Clear(
				0,NULL,
				D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,//D3DCLEAR_TARGET,
				D3DCOLOR_XRGB(0,255,255),1.0f,0)
				))
			{
				return E_FAIL;
			}
			return S_OK;
		}

		//描画開始
		HRESULT NatsumeManager::BeginRender()
		{
			if(FAILED(
				LpD3DDevice->BeginScene()
				) )
			{
				return E_FAIL;
			}


			return S_OK;
		}

		//描画終了
		HRESULT NatsumeManager::EndRender()
		{
			if(FAILED(
				LpD3DDevice->EndScene()
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				LpD3DDevice->Present(NULL,NULL,NULL,NULL)
				) )
			{
				return E_FAIL;
			}

			return S_OK;
		}












	};
};

