/*

	エントリーポイント

*/



#include "NatsumeGrass.h"





INT WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE,
				   LPSTR lpCmdLine,
				   INT nCmdShow)
{

	Sequence::SetScreenInfo(600,800,"CircleWave",true);

	if(FAILED(Sequence::Initialize(hInstance)))
	{
		return -1;
	}

	
	UINT SoundKey[5];
	ZeroMemory(&SoundKey,sizeof(SoundKey));
	SoundKey[0] = Sound::LoadSound("Data/トコハナ(常花).wav");
	SoundKey[1] = Sound::LoadSound("Data/トコハナ(常花).wav");
	SoundKey[2] = Sound::LoadSound("Data/トコハナ(常花).wav");
	SoundKey[3] = Sound::LoadSound("Data/トコハナ(常花).wav");
	
	
	Sound::Play(SoundKey[3]);
	

	CustomSquare A;


	{
		//メッセージループ
		MSG msg = {0};
		ZeroMemory(&msg,sizeof(msg));

		while(msg.message != WM_QUIT)
		{
				if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					Input::UpdateAcquire();
					if(Input::KeyCheckFrame(DIK_SPACE) == 1)
					{
						Sound::Stop(SoundKey[3]);
					}
					//メインループ
					Graphic::BeginRender();

					Graphic::CreateBox(A);

					Graphic::EndRender();
				}

		}

	}

	if(FAILED(Sequence::End()))
	{
		return -1;
	}

	return 0;
};



