#include "CWEMouse.h"
#include "CWEInputManager.h"
#include "CWEGraphicManager.h"
#include "CWEUsefullyMacro.h"
#include "CWEStatic.h"


namespace CWE
{
namespace callbacks
{



//CWE�̃E�B���h�E�v���V�[�W��
extern LRESULT CALLBACK CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	using namespace CWE;

	Windows& window =Graphic::GetInstance().mWindows;
	Input& input =Input::GetInstance();
	Graphic& graphic =Graphic::GetInstance();


	int windowPlaceX =0;
	int windowPlaceY =0;
	int monitorSizeX =0;
	int monitorSizeY =0;

	//�E�B���h�E���b�Z�[�W����
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		window.mWinMsgState.mQuitMessage =true;
		break;

	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE) //��A�N�e�B�u��
		{
			window.mWinMsgState.mActive =false;
			window.mIsMouseVisible =ResetMouseVisible(true);
		}
		else
		{
			window.mWinMsgState.mActive =true;
			window.mIsMouseVisible =ResetMouseVisible(Mouse::GetInstance().mPointerMode);
		}
		//�������x���̃��Z�b�g
		if(Mouse::GetInstance().mpDevice)
		{
			SAFE_UNACQUIRE(Mouse::GetInstance().mpDevice);
			Mouse::GetInstance().mpDevice->SetCooperativeLevel(window.mMainWindow, (DISCL_NONEXCLUSIVE|DISCL_FOREGROUND));
			Mouse::GetInstance().mpDevice->Acquire();
		}
		break;

	case WM_MOVE:
		window.mClientPos.x =lParam & 0xFFFF;
		window.mClientPos.y =(lParam >>16) & 0xFFFF;
		break;

	case WM_SIZE:
		if((!Graphic::GetInstance().CheckDeviceIs()) | (wParam == SIZE_MINIMIZED)) {break;}
		Graphic::GetInstance().InnerResetWindowSize();
		Graphic::GetInstance().ResetBackBuffer();

		//�E�B���h�E�ʒu�̒���
		windowPlaceX =0,
		windowPlaceY =0;
		monitorSizeX =GetSystemMetrics(SM_CXSCREEN),
		monitorSizeY =GetSystemMetrics(SM_CYSCREEN);
		if((window.mWindowSizeX <= monitorSizeX) && (window.mWindowSizeY <= monitorSizeY))
		{
			windowPlaceX =(monitorSizeX -window.mWindowSizeX) /2;
			windowPlaceY =(monitorSizeY -window.mWindowSizeY) /2;
		}
		SetWindowPos(window.mMainWindow, NULL, windowPlaceX, windowPlaceY, NULL, NULL, (SWP_NOSIZE|SWP_NOZORDER));
		break;

	case WM_SYSKEYDOWN:
		//Windows API��DirectX11�̃V�X�e���L�[��}��
		switch(wParam)
		{
		case VK_RETURN:
			Graphic::GetInstance().InnerWindowModeStateRestoration();
			break;

		case VK_F10:
			break;
		}
		break;

	case WM_DEVICECHANGE:
		//�Q�[���p�b�h�̓o�^�󋵂����Z�b�g
		input.GamepadRegister();
		input.mIsChangeConnection =true;
		break;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// �Q�[���p�b�h�f�o�C�X�̗�
extern BOOL CALLBACK EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext)
{
	Input& input =Input::GetInstance();

	if(CWE_GAMEPAD_MAX <= input.mPadsNum)
	{
		return DIENUM_STOP;
	}

	//�o�^�ς݃f�o�C�X���ǂ���
	for(int i=0; i<=CWE_GAMEPAD_MAX; ++i)
	{
		if(input.mGamePads[i].mIdentifier == pDIDInstance->guidInstance)
		{
			return DIENUM_CONTINUE;
		}
	}

	if(input.mPadsNum < 4)
	{
		if(input.mGamePads[input.mPadsNum].XIInit(input.mPadsNum))
		{
			//���ʎq��o�^
			input.mGamePads[input.mPadsNum].mIdentifier =pDIDInstance->guidInstance;
			input.mGamePads[input.mPadsNum].mNumber =input.mPadsNum;
			++input.mPadsNum;

			return DIENUM_STOP;
		}
	}
	

	if(FAILED(input.mpDevice->CreateDevice(pDIDInstance->guidInstance, &input.mGamePads[input.mPadsNum].mpDevice, NULL)))
	{
		return DIENUM_CONTINUE;
	}

	//���ʎq��o�^
	input.mGamePads[input.mPadsNum].mIdentifier =pDIDInstance->guidInstance;
	input.mGamePads[input.mPadsNum].mNumber =input.mPadsNum;
		
	if(!input.mGamePads[input.mPadsNum].DIInit())
	{
		return DIENUM_CONTINUE;
	}

	++input.mPadsNum;

	return DIENUM_STOP;
}


//�A�i���O�X�e�B�b�N���̍ő�l�ŏ��l�̐ݒ�
extern BOOL CALLBACK EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext)
{
	Input& gamePad =Input::GetInstance();

	DIPROPRANGE diprop;
	diprop.diph.dwSize =sizeof(DIPROPRANGE);
	diprop.diph.dwHeaderSize =sizeof(DIPROPHEADER);
	diprop.diph.dwHow =DIPH_BYID;
	diprop.diph.dwObj =pDIDObInstance->dwType;
	diprop.lMin =-32768;
	diprop.lMax =32767;
	gamePad.mGamePads[gamePad.mPadsNum].mpDevice->SetProperty(DIPROP_RANGE, &diprop.diph);

	return DIENUM_CONTINUE;
}


//�f�b�h�]�[���̐ݒ�
extern BOOL CALLBACK EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext)
{
	Input& gamePad =Input::GetInstance();

	DIPROPDWORD diprop_w;
	diprop_w.diph.dwSize =sizeof(DIPROPDWORD);
	diprop_w.diph.dwHeaderSize =sizeof(DIPROPHEADER);
	diprop_w.diph.dwHow =DIPH_BYID;
	diprop_w.diph.dwObj =pDIDObInstance->dwType;
	diprop_w.dwData =gamePad.mGamePads[gamePad.mSettingPadNumber].mDeadZone.mPercentage;
	gamePad.mGamePads[gamePad.mSettingPadNumber].mpDevice->SetProperty(DIPROP_DEADZONE, &diprop_w.diph);

	return DIENUM_CONTINUE;
}


}//callbacks
}//CWE