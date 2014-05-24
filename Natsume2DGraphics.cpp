/*
	
	2D�`��Ɋւ���

*/



#include "Natsume2DGraphics.h"





namespace NatsumeGrass
{
	namespace System
	{
		Polygon_2D::Polygon_2D()
		{




		};//�R���X�g

		Polygon_2D::~Polygon_2D()
		{

			


		};//�f�X�g

		

		//�l�p�`�̐���
		HRESULT Polygon_2D::CreateSquare(float _x,float _y,float _Height,float _Width,DWORD _Color)
		{
			

			CustomVertex Ver[4] = {
				{(_x+_Width), _y, 0.0f, 1.0f, _Color, 0.0f, 0.0f},//�E��
				{(_x+_Width), (_y+_Height), 0.0f, 1.0f, _Color, 0.0f, 1.0f},//�E��
				{_x, _y, 0.0f, 1.0f, _Color, 1.0f, 0.0f},//����
				{_x, (_y+_Height), 0.0f, 1.0f, _Color, 1.0f, 1.0f},//����
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

		}//�R���X�g}

		TextureClass::~TextureClass()
		{
			//�ǂݍ��񂾃e�N�X�`���̃N���A
			TextureMap.clear();

		}//�f�X�g

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
				Error("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B");
				return -1;
			}
			else
			{
				INT ReturnKey = NextTextureKey;
				//���������ꍇ��Map��Key�����炵�A�o�^����Key��Ԃ��܂�
				NextTextureKey ++ ;

				return ReturnKey;
			}

		};//Create};

		BOOL SetPos(IDirect3DVertexBuffer9 *_Vertex,float CX,float CY,float CWidth,float CHeight,BYTE _Alpha)
		{
			DWORD _Color = (_Alpha * 0x01000000) + 0x00ffffff;

			CustomVertex _v[4] = {
				{(CX + CWidth), (CY), 0.0f, 1.0f, _Color, 1.0f, 0.0f},//�E��
				{(CX + CWidth), (CY + CHeight), 0.0f, 1.0f, _Color, 1.0f, 1.0f},//�E��
				{(CX), (CY), 0.0f, 1.0f, _Color, 0.0f, 0.0f},//����
				{(CX), (CY + CHeight), 0.0f, 1.0f, _Color, 0.0f, 1.0f},//����
					
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
			//�e�N�X�`���̐ݒ�
			static bool One = false;
			if(One == false)
			{
				One = true;
				NatsumeManager::LpD3DDevice->SetFVF(FVF_CUSTOM);
			}
			/*
				�e�N�X�`���u�����f�B���O�ɂ���
				**�e�N�X�`���u�����f�B���O��
				**�e�N�X�`���X�e�[�W

				TextureStage�Ƃ́ATexture���u�����h���鎞�̏��Ԃ̎��B
				TextureStage����Texture�����蓖�āA�������������B
				�����TextureBlending�ƌĂԁB

				�eStage��Texture��I������̂�
				IDirect3DDevice9::SetTexture
				(DWORD Stage,IDirect3DBaseTexture* pTexture)

				�����Ă���Stage�Łu�����ǂ��������ɍ�������̂��v(���ߓ�
				�����߂�̂�
				IDirect3DDevice9::SetTextureStageState
				(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
				��Stage
					��TextureStage
				��Type
					�͍����Ώۂ�A�������Z�����߂镔����
					D3DTEXTURESTAGESTATETYPE�񋓌^
					�ɑS�Ē�`����Ă���B
					�����ł͉��̐ݒ���������̂��A�����߂镔��
				��Value
					��Type�ɂ���ĈӖ����ς������ł���B
					Type�ŐF��ݒ肵�����Ɛ錾�����ꍇ�͎g�p����F
					Type�œ��߂�ݒ肵�����Ɛ錾�����ꍇ�͉��𓧖��ɂ��邩
					Type�ŉ��Z�q��ݒ肵�����Ɛ錾�����ꍇ�Ȃ�ǂ����������Z�q�ɂ���̂�
					���ƕς���Ă��܂��B

				Stage�̐ݒ���@

						�����P			�����Q
						------			------
					�F	|    |	  ��	|	 |
						------	���Z�q	------
							�Ώ�			 �Ώ�

						------			------
				�����x	|	 |	  ��	|    |
						------	���Z�q	------
							�Ώ�			 �Ώ�

				��̃X�e�[�W�ō������s���ꍇ�A��Ɍ��߂Ȃ���΂����Ȃ��̂�
				��L�̋󔒁��̕����ł��B
				���Ώ�
				�Ƃ��������ɂ�Texture,�|���S���̃f�B�t���[�Y�A�X�y�L�����A�u�O�̌��ʁv
				��������܂��B�O�̌��ʂƂ����̂͑O�̃X�e�[�W�ł̏o�͌��ʁB
				�����Z�q
				�Ƃ��������ɂ͉��Z�A���Z�A��Z�A���̍������@������܂��B
				���F
				�Ƃ��������͖ڂɌ�����F�̌v�Z��
				�������x
				�Ƃ��������ɂ͂ǂꂾ�������ɂ��邩�A���v�Z��

				�����e�N�X�`���ƃe�N�X�`���̍����͏o���Ȃ��̂Œ��Ӂ���

				SetTexture��P���Ɏg�p�����ꍇ�̌v�Z���@

				�F�@|�e�N�X�`���̐F| |��(�~)| |�|���S���̐F|
				�����x|�e�N�X�`���̓����x| |����1���g�p| |����|

				������v���O�����ŏ����Ƃ����ꍇ
				SetTextureStageState(0, 	D3DTSS_COLORARG1 	, D3DTA_TEXTURE 	);
				SetTextureStageState(0, 	D3DTSS_COLOROP 		, D3DTOP_MODULATE 	);
				SetTextureStageState(0, 	D3DTSS_COLORARG2 	, D3DTA_DIFFUSE 	);
				SetTextureStageState(0, 	D3DTSS_ALPHAARG1 	, D3DTA_TEXTURE 	);
				SetTextureStageState(0, 	D3DTSS_ALPHAOP		, D3DTOP_SELECTARG1	);

				�ƂȂ�܂��B�i���p�j

				��R�i�ŐF�̐ݒ���A���Q�i�������x�̐ݒ�ƂȂ��Ă���B
				�ォ��F�̈����P�A���Z�q�A�����Q�A
					�@�����x�̈����P�A���Z�q
				�Ƃ������ɂȂ��Ă���B

				�F�Ɠ����x�̈����P�A���Z�q�A�����Q
				D3DTSS_COLORARG1	�F�E�����P
				D3DTSS_COLOROP		�F�E���Z�q
				D3DTSS_COLORARG2	�F�E����

				D3DTSS_ALPHAARG1	���E�����P
				D3DTSS_ALPHAOP		���E���Z�q
				D3DTSS_ALPHAARG2	���E�����Q

				�����Ƃ��āA
				{
					D3DTSS_��
					�F�@COLOR Color
					�����x ALPHA Alpha

					���� ARG Argument 
					��1or2
					
					���Z�q OP Operator
				}
				�Ƃ�����`�ƂȂ��Ă���B

				Value�̈����Ƃ���
				{
					�����P�E�Q�Ή��H
					D3DTA_ �𓪂Ƃ���
					TEXTURE
					DIFFUSE
					���A


					���Z�q?
					D3DTOP_�@�𓪂�
					??
					MODULATE �|�Z
				}
					

			*/


			if(_AlphaFlag == false)
			{//���ߖ���
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			}
			else
			{//���ߗL��
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 , D3DTA_TEXTURE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP ,D3DTOP_MODULATE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
				NatsumeManager::LpD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
				
				//�A���t�@�̎g�p�錾
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
				//�u�����h���@�ݒ�
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				NatsumeManager::LpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
				

			//D3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,  D3DTOP_MODULATE);
			//D3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			//D3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
				
			//�J���[�L�[�p�A���t�@�e�X�g
			//D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
			//D3DDevice->SetRenderState(D3DRS_ALPHAREF,0x80);
			//D3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);

			//System::D3DDevice->SetTexture(0,TextureMap[_Key]);

			/*
			//������
			D3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// �����̐�������Z����
			D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			// �A���t�@�����̐ݒ�
			D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			*/

			if(SetPos(VerBuff,_Square.x,_Square.y,_Square.Width,_Square.Height,_AlphaColor))
			{
				//�e�N�X�`���̕`��
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







