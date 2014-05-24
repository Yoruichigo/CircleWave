/*

	�T�E���h�֌W�w�b�_

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
			LPDIRECTSOUND8 LpDirectSound;	//IDirectSound8�̃|�C���^�^,�f�o�C�X�f�[�^�H
			std::map<UINT,IDirectSoundBuffer8*> SoundDataBuffer;
			UINT SoundDataBufferNextKey;		//�T�E���h�o�b�t�@�̎��̃L�[


			HRESULT Initialize();
			void Release();

			HRESULT LoadSoundWave(TCHAR*);

			HRESULT OpenWave( TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize );

		};


	};

};



