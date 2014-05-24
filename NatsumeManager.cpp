/*

	Natsume�}�l�[�W���\�[�X

*/


#include "NatsumeManager.h"


// **********************
// �E�B���h�E�v���V�[�W��
// **********************

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return NatsumeGrass::External::Proc::MsgProc(hWnd,uMsg,wParam,lParam);
}





namespace NatsumeGrass
{
	namespace System
	{
		HINSTANCE			NatsumeManager::hInstance	=	nullptr;
		HWND				NatsumeManager::hWnd		=	nullptr;
		LPDIRECT3DDEVICE9	NatsumeManager::LpD3DDevice	=	nullptr;
		LPDIRECT3D9			NatsumeManager::LpD3D		=	nullptr;


		NatsumeManager::NatsumeManager()
		{




		}

		NatsumeManager::~NatsumeManager()
		{
			Release();

		}


		HRESULT NatsumeManager::Initialize(HINSTANCE _hInstance)
		{
			hInstance = _hInstance;

			if(FAILED(
				InitWindow(_hInstance)
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				InitD3D()
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				OtherInitialize()
				))
			{
				return E_FAIL;
			}

			return S_OK;
		}

		void NatsumeManager::Release()
		{
			OtherRelease();

			SAFE_RELEASE(LpD3D);
			SAFE_RELEASE(LpD3DDevice);
		}

		//�T�u�N���X�̏������ł��B
		HRESULT NatsumeManager::OtherInitialize()
		{
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::InputClass);
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::Polygon_2D);
			CREATE_SINGLETON_INSTANCE(NatsumeGrass::System::NatsumeSound);
			return S_OK;
		}

		//�T�u�N���X�̃����[�X�ł��B
		void NatsumeManager::OtherRelease()
		{
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::InputClass);
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::Polygon_2D);
			DESTROY_SINGLETON_INSTANCE(NatsumeGrass::System::NatsumeSound);

		}


		HRESULT NatsumeManager::InitWindow(HINSTANCE _hInstance)
		{
			//�E�B���h�E�^�C�g���̕ۑ�
			LPCSTR WindowName = AppTitleName;

			WNDCLASSEX wc;
			ZeroMemory(&wc,sizeof(wc));
			wc.cbSize = sizeof(wc);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = _hInstance;
			wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
			wc.lpszClassName = WindowName;
			wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
			RegisterClassEx(&wc);

			//�E�B���h�E�n���h���ɓo�^
			hWnd = CreateWindow(WindowName,WindowName,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				0,
				ScreenSizeWidth,
				ScreenSizeHeight,
				NULL,NULL,hInstance,NULL);

			if(! hWnd)
			{
				return E_FAIL;
			}
	
			//ValidateRect(hWnd,0);

			//�E�B���h�E�̕\��
			ShowWindow(hWnd,SW_SHOW);
			//UpdateWindow(hWnd);

			return S_OK;
		}

		HRESULT NatsumeManager::InitD3D()
		{
			
			LpD3D = NULL;
			LpD3DDevice = NULL;

			//�܂��ŏ���D3D9�̃R���|�[�l���g���擾���܂��B
			if(NULL == (
			LpD3D = Direct3DCreate9(D3D_SDK_VERSION)
				) )
			{
				Error("Direct3DCreate() FAILED");
				return E_FAIL;
			}

			D3DPRESENT_PARAMETERS D3DParam;
			//�p�����[�^�̐ݒ�
			D3DParam.BackBufferWidth = ScreenSizeWidth;
			D3DParam.BackBufferHeight = ScreenSizeHeight;

			//��ʂ̃t�H�[�}�b�g���A��R�̃��[�h������炵���̂Ŏg�������Ă݂�
			//D3DParam.BackBufferFormat = D3DFMT_A8R8G8B8;
			D3DParam.BackBufferFormat = D3DFMT_UNKNOWN;//�y�ȓz�H

			//�o�b�N�o�b�t�@�[�̐��A���ʁi�H�j�Ȏ������Ȃ��ꍇ1���ŗǂ��炵��
			//D3DParam.BackBufferCount = 2;
			D3DParam.BackBufferCount = 1;

			//�}���`�T���v���^�C�v�A�W���M�����A�A���`�G�C���A�X���|���鎖���o�����肷��B
			//�r�f�I�J�[�h���Ή����Ă��Ȃ��ꍇ������炵���̂�D3DMULTISAMPLE_NONE��
			//�f�t�H���g�ݒ肵�Ă������ق����ǂ��Ƃ̎��B
			//IDirect3D9::CheckDeviceMultiSampleType�֐��ŃA���`�G�C���A�X�̏����擾���鎖���\
			D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;
			//��̃}���`�T���v���^�C�v�̃N�I���e�B�A D3DMULTISAMPLE_NONE;
			//��ݒ肵���ꍇ��0�ō\��Ȃ��B��ɏ������֐��ł̈����ŕԂ����l�ƂȂ��Ă���
			D3DParam.MultiSampleQuality = 0;

			//SwapEffect�̓o�b�N�o�b�t�@�ƃt�����g�o�b�t�@��؂�ւ�����@���`���镨
			//D3DSWAPEFFECT_DISCARD���`����Ǝ����Ŕ��f���Ă���܂��B
			//�ł����A�A���t�@�u�����f�B���O���̐ݒ肪�ۏႳ��Ă��Ȃ��Ƃ���Ƃ�
			D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

			//��ʕ`�悷��E�B���h�E�n���h���̎w��ANULL�̏ꍇ���݃t�H�[�J�X���Ă���E�B���h�E���g�p
			D3DParam.hDeviceWindow = NULL;

			//TRUE�ŃE�B���h�E���[�h�AFALSE�Ńt���X�N���[��
			D3DParam.Windowed = WindowMode;

			//�[�x�X�e���V���o�b�t�@�̗L���̐ݒ�A�R�c�������̂Ȃ�TRUE��
			D3DParam.EnableAutoDepthStencil = TRUE;
			//��̃X�e���V���o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
			//D3DFORMAT�񋓌^�̃}�j���A��������Ƃ悢
			//D3DParam.AutoDepthStencilFormat = D3DFMT_D24X8;
			D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;

			//Flags�̓o�b�N�o�b�t�@����t�����g�o�b�t�@�ɓ]������Ƃ���
			//�@�\�̃I�v�V�����̐ݒ�A�Ƃ肠�����O�w��i��X�ǋL�\��j
			D3DParam.Flags = 0;

			//�t���X�N���[�����̃��t���b�V�����[�g�̎w��
			//�E�B���h�E���[�h�̏ꍇ�Ȃ�O�ɂ���K�v������A
			//�t���X�N���[�����ɂ��K���Ȓl�����邱�Ƃ͏o���Ȃ��B
			D3DParam.FullScreen_RefreshRateInHz =0;

			//�X���b�v�̏��������̃^�C�~���O�̐ݒ�
			//�ő��ɂ���ƃe�A�����O�����������蓙������Ɛݒ肵�Ă����Ȃ���΂Ȃ�Ȃ�
			D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			//D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			//D3DParam.PresentationInterval = 0;

			//�f�o�C�X�̐����A4���闝�R�͑O���ŃT�|�[�g���Ă��Ȃ����������ꍇ�̎����l���Ă̎�
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			if( FAILED( LpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &LpD3DDevice ) ) )
			{
				Release();
				return E_FAIL;
			}

			return S_OK;
		}



		//��ʂ̃N���A
		HRESULT NatsumeManager::ClearRender()
		{
			if(FAILED(
				LpD3DDevice->Clear(
				0,NULL,
				D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,//D3DCLEAR_TARGET,
				D3DCOLOR_XRGB(0,255,255),1.0f,0)
				))
			{
				return E_FAIL;
			}
			return S_OK;
		}

		//�`��J�n
		HRESULT NatsumeManager::BeginRender()
		{
			if(FAILED(
				LpD3DDevice->BeginScene()
				) )
			{
				return E_FAIL;
			}


			return S_OK;
		}

		//�`��I��
		HRESULT NatsumeManager::EndRender()
		{
			if(FAILED(
				LpD3DDevice->EndScene()
				))
			{
				return E_FAIL;
			}

			if(FAILED(
				LpD3DDevice->Present(NULL,NULL,NULL,NULL)
				) )
			{
				return E_FAIL;
			}

			return S_OK;
		}












	};
};

