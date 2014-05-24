/*

	2Dの描画ヘッダ

*/


#pragma once

#include "NatsumeSystem.h"


#include "NatsumeManager.h"

#include "NatsumeExternal.h"




#define FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


namespace NatsumeGrass
{
	namespace External
	{
		struct CustomSquare;
	};
};


namespace NatsumeGrass
{
	namespace System
	{
		//頂点情報
		struct CustomVertex{
			float 	x, y, z; 	// 頂点座標
			float 	rhw; 	// 除算数
			DWORD 	dwColor; 	// 頂点の色
			float 	u, v; 	//　テクスチャ座標
		};


		class Polygon_2D :public Natsume_Core_Single<Polygon_2D>
		{
		public:
			friend class Natsume_Core_Single<Polygon_2D>;
			
			HRESULT CreateSquare(float,float,float,float,DWORD);

			

		private:
			Polygon_2D();
			~Polygon_2D();

			static Polygon_2D *Instance;

		};


		//テクスチャを扱うクラス
		class TextureClass : public Natsume_Core_Single<TextureClass>
		{
		public:
			friend class Natsume_Core_Single<TextureClass>;

			//TextureのKeyを返します、表示する時に使用して下さい。(返り血が-1は失敗)
			INT LoadTexture(LPCSTR _GraphicLocation);

			HRESULT DrawTexture(INT _TextureKey,NatsumeGrass::External::CustomSquare _Square,BOOL _AlphaFlag,BYTE _AlphaColor);

		private:
			TextureClass();
			~TextureClass();
				
			std::map<INT,IDirect3DTexture9*> TextureMap;

			//テクスチャのキー
			INT NextTextureKey;

			IDirect3DVertexBuffer9 *VerBuff;
		};

	}

};





