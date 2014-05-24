/*

	2D�̕`��w�b�_

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
		//���_���
		struct CustomVertex{
			float 	x, y, z; 	// ���_���W
			float 	rhw; 	// ���Z��
			DWORD 	dwColor; 	// ���_�̐F
			float 	u, v; 	//�@�e�N�X�`�����W
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


		//�e�N�X�`���������N���X
		class TextureClass : public Natsume_Core_Single<TextureClass>
		{
		public:
			friend class Natsume_Core_Single<TextureClass>;

			//Texture��Key��Ԃ��܂��A�\�����鎞�Ɏg�p���ĉ������B(�Ԃ茌��-1�͎��s)
			INT LoadTexture(LPCSTR _GraphicLocation);

			HRESULT DrawTexture(INT _TextureKey,NatsumeGrass::External::CustomSquare _Square,BOOL _AlphaFlag,BYTE _AlphaColor);

		private:
			TextureClass();
			~TextureClass();
				
			std::map<INT,IDirect3DTexture9*> TextureMap;

			//�e�N�X�`���̃L�[
			INT NextTextureKey;

			IDirect3DVertexBuffer9 *VerBuff;
		};

	}

};





