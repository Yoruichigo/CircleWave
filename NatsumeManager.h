/*

	Natsume�}�l�[�W���w�b�_

*/


#pragma once


#include "NatsumeSystem.h"

#include "NatsumeSound.h"

#include "NatsumeExternal.h"



namespace NatsumeGrass
{
	namespace System
	{
		
		class NatsumeManager : public Natsume_Core_Single<NatsumeManager>
		{
		public:
			friend class Natsume_Core_Single<NatsumeManager>;

			static HINSTANCE hInstance;
			static HWND hWnd;
			static LPDIRECT3DDEVICE9 LpD3DDevice;
			static LPDIRECT3D9 LpD3D;


			//NatsumeManager������A�Ăяo���B
			HRESULT Initialize(HINSTANCE);

			HRESULT ClearRender();
			HRESULT BeginRender();
			HRESULT EndRender();


		private:
			NatsumeManager();
			~NatsumeManager();

			HRESULT InitWindow(HINSTANCE);
			HRESULT InitD3D();
			HRESULT OtherInitialize();

			void Release();
			void OtherRelease();

		};



	};
};



