/*

	サウンド関係ヘッダ

*/



#pragma once

#include "NatsumeSystem.h"

#include "NatsumeManager.h"


namespace NatsumeGrass
{
	namespace System
	{

		class NatsumeManager;

		class NatsumeSound : public Natsume_Core_Single<NatsumeSound>
		{
		public:
			friend class Natsume_Core_Single<NatsumeSound>;
			
			UINT SoundDataLoad(TCHAR *_FileName);

			void SoundPlay(UINT _Key);

		private:
			NatsumeSound();
			~NatsumeSound();
			LPDIRECTSOUND8 LpDirectSound;	//IDirectSound8のポインタ型,デバイスデータ？
			std::map<UINT,IDirectSoundBuffer8*> SoundDataBuffer;
			UINT SoundDataBufferNextKey;		//サウンドバッファの次のキー


			HRESULT Initialize();
			void Release();

			HRESULT LoadSoundWave(TCHAR*);

			HRESULT OpenWave( TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize );

		};


	};

};



