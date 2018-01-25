#include "CWEInputManager.h"
#include "CWEKeyboard.h"
#include "CWEMouse.h"
#include "CWEGraphicManager.h"
#include "CWEError.h"
#include "CWEUsefullyMacro.h"



namespace CWE
{


//�R���X�g���N�^
Input::Input():
mpDevice(0), mPadsNum(0), mUsingPadsNum(0), mSettingPadNumber(0),
mKeyboard(Keyboard::GetInstance()),
mMouse(Mouse::GetInstance())
{
}

//�f�X�g���N�^
Input::~Input()
{
}

//�C���X�^���X�̎擾
Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}


//������
bool Input::Init()
{
	//DirectInput�̏�����
	if(FAILED(DirectInput8Create(Graphic::GetInstance().mWindows.mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mpDevice, NULL)))
	{
		(L"DirectInput�̏������Ɏ��s���܂����B");
		return false;
	}
	//�L�[�{�[�h�̏�����
	if(FAILED(mpDevice->CreateDevice(GUID_SysKeyboard, &mKeyboard.mpDevice, NULL)))
	{
		FatalError::GetInstance().Outbreak(L"�L�[�{�[�h�f�o�C�X�̏������Ɏ��s���܂����B\n�L�[�{�[�h���������ڑ�����Ă��邩�m�F���ĉ������B");
		return false;
	}
	if(!mKeyboard.Init()) {return false;}
	//�}�E�X�̏�����
	if(FAILED(mpDevice->CreateDevice(GUID_SysMouse, &mMouse.mpDevice, NULL)))
	{
		FatalError::GetInstance().Outbreak(L"�}�E�X�f�o�C�X�̏������Ɏ��s���܂����B\n�}�E�X���������ڑ�����Ă��邩�m�F���ĉ������B");
		return false;
	}
	if(!mMouse.Init(Graphic::GetInstance().mWindows.mMainWindow)) {return false;}
	//�Q�[���p�b�h�̓o�^
	GamepadRegister();

	return true;
}


//�Q�[���p�b�h�̓o�^
void Input::GamepadRegister()
{
	//�ꉞ�S�Ẵp�b�h�̏��𖕏����Ă���Đݒ肷��
	mPadsNum =0;
	for(i=0; i<CWE_GAMEPAD_MAX; ++i)
	{
		mGamePads[i].Clear();
	}

	//�f�o�C�X�̏�����
	for(i=0; i<CWE_GAMEPAD_MAX; ++i)
	{
		mpDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, callbacks::EnumGamePads, NULL, DIEDFL_ATTACHEDONLY);		
	}
}


//�I������
void Input::End()
{
	//�Q�[���p�b�h
	for(i=0; i<CWE_GAMEPAD_MAX; ++i)
	{
		mGamePads[i].End();
	}
	//�}�E�X
	mMouse.End();
	//�L�[�{�[�h
	mKeyboard.End();

	SAFE_RELEASE(mpDevice);
}


//�X�V����
void Input::Update()
{
	static Windows& core =Graphic::GetInstance().mWindows;
	
	//�A�N�e�B�u��ԂȂ珈�����s��
	if(core.mWinMsgState.mActive)
	{
		mKeyboard.Update();
		mMouse.Update();

		//�X�̃Q�[���p�b�h�̍X�V
		for(i=0; i<CWE_GAMEPAD_MAX; ++i)
		{
			mGamePads[i].Update();
		}
	}
}



//�Q�[���p�b�h�N���X�̏���񋟂���֐�
Gamepad* const Input::GetGamepad()
{
	++mUsingPadsNum;
	CWE_ASSERT(mUsingPadsNum-1 < CWE_GAMEPAD_MAX && "�Q�[���p�b�h�̐����������܂�");
	return &mGamePads[mUsingPadsNum-1];
}


//�擾�����Q�[���p�b�h�������
void Input::ReleaseGamepad(Gamepad* pGamePad)
{
	if(pGamePad)
	{
		--mUsingPadsNum;
	}
	pGamePad =0;
}


//�f�o�C�X�̐ڑ���ԂɕύX�����������Ƃ�m�点��
//true=�ύX���� false=�ύX�Ȃ�
bool Input::GetChangeConnection()
{
	if(mIsChangeConnection)
	{
		mIsChangeConnection =false;
		return true;
	}

	return false;
}



}//CWE