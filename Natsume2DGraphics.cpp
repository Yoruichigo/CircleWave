/*
	
	2D描画に関して

*/



#include "Natsume2DGraphics.h"





namespace NatsumeGrass
{
	namespace System
	{
		Polygon_2D::Polygon_2D()
		{




		};//コンスト

		Polygon_2D::~Polygon_2D()
		{

			


		};//デスト

		

		//四角形の生成
		HRESULT Polygon_2D::CreateSquare(float _x,float _y,float _Height,float _Width,DWORD _Color)
		{
			

			CustomVertex Ver[4] = {
				{(_x+_Width), _y, 0.0f, 1.0f, _Color, 0.0f, 0.0f},//右上
				{(_x+_Width), (_y+_Height), 0.0f, 1.0f, _Color, 0.0f, 1.0f},//右下
				{_x, _y, 0.0f, 1.0f, _Color, 1.0f, 0.0f},//左上
				{_x, (_y+_Height), 0.0f, 1.0f, _Color, 1.0f, 1.0f},//左下
			};

			IDirect3DVertexBuffer9* VerBuf;

			if(FAILED(
				NatsumeManager::LpD3DDevice->CreateVertexBuffer(
					sizeof(Ver),D3DUSAGE_WRITEONLY,
					FVF_CUSTOM,D3DPOOL_MANAGED,&VerBuf,NULL)
					) )
			{
				return E_FAIL;
			}

			void *Data;
			if(FAILED(
				VerBuf->Lock(0, sizeof(Ver),(void**)&Data, 0)
				) )
			{
				return E_FAIL;
			}

			memcpy(Data, Ver, sizeof(Ver));
			VerBuf->Unlock();

			NatsumeManager::LpD3DDevice->SetStreamSource(0, VerBuf, 0, sizeof(CustomVertex));
			NatsumeManager::LpD3DDevice->SetFVF(FVF_CUSTOM);
			NatsumeManager::LpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


			return S_OK;

		};//CreateSquare};

		

		TextureClass::TextureClass()
		{
			NextTextureKey = 0;

		}//コンスト}

		TextureClass::~TextureClass()
		{
			//読み込んだテクスチャのクリア
			TextureMap.clear();

		}//デスト

		INT TextureClass::LoadTexture(LPCSTR _Location)
		{
				

			static bool CreateBuf = false;
			if(CreateBuf == false)
			{
				CreateBuf = true;
				if(FAILED(
					NatsumeManager::LpD3DDevice->CreateVertexBuffer(
					sizeof(CustomVertex)*4,D3DUSAGE_WRITEONLY,FVF_CUSTOM,
					D3DPOOL_MANAGED,&VerBuff,NULL)
					))
				{
					Error("CreateVertexBuffer Error");
				}
			}

				

			if(FAILED(
				D3DXCreateTextureFromFileEx(
				NatsumeManager::LpD3DDevice,
				_Location,
				D3DX_DEFAULT,D3DX_DEFAULT,
				0,0,
				D3DFMT_A8R8G8B8,//D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				(D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER),
				D3DX_DEFAULT,0,NULL,NULL,
				&TextureMap[NextTextureKey])
				))
			{
				Error("テクスチャの読み込みに失敗しました。");
				return -1;
			}
			else
			{
				INT ReturnKey = NextTextureKey;
				//成功した場合はMapのKeyをずらし、登録したKeyを返します
				NextTextureKey ++ ;

				return ReturnKey;
			}

		};//Create};

		BOOL SetPos(IDirect3DVertexBuffer9 *_Vertex,float CX,float CY,float CWidth,float CHeight,BYTE _Alpha)
		{
			DWORD _Color = (_Alpha * 0x01000000) + 0x00ffffff;

			CustomVertex _v[4] = {
				{(CX + CWidth), (CY), 0.0f, 1.0f, _Color, 1.0f, 0.0f},//右上
				{(CX + CWidth), (CY + CHeight), 0.0f, 1.0f, _Color, 1.0f, 1.0f},//右下
				{(CX), (CY), 0.0f, 1.0f, _Color, 0.0f, 0.0f},//左上
				{(CX), (CY + CHeight), 0.0f, 1.0f, _Color, 0.0f, 1.0f},//左下
					
			};

			void *Data;
			if(SUCCEEDED(
				_Vertex->Lock(0,sizeof(_v),(void**)&Data,0)
				))
			{
				memcpy(Data,_v,sizeof(_v));
				_Vertex->Unlock();
				return TRUE;
			}

				

			return FALSE;
		}


		HRESULT TextureClass::DrawTexture(INT _Key,NatsumeGrass::External::CustomSquare _Square,BOOL _AlphaFlag,BYTE _AlphaColor)
		{
			//テクスチャの設定
			static bool One = false;
			if(One == false)
			{
				One = true;
				NatsumeManager::LpD3DDevice->SetFVF(FVF_CUSTOM);
			}
			/*
				テクスチャブレンディングについて
				**テクスチャブレンディングと
				**テクスチャステージ

				TextureStageとは、Textureをブレンドする時の順番の事。
				TextureStage事にTextureを割り当て、それを合成する。
				それをTextureBlendingと呼ぶ。

				各StageにTextureを選択するのが
				IDirect3DDevice9::SetTexture
				(DWORD Stage,IDirect3DBaseTexture* pTexture)

				そしてそのStageで「何をどういう風に合成するのか」(透過等
				を決めるのが
				IDirect3DDevice9::SetTextureStageState
				(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
				＊Stage
					はTextureStage
				＊Type
					は合成対象や、合成演算を決める部分で
					D3DTEXTURESTAGESTATETYPE列挙型
					に全て定義されている。
					ここでは何の設定をしたいのか、を決める部分
				＊Value
					はTypeによって意味が変わる引数である。
					Typeで色を設定したいと宣言した場合は使用する色
					Typeで透過を設定したいと宣言した場合は何を透明にするか
					Typeで演算子を設定したいと宣言した場合ならどういった演算子にするのか
					等と変わってきます。

				Stageの設定方法

						引数１			引数２
						------			------
					色	|    |	  □	|	 |
						------	演算子	------
							対象			 対象

						------			------
				透明度	|	 |	  □	|    |
						------	演算子	------
							対象			 対象

				一つのステージで合成を行う場合、主に決めなければいけないのは
				上記の空白□の部分です。
				＊対象
				という部分にはTexture,ポリゴンのディフューズ、スペキュラ、「前の結果」
				等が入ります。前の結果というのは前のステージでの出力結果。
				＊演算子
				という部分には加算、減算、乗算、等の合成方法が入ります。
				＊色
				という部分は目に見える色の計算を
				＊透明度
				という部分にはどれだけ透明にするか、を計算を

				＊＊テクスチャとテクスチャの合成は出来ないので注意＊＊

				SetTextureを単純に使用した場合の計算方法

				色　|テクスチャの色| |＊(×)| |ポリゴンの色|
				透明度|テクスチャの透明度| |引数1を使用| |無し|

				これをプログラムで書くとした場合
				SetTextureStageState(0, 	D3DTSS_COLORARG1 	, D3DTA_TEXTURE 	);
				SetTextureStageState(0, 	D3DTSS_COLOROP 		, D3DTOP_MODULATE 	);
				SetTextureStageState(0, 	D3DTSS_COLORARG2 	, D3DTA_DIFFUSE 	);
				SetTextureStageState(0, 	D3DTSS_ALPHAARG1 	, D3DTA_TEXTURE 	);
				SetTextureStageState(0, 	D3DTSS_ALPHAOP		, D3DTOP_SELECTARG1	);

				となります。（引用）

				上３段で色の設定を、下２段が透明度の設定となっており。
				上から色の引数１、演算子、引数２、
					　透明度の引数１、演算子
				という順になっている。

				色と透明度の引数１、演算子、引数２
				D3DTSS_COLORARG1	色・引数１
				D3DTSS_COLOROP		色・演算子
				D3DTSS_COLORARG2	色・引数

				D3DTSS_ALPHAARG1	α・引数１
				D3DTSS_ALPHAOP		α・演算子
				D3DTSS_ALPHAARG2	α・引数２

				見方として、
				{
					D3DTSS_の
					色　COLOR Color
					透明度 ALPHA Alpha

					引数 ARG Argument 
					の1or2
					
					演算子 OP Operator
				}
				という定義となっている。

				Valueの引数として
				{
					引数１・２対応？
					D3DTA_ を頭として
					TEXTURE
					DIFFUSE
					等、


					演算子?
					D3DTOP_　を頭に
					??
					MODULATE 掛算
				}
					

			*/


			if(_AlphaFlag == false)
			{//透過無し
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			}
			else
			{//透過有り
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 , D3DTA_TEXTURE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP ,D3DTOP_MODULATE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
				
				//アルファの使用宣言
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
				//ブレンド方法設定
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
				

			//D3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,  D3DTOP_MODULATE);
			//D3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			//D3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
				
			//カラーキー用アルファテスト
			//D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
			//D3DDevice->SetRenderState(D3DRS_ALPHAREF,0x80);
			//D3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);

			//System::D3DDevice->SetTexture(0,TextureMap[_Key]);

			/*
			//半透明
			D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
			D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			// アルファ合成の設定
			D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			*/

			if(SetPos(VerBuff,_Square.x,_Square.y,_Square.Width,_Square.Height,_AlphaColor))
			{
				//テクスチャの描画
				NatsumeManager::LpD3DDevice->SetTexture(0,TextureMap[_Key]);
				NatsumeManager::LpD3DDevice->SetStreamSource(0,VerBuff,0,sizeof(CustomVertex));
				NatsumeManager::LpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
			}
			else 
			{
				return E_FAIL;
			}
				
				
			return S_OK;
		};//Draw();



	};

};







