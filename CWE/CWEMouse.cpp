#include "CWEMouse.h"
#include "CWEGraphicManager.h"
#include "CWEStatic.h"
#include "CWEError.h"
#include "CWEUsefullyMacro.h"



namespace CWE
{


//�R���X�g���N�^
Mouse::Mouse():
mpDevice(0), mPointerMode(Graphic::GetInstance().mWindows.mIsMouseVisible)
{
}

//�f�X�g���N�^
Mouse::~Mouse()
{
}

//�C���X�^���X
Mouse& Mouse::GetInstance()
{
	static Mouse instance;
	return instance;
}


//����������
bool Mouse::Init(const HWND window)
{
	//�}�E�X�f�o�C�X�̃Z�b�g
	if(FAILED(mpDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		CWE::FatalError::GetInstance().Outbreak(L"�}�E�X�f�o�C�X�̏������Ɏ��s���܂����B\n�}�E�X���������Ȃ���Ă��邩�m�F���ĉ������B");
		return false;
	}
	if(FAILED(mpDevice->SetCooperativeLevel(window, (DISCL_NONEXCLUSIVE|DISCL_FOREGROUND))))
	{
		CWE::FatalError::GetInstance().Outbreak(L"�}�E�X�f�o�C�X�̏������Ɏ��s���܂����B\n�}�E�X���������Ȃ���Ă��邩�m�F���ĉ������B");
		return false;
	}

	//�}�E�X�̃w�b�_�[
	DIPROPDWORD aDIProp;
	aDIProp.diph.dwSize =sizeof(DIPROPDWORD);
	aDIProp.diph.dwHeaderSize =sizeof(DIPROPHEADER);
	aDIProp.diph.dwObj =0;
	aDIProp.diph.dwHow =DIPH_DEVICE;
	aDIProp.dwData =DIPROPAXISMODE_REL;
	if(FAILED(mpDevice->SetProperty(DIPROP_AXISMODE, &aDIProp.diph)))
	{
		CWE::FatalError::GetInstance().Outbreak(L"�}�E�X�f�o�C�X�̏������Ɏ��s���܂����B\n�}�E�X���������Ȃ���Ă��邩�m�F���ĉ������B");
		return false;
	}

	return true;
}


//�I������
void Mouse::End()
{
	SAFE_UNACQUIRE(mpDevice);
	SAFE_RELEASE(mpDevice);
}


//�X�V����
void Mouse::Update()
{
	static CWE::Windows& core =CWE::Graphic::GetInstance().mWindows;

	//�}�E�X���̎擾
	if(FAILED(mpDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mState)))
	{
		mpDevice->Acquire();
		mpDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mState);
	}

	//�}�E�X�J�[�\�����g��Ȃ��Ƃ��̏���
	if(!mPointerMode)
	{
		//���W�f�[�^�̑��
		mX =mState.lX;
		mY =mState.lY;
	}
	//����ȊO
	else
	{
		// �X�N���[����ł̃J�[�\���̈ʒu���擾
		GetCursorPos(&mMousePos);

		//�E�C���h�E���[�h�̏ꍇ�A���W���N���C�A���g�̈�ɍ��킹��
		if(core.mIsWindowMode)
		{
			mMousePos.x -=core.mClientPos.x;
			mMousePos.y -=core.mClientPos.y;

			
			mMousePos.x =(int)(mMousePos.x /core.mWindowSizeExRateX);
			mMousePos.y =(int)(mMousePos.y /core.mWindowSizeExRateY);
		}

		mX =mMousePos.x;
		mY =mMousePos.y;
	}

}


//�}�E�X�J�[�\�����g����������L���ɂ��邩�ǂ���
//�f�t�H���g�́u�g�p����v
bool Mouse::ResetPointerMode(const bool& usePointer)
{
	mPointerMode =usePointer;
	CWE::Graphic::GetInstance().mWindows.mIsMouseVisible =ResetMouseVisible(usePointer);

	//�}�E�X�̋������x���̃��Z�b�g
	SAFE_UNACQUIRE(mpDevice);
	if(FAILED(mpDevice->SetCooperativeLevel(CWE::Graphic::GetInstance().mWindows.mMainWindow, ((usePointer ? DISCL_NONEXCLUSIVE:DISCL_EXCLUSIVE)|DISCL_FOREGROUND))))
	{
		return false;
	}
	mpDevice->Acquire();
	return true;
}


//�����ꂽ�{�^���̔ԍ����擾
//true=�擾���� false=�擾���s
bool Mouse::GetInputButtonNum(InputCode& buttonNum)
{
	for(InputCode i; i.mInputCode<8; ++i.mInputCode)
	{
		if(Button(i))
		{
			buttonNum =i;
			return true;
		}
	}
	return false;
}


}//CWE