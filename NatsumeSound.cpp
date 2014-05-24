/*

	サウンド関係


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
			//全てのサウンドデータのリリース
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

			// サウンドデバイス作成
			//IDirectSound8 *pDS8;
			if(FAILED(
				DirectSoundCreate8(NULL, &LpDirectSound, NULL)
				))
			{
				return E_FAIL;
			}
			
			//強調レベルの設定
			if(FAILED(
			LpDirectSound->SetCooperativeLevel(
				NatsumeManager::hWnd,
				DSSCL_PRIORITY)
				))
			{
				return E_FAIL;
			}
			// ****!!
			//	多分、初期化時にセカンダリバッファの生成はしない、
			//　音楽をロードするときにセカンダリバッファの生成を行う
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
				//セカンダリバッファの作成

				//セカンダリバッファを作成するにあたって、必要な物は
				//DSBUFFERDESC構造体が必要
				DSBUFFERDESC DSBufferDesc;
				//中身は
				/*
				typedef struct {

				   DWORD dwSize;		dwSizeはこの構造体の大きさです。sizeof演算子で与えてください。
				   DWORD dwFlags;		dwFlagsはデバイスの付加能力を指定するフラグ
				   DWORD dwBufferBytes;	dwBufferByttesはセカンダリバッファのサイズでバイト単位で指定します。
										ここで指定した大きさのメモリが確保されます。どういう大きさにするべきかは書き込みする波形データによって異なります。
				   DWORD dwReserved;	dwReservedは予約域で0にします。
				   LPWAVEFORMATEX lpwfxFormat;	lpwfxFormatはWAVのフォーマットをWAVEFORATEX構造体で指定します
				   GUID guid3DAlgorithm;		guid3DAlgorithmは使用する仮想3DエフェクトのGUIDを設定するのですが、エフェクトを使用しない時はGUID_NULLにします。

				} DSBUFFERDESC, *LPDSBUFFERDESC; 

				5段目にあるWAVEFORMATEX構造体はWAVフォーマットに含まれる音質やチャンネル数（モノラルとかステレオの事です）の情報を格納する構造体
				typedef struct {

					WORD  wFormatTag;		wFormatTagはウェーブフォームオーディオフォーマットのタイプを指定します。いわゆるオーディオの圧縮技術のフォーマットタグを指定するのですが、非圧縮で良い場合はWAVE_FORMAT_PCMを与えます。
					WORD  nChannels;		nChannnelsは通常はステレオ・モノラルの選択です。ステレオならば左右のスピーカーから独立した音がなるのでチャンネル数が2になります。
					DWORD nSamplesPerSec;	nSamplesPerSecはサンプリングレート（音質）をHz単位で指定します。サウンドボードによって能力が異なりますが、CDが再生できるなら44100(Hz)を扱えます
					DWORD nAvgBytesPerSec;	nAvgBytesPerSecは平均データ転送速度を指定するのですが、wFormatTagにWAVE_FORMAT_PCM（非圧縮波形）が指定されている場合はnSamplePerSec*nBlockAlineになります。
					WORD  nBlockAlign;		nBlockAlineはwFormatTagで指定したフォーマット形式の最小単位（バイト単位）を指定します。wFormatTagにWAVE_FORMAT_PCMを指定した場合はnChannels*wBitsPerSample/8になります。
					WORD  wBitsPerSample;	wBitsPerSamplesはサンプリングビットを指定します。サンプリングビットとしては8, 16, 32などを指定できます。数が大きいほどクリアな音質になります。
					WORD  cbSize;			cbSizeはこの構造体の後に付加的な情報がある場合にそのサイズを指定します。wFormatTagがWAVE_FORMAT_PCMの場合は無視されるので0を入れておきます。

				} WAVEFORMATEX;


				この設定をすることで、ＣＤ並のクリアな音質の設定も可能
					WAVEFORMATEX wFmt;
					wFmt.wFormatTag = WAVE_FORMAT_PCM;
					wFmt.nChannels = 2;
					wFmt.nSamplesPerSec = 44100;
					wFmt.wBitsPerSample = 16;
					wFmt.nBlockAlign = wFmt.nChannels * wFmt.wBitsPerSample / 8;
					wFmt.nAvgBytesPerSec = wFmt.nSamplesPerSec * wFmt.nBlockAlign;
					wFmt.cbSize = 0;

				ただし、これらの情報はWAVファイルから直接読み取れるので手打ちする必要性はない
				この情報をDSBUFFERDESC::lpwfxFormatに渡します


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
				Error("ファイルの読み込みに失敗しました。");
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
				Error("サウンドキーが無効です。");
			}
			SoundDataBuffer[_Key]->Play(0,0,0);
		}


		HRESULT NatsumeSound::LoadSoundWave(TCHAR *_FileName)
		{
			// Waveファイルオープン
			WAVEFORMATEX wFmt;
			char *pWaveData = 0;
			DWORD WaveSize = 0;
			if(FAILED(
				OpenWave(_FileName,wFmt,&pWaveData,WaveSize)
				))
			{
				Error("Waveファイルのロードに失敗しました。");
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

			// セカンダリバッファにWaveデータ書き込み
			LPVOID lpvWrite = 0;
			DWORD dwLength = 0;
			if ( DS_OK == SoundDataBuffer[SoundDataBufferNextKey]->Lock( 0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER ) )
			{
				memcpy( lpvWrite, pWaveData, dwLength);
				SoundDataBuffer[SoundDataBufferNextKey]->Unlock( lpvWrite, dwLength, NULL, 0);
			}
			delete[] pWaveData; // 元音はもういらない

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

			// Waveファイルオープン
			memset( &mmioInfo, 0, sizeof(MMIOINFO) );
			hMmio = mmioOpen( filepath, &mmioInfo, MMIO_READ );
			if( !hMmio )
			{
				return E_FAIL; // ファイルオープン失敗
			}
			// RIFFチャンク検索
			MMRESULT mmRes;
			MMCKINFO riffChunk;
			riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			mmRes = mmioDescend( hMmio, &riffChunk, NULL, MMIO_FINDRIFF );
			if( mmRes != MMSYSERR_NOERROR )
			{
					mmioClose( hMmio, 0 );
					return E_FAIL;
			}

			// フォーマットチャンク検索
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

			// データチャンク検索
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

			// ハンドルクローズ
			mmioClose( hMmio, 0 );

			return S_OK;
		}


	};

};




