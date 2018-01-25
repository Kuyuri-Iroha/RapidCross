#include "CWEKeyboard.h"
#include "CWEGraphicManager.h"
#include "CWEError.h"
#include "CWEUsefullyMacro.h"


namespace CWE
{


//�R���X�g���N�^
Keyboard::Keyboard():
mpDevice(0)
{
}

//�f�X�g���N�^
Keyboard::~Keyboard()
{
}

//�C���X�^���X�̎擾
Keyboard& Keyboard::GetInstance()
{
	static Keyboard instance;
	return instance;
}


//����������
bool Keyboard::Init()
{
	if(FAILED(mpDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		FatalError::GetInstance().Outbreak(L"�L�[�{�[�h�f�o�C�X�̏������Ɏ��s���܂����B\n�L�[�{�[�h���������ڑ�����Ă��邩�m�F���ĉ������B");
		return false;
	}
	if(FAILED(mpDevice->SetCooperativeLevel(CWE::Graphic::GetInstance().mWindows.mMainWindow, (DISCL_FOREGROUND|DISCL_NONEXCLUSIVE))))
	{
		FatalError::GetInstance().Outbreak(L"�L�[�{�[�h�f�o�C�X�̏������Ɏ��s���܂����B\n�L�[�{�[�h���������ڑ�����Ă��邩�m�F���ĉ������B");
		return false;
	}
	ZeroMemory(mKeyState, 256);

	return true;
}


//�I������
void Keyboard::End()
{
	SAFE_UNACQUIRE(mpDevice);
	SAFE_RELEASE(mpDevice);
}


//�X�V����
void Keyboard::Update()
{
	if(FAILED(mpDevice->GetDeviceState(256, mKeyState)))
	{
		mpDevice->Acquire();
		mpDevice->GetDeviceState(256, mKeyState);
	}
}


//Window�L�[���{�^����L���ɂ��邩���Z�b�g����
//�Ԃ�l: true=���� false=���s
//����: true=�L�� false=����
bool Keyboard::ResetWindowsKey(bool windowsKey)
{
	unsigned flags =(DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	if(!windowsKey) {flags |=DISCL_NOWINKEY;}
	SAFE_UNACQUIRE(mpDevice);
	if(FAILED(mpDevice->SetCooperativeLevel(CWE::Graphic::GetInstance().mWindows.mMainWindow, flags)))
	{
		return false;
	}
	mpDevice->Acquire();
	return true;
}


//�����ꂽ�L�[�̃L�[�R�[�h���擾
//true=�擾���� false=�擾���s
bool Keyboard::GetInputKeyCode(InputCode& keyCode)
{
	for(InputCode i; i.mInputCode<256; ++i.mInputCode)
	{
		if(Key(i))
		{
			keyCode.mInputCode =i.mInputCode;
			return true;
		}
	}
	return false;
}


}//CWE