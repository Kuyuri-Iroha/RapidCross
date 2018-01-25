#include "CWEWindows.h"
#include "CWEError.h"
#include "CWEStatic.h"
#include "CWEMacroConstants.h"


namespace CWE
{


// �R���X�g���N�^
Windows::Windows()
{
	mIsWindowMode =true;
}

// �f�X�g���N�^
Windows::~Windows()
{
}


// ������
bool Windows::Init(const unsigned& width, const unsigned& height, const wchar_t windowTitle[128])
{
	// �������Ɏg�p����f�[�^�̃Z�b�g
	mInstance =GetModuleHandle(NULL);
	lstrcpy(mWindowTitle, windowTitle);
	lstrcpy(mClassName, mWindowTitle);
	mWindowSizeX =width;
	mWindowSizeY =height;
	mInitialWindowSizeX =mWindowSizeX;
	mInitialWindowSizeY =mWindowSizeY;
	mWindowSizeExRateX =mWindowSizeX/mInitialWindowSizeX;
	mWindowSizeExRateY =mWindowSizeY/mInitialWindowSizeY;

	//���d�N���̖h�~
	if(FindWindow(mClassName, mWindowTitle) != NULL)
	{
		return false;
	}

	//�X�N���[���Z�[�o�[������
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_SENDWININICHANGE);

	WNDCLASSEX wcx;

	wcx.cbSize =sizeof(wcx);
	wcx.style =(CS_HREDRAW | CS_VREDRAW);
	wcx.lpfnWndProc =callbacks::CWE_WndProc;
	wcx.cbClsExtra =0;
	wcx.cbWndExtra =0;
	wcx.hInstance =mInstance;
	wcx.hIcon =static_cast<HICON>(LoadImage(mInstance, (mIconID != 0) ? MAKEINTRESOURCE(mIconID) : IDI_APPLICATION, IMAGE_ICON, 0, 0, (LR_DEFAULTSIZE|LR_SHARED)));
	wcx.hCursor =static_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, (LR_DEFAULTSIZE|LR_SHARED)));
	wcx.hbrBackground =static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wcx.lpszMenuName =NULL;
	wcx.lpszClassName =mClassName;
	wcx.hIconSm =static_cast<HICON>(LoadImage(mInstance, (mIconID != 0) ? MAKEINTRESOURCE(mIconID) : IDI_APPLICATION, IMAGE_ICON, 0, 0, (LR_DEFAULTSIZE|LR_SHARED)));

	if(RegisterClassEx(&wcx) == 0)
	{
		FatalError::GetInstance().Outbreak(L"�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B");
		return false;
	}



	//�}�E�X�J�[�\����\��or��\��
	mIsMouseVisible =ResetMouseVisible(mIsWindowMode?true:false);

	//�E�C���h�E�̏����ʒu�̊m��
	int windowPlaceX =0,
		windowPlaceY =0;
	int monitorSizeX =GetSystemMetrics(SM_CXSCREEN),
		monitorSizeY =GetSystemMetrics(SM_CYSCREEN);
	if((mWindowSizeX <= monitorSizeX) && (mWindowSizeY <= monitorSizeY))
	{
		windowPlaceX =(monitorSizeX -mWindowSizeX) /2;
		windowPlaceY =(monitorSizeY -mWindowSizeY) /2;
	}

	//�E�C���h�E�̍쐬
	mMainWindow =CreateWindow(
						mClassName,
						mWindowTitle,
						CWE_WINDOW_MODE_STYLE, 
						windowPlaceX, 
						windowPlaceY, 
						mWindowSizeX, 
						mWindowSizeY,
						NULL,
						NULL, 
						mInstance, 
						NULL);

	//�E�B���h�E�T�C�Y�̒���
	RECT client;
	GetClientRect(mMainWindow, &client);
	MoveWindow(mMainWindow, windowPlaceX, windowPlaceY, mWindowSizeX+(mWindowSizeX-client.right), mWindowSizeY+(mWindowSizeY-client.bottom), TRUE);
	


	if(mMainWindow == NULL)
	{
		FatalError::GetInstance().Outbreak(L"�E�B���h�E�̍쐬�Ɏ��s���܂����B");
		return false;
	}

	ShowWindow(mMainWindow, SW_SHOW);
	UpdateWindow(mMainWindow);

	return true;
}


// �I������
void Windows::End()
{
	//�X�N���[���Z�[�o�[�L����
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, SPIF_SENDWININICHANGE);

	//�}�E�X���������Ă���
	ResetMouseVisible(true);
}


// ���b�Z�[�W����
void Windows::ProcessMessage()
{
	if(PeekMessage(&m_WinMessage, NULL, 0, 0, PM_REMOVE))
	{
		//WM_QUIT���b�Z�[�W�����s���ꂽ��I��
		if(mWinMsgState.mQuitMessage) 
		{
			EndRequest();
		}

		TranslateMessage(&m_WinMessage);//���b�Z�[�W��ϊ�
		DispatchMessage(&m_WinMessage);//���b�Z�[�W�𑗏o
	}
}



}// CWE