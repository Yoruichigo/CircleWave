/*

	�K�{�C���N���[�h��

*/


#pragma once


#include <windows.h>


#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//DirectX�K�C�h
#pragma comment ( lib, "dxguid.lib" )

//����
//DInput�̎g�p�o�[�W�����̐ݒ�
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")


//�T�E���h
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")
#include <dsound.h>
#include <mmsystem.h>

#include <map>

#include "Error.h"



//�Z�[�t�f���[�g
#define SAFE_DELETE(x) if(x != nullptr){delete x;x = nullptr;}

//�Z�[�t�����[�X
#define SAFE_RELEASE(x) if(x != NULL){ (x)->Release();(x) = NULL;}


//natsume�Z�[�t�����[�X
#define SAFE_RELEASE_NATSUME(x) if(x){ (x)->Release();(x) = NULL;}

//�V���O���g���C���X�^���X�̐����}�N��
#define CREATE_SINGLETON_INSTANCE(x) if(FAILED(x::CreateInstance())){Error("Error");}

//�V���O���g���C���X�^���X�̃C���X�^���X�擾
#define GET_INSTANCE(x) &x::GetInstance()

//�V���O���g���C���X�^���X�폜�}�N��
#define DESTROY_SINGLETON_INSTANCE(x) if(FAILED(x::DeleteInstance())){Error("Error");}





namespace NatsumeGrass
{
	namespace System
	{

		template <class T>
		class Natsume_Core_Single
		{
		public:
			static inline T &GetInstance(){
				return *Instance;
			};

			static inline HRESULT CreateInstance(){
				if(Instance == nullptr){Instance = new T(); return S_OK;}
				else{Error("Instance OverLap"); return E_FAIL;}
			};

			static inline HRESULT DeleteInstance(){
				if(Instance != nullptr){SAFE_DELETE(Instance); return S_OK;}
				else{Error("Instance nullDelete");return E_FAIL;}
			};

		protected:
			Natsume_Core_Single(){};
			virtual ~Natsume_Core_Single(){};
			
			static T *Instance;

		};

		template<class T>
		T *Natsume_Core_Single<T>::Instance = nullptr;




	};
};







