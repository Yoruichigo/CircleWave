/*

	�T�E���h�֌W


*/


#include "NatsumeSound.h"



namespace NatsumeGrass
{
	namespace System
	{

		NatsumeSound::NatsumeSound()
		{
			SoundDataBufferNextKey = 1;
			if(FAILED(Initialize()))
			{
				Release();
				Error("DirectSound Initialize Failed");
			}
		}

		NatsumeSound::~NatsumeSound()
		{
			Release();
		}

		void NatsumeSound::Release()
		{
			//�S�ẴT�E���h�f�[�^�̃����[�X
			std::map<UINT,IDirectSoundBuffer8*>::iterator it;
			for(it = SoundDataBuffer.begin();it != SoundDataBuffer.end();it++)
			{
				SAFE_RELEASE( ((*it).second) );
			}
			SoundDataBuffer.clear();


			SAFE_RELEASE(LpDirectSound);

		}


		HRESULT NatsumeSound::Initialize()
		{

			// �T�E���h�f�o�C�X�쐬
			//IDirectSound8 *pDS8;
			if(FAILED(
				DirectSoundCreate8(NULL, &LpDirectSound, NULL)
				))
			{
				return E_FAIL;
			}
			
			//�������x���̐ݒ�
			if(FAILED(
			LpDirectSound->SetCooperativeLevel(
				NatsumeManager::hWnd,
				DSSCL_PRIORITY)
				))
			{
				return E_FAIL;
			}
			// ****!!
			//	�����A���������ɃZ�J���_���o�b�t�@�̐����͂��Ȃ��A
			//�@���y�����[�h����Ƃ��ɃZ�J���_���o�b�t�@�̐������s��
			// ****!!
			/*
			DSBUFFERDESC DSBufferDesc;
			DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
			DSBufferDesc.dwFlags = 0;
			DSBufferDesc.dwBufferBytes = waveSize;
			DSBufferDesc.dwReserved = 0;
			DSBufferDesc.lpwfxFormat = &wFmt;
			DSBufferDesc.guid3DAlgorithm = GUID_NULL;

			IDirectSoundBuffer *ptmpBuf = 0;
			IDirectSoundBuffer8 *pDSBuffer;
			LpDirectSound->CreateSoundBuffer( 
			&DSBufferDesc,
			&ptmpBuf,
			NULL );
			ptmpBuf->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&pDSBuffer );
			ptmpBuf->Release();
			if ( pDSBuffer == 0 ) 
			{
					return E_FAIL;
			}*/
			


			{/*
				//�Z�J���_���o�b�t�@�̍쐬

				//�Z�J���_���o�b�t�@���쐬����ɂ������āA�K�v�ȕ���
				//DSBUFFERDESC�\���̂��K�v
				DSBUFFERDESC DSBufferDesc;
				//���g��
				/*
				typedef struct {

				   DWORD dwSize;		dwSize�͂��̍\���̂̑傫���ł��Bsizeof���Z�q�ŗ^���Ă��������B
				   DWORD dwFlags;		dwFlags�̓f�o�C�X�̕t���\�͂��w�肷��t���O
				   DWORD dwBufferBytes;	dwBufferByttes�̓Z�J���_���o�b�t�@�̃T�C�Y�Ńo�C�g�P�ʂŎw�肵�܂��B
										�����Ŏw�肵���傫���̃��������m�ۂ���܂��B�ǂ������傫���ɂ���ׂ����͏������݂���g�`�f�[�^�ɂ���ĈقȂ�܂��B
				   DWORD dwReserved;	dwReserved�͗\����0�ɂ��܂��B
				   LPWAVEFORMATEX lpwfxFormat;	lpwfxFormat��WAV�̃t�H�[�}�b�g��WAVEFORATEX�\���̂Ŏw�肵�܂�
				   GUID guid3DAlgorithm;		guid3DAlgorithm�͎g�p���鉼�z3D�G�t�F�N�g��GUID��ݒ肷��̂ł����A�G�t�F�N�g���g�p���Ȃ�����GUID_NULL�ɂ��܂��B

				} DSBUFFERDESC, *LPDSBUFFERDESC; 

				5�i�ڂɂ���WAVEFORMATEX�\���̂�WAV�t�H�[�}�b�g�Ɋ܂܂�鉹����`�����l�����i���m�����Ƃ��X�e���I�̎��ł��j�̏����i�[����\����
				typedef struct {

					WORD  wFormatTag;		wFormatTag�̓E�F�[�u�t�H�[���I�[�f�B�I�t�H�[�}�b�g�̃^�C�v���w�肵�܂��B������I�[�f�B�I�̈��k�Z�p�̃t�H�[�}�b�g�^�O���w�肷��̂ł����A�񈳏k�ŗǂ��ꍇ��WAVE_FORMAT_PCM��^���܂��B
					WORD  nChannels;		nChannnels�͒ʏ�̓X�e���I�E���m�����̑I���ł��B�X�e���I�Ȃ�΍��E�̃X�s�[�J�[����Ɨ����������Ȃ�̂Ń`�����l������2�ɂȂ�܂��B
					DWORD nSamplesPerSec;	nSamplesPerSec�̓T���v�����O���[�g�i�����j��Hz�P�ʂŎw�肵�܂��B�T�E���h�{�[�h�ɂ���Ĕ\�͂��قȂ�܂����ACD���Đ��ł���Ȃ�44100(Hz)�������܂�
					DWORD nAvgBytesPerSec;	nAvgBytesPerSec�͕��σf�[�^�]�����x���w�肷��̂ł����AwFormatTag��WAVE_FORMAT_PCM�i�񈳏k�g�`�j���w�肳��Ă���ꍇ��nSamplePerSec*nBlockAline�ɂȂ�܂��B
					WORD  nBlockAlign;		nBlockAline��wFormatTag�Ŏw�肵���t�H�[�}�b�g�`���̍ŏ��P�ʁi�o�C�g�P�ʁj���w�肵�܂��BwFormatTag��WAVE_FORMAT_PCM���w�肵���ꍇ��nChannels*wBitsPerSample/8�ɂȂ�܂��B
					WORD  wBitsPerSample;	wBitsPerSamples�̓T���v�����O�r�b�g���w�肵�܂��B�T���v�����O�r�b�g�Ƃ��Ă�8, 16, 32�Ȃǂ��w��ł��܂��B�����傫���قǃN���A�ȉ����ɂȂ�܂��B
					WORD  cbSize;			cbSize�͂��̍\���̂̌�ɕt���I�ȏ�񂪂���ꍇ�ɂ��̃T�C�Y���w�肵�܂��BwFormatTag��WAVE_FORMAT_PCM�̏ꍇ�͖��������̂�0�����Ă����܂��B

				} WAVEFORMATEX;


				���̐ݒ�����邱�ƂŁA�b�c���̃N���A�ȉ����̐ݒ���\
					WAVEFORMATEX wFmt;
					wFmt.wFormatTag = WAVE_FORMAT_PCM;
					wFmt.nChannels = 2;
					wFmt.nSamplesPerSec = 44100;
					wFmt.wBitsPerSample = 16;
					wFmt.nBlockAlign = wFmt.nChannels * wFmt.wBitsPerSample / 8;
					wFmt.nAvgBytesPerSec = wFmt.nSamplesPerSec * wFmt.nBlockAlign;
					wFmt.cbSize = 0;

				�������A�����̏���WAV�t�@�C�����璼�ړǂݎ���̂Ŏ�ł�����K�v���͂Ȃ�
				���̏���DSBUFFERDESC::lpwfxFormat�ɓn���܂�


				 * /

				IDirectSoundBuffer *ptmpBuf = 0;
				IDirectSoundBuffer8 *pDSBuffer;

				LpDirectSound->CreateSoundBuffer(
					&DSBufferDesc,
					&ptmpBuf,
					NULL);
				ptmpBuf->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&pDSBuffer );
				ptmpBuf->Release();

			*/}


			return S_OK;
		}


		UINT NatsumeSound::SoundDataLoad(TCHAR *_FileName)
		{
			if(FAILED(LoadSoundWave(_FileName)))
			{
				Error("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
				return 0;
			}

			UINT ReturnKey = SoundDataBufferNextKey;

			SoundDataBufferNextKey ++;



			return ReturnKey;
		}

		void NatsumeSound::SoundPlay(UINT _Key)
		{
			if(_Key == NULL)
			{
				Error("�T�E���h�L�[�������ł��B");
			}
			SoundDataBuffer[_Key]->Play(0,0,0);
		}


		HRESULT NatsumeSound::LoadSoundWave(TCHAR *_FileName)
		{
			// Wave�t�@�C���I�[�v��
			WAVEFORMATEX wFmt;
			char *pWaveData = 0;
			DWORD WaveSize = 0;
			if(FAILED(
				OpenWave(_FileName,wFmt,&pWaveData,WaveSize)
				))
			{
				Error("Wave�t�@�C���̃��[�h�Ɏ��s���܂����B");
				return E_FAIL;
			}

			DSBUFFERDESC DSBufferDesc;
			DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
			DSBufferDesc.dwFlags = 0;
			DSBufferDesc.dwBufferBytes = WaveSize;
			DSBufferDesc.dwReserved = 0;
			DSBufferDesc.lpwfxFormat = &wFmt;
			DSBufferDesc.guid3DAlgorithm = GUID_NULL;


			IDirectSoundBuffer *ptmpBuf = 0;

			LpDirectSound->CreateSoundBuffer( &DSBufferDesc, &ptmpBuf, NULL );
			ptmpBuf->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&SoundDataBuffer[SoundDataBufferNextKey] );
			ptmpBuf->Release();
			if ( SoundDataBuffer[SoundDataBufferNextKey] == 0 )
			{
				//pDS8->Release();
				return E_FAIL;
			}

			// �Z�J���_���o�b�t�@��Wave�f�[�^��������
			LPVOID lpvWrite = 0;
			DWORD dwLength = 0;
			if ( DS_OK == SoundDataBuffer[SoundDataBufferNextKey]->Lock( 0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER ) )
			{
				memcpy( lpvWrite, pWaveData, dwLength);
				SoundDataBuffer[SoundDataBufferNextKey]->Unlock( lpvWrite, dwLength, NULL, 0);
			}
			delete[] pWaveData; // �����͂�������Ȃ�

			return S_OK;
		}


		HRESULT NatsumeSound::OpenWave( TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize )
		{
			if ( filepath == 0 )
			{
				return E_FAIL;
			}
			HMMIO hMmio = NULL;
			MMIOINFO mmioInfo;

			// Wave�t�@�C���I�[�v��
			memset( &mmioInfo, 0, sizeof(MMIOINFO) );
			hMmio = mmioOpen( filepath, &mmioInfo, MMIO_READ );
			if( !hMmio )
			{
				return E_FAIL; // �t�@�C���I�[�v�����s
			}
			// RIFF�`�����N����
			MMRESULT mmRes;
			MMCKINFO riffChunk;
			riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			mmRes = mmioDescend( hMmio, &riffChunk, NULL, MMIO_FINDRIFF );
			if( mmRes != MMSYSERR_NOERROR )
			{
					mmioClose( hMmio, 0 );
					return E_FAIL;
			}

			// �t�H�[�}�b�g�`�����N����
			MMCKINFO formatChunk;
			formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
			mmRes = mmioDescend( hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK );
			if( mmRes != MMSYSERR_NOERROR )
			{
					mmioClose( hMmio, 0 );
					return E_FAIL;
			}
			DWORD fmsize = formatChunk.cksize;
			DWORD size = mmioRead( hMmio, (HPSTR)&waveFormatEx, fmsize );
			if( size != fmsize )
			{
					mmioClose( hMmio, 0 );
					return E_FAIL;
			}

			mmioAscend( hMmio, &formatChunk, 0 );

			// �f�[�^�`�����N����
			MMCKINFO dataChunk;
			dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
			mmRes = mmioDescend( hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK );
			if( mmRes != MMSYSERR_NOERROR )
			{
					mmioClose( hMmio, 0 );
					return E_FAIL;
			}
			*ppData = new char[ dataChunk.cksize ];
			size = mmioRead( hMmio, (HPSTR)*ppData, dataChunk.cksize );
			if(size != dataChunk.cksize )
			{
					delete[] *ppData;
					return E_FAIL;
			}
			dataSize = size;

			// �n���h���N���[�Y
			mmioClose( hMmio, 0 );

			return S_OK;
		}


	};

};




