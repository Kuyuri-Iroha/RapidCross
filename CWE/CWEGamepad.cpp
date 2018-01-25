#include "CWEInputManager.h"
#include "CWEGamepad.h"
#include "CWEGraphicManager.h"
#include "CWEError.h"
#include "CWEUsefullyMacro.h"


namespace CWE
{


//�Q�[���p�b�h�p�̃R�[���o�b�N�֐��̐錾
extern BOOL CALLBACK callbacks::EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext);
extern BOOL CALLBACK callbacks::EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
extern BOOL CALLBACK callbacks::EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);


//========================== �X�̃Q�[���p�b�h =============================


//�R���X�g���N�^
Gamepad::Gamepad():
mNumber(-1),
mCanUse(false), mIsXInput(false), mCanUseEffect(false),
mpDevice(0), mpDIState(0), mpDIEffect(0), mpDIEffectState(0)
{
	mDeadZone.mValue =XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	mDeadZone.mPercentage =3000;
	mXIEffect.mState.wLeftMotorSpeed =65535;
	mXIEffect.mState.wRightMotorSpeed =65535;
	mXIEffect.mRunning =false;
}

//�f�X�g���N�^
Gamepad::~Gamepad()
{
}


//DirectInput���̏�����
bool Gamepad::DIInit()
{
	//�t�H�[�}�b�g�̃Z�b�g
	if(FAILED(mpDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return false;
	}
	//�������x���̃Z�b�g
	if(FAILED(mpDevice->SetCooperativeLevel(CWE::Graphic::GetInstance().mWindows.mMainWindow, (DISCL_EXCLUSIVE|DISCL_FOREGROUND))))
	{
		return false;
	}
	//���̍ő�l�E�ŏ��l�̃Z�b�g
	if(FAILED(mpDevice->EnumObjects(callbacks::EnumGamePadsFunction, CWE::Graphic::GetInstance().mWindows.mMainWindow, DIDFT_AXIS)))
	{
		return false;
	}
	//�ݒ肷��Q�[���p�b�h�̔ԍ����Z�b�g
	CWE::Input::GetInstance().mSettingPadNumber =mNumber;
	//�f�b�h�]�[���̃Z�b�g
	if(FAILED(mpDevice->EnumObjects(callbacks::EnumGamePadsFunctionForSetDeadZone, CWE::Graphic::GetInstance().mWindows.mMainWindow, DIDFT_AXIS)))
	{
		return false;
	}

	//�G�t�F�N�g�̍쐬
	mpDIEffectState =new DIEFFECT;

	DWORD axes[2] ={DIJOFS_X, DIJOFS_Y};
	LONG direction[2] ={0, 0};
	DICONSTANTFORCE cf;

	cf.lMagnitude =10000;
	mpDIEffectState->dwSize                  =sizeof( DIEFFECT );
	mpDIEffectState->dwFlags                 =DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	mpDIEffectState->dwDuration              =INFINITE;
	mpDIEffectState->dwSamplePeriod          =0;
	mpDIEffectState->dwGain                  =DI_FFNOMINALMAX;
	mpDIEffectState->dwTriggerButton         =DIEB_NOTRIGGER;
	mpDIEffectState->dwTriggerRepeatInterval =0;
	mpDIEffectState->cAxes                   =2;
	mpDIEffectState->rgdwAxes                =axes;
	mpDIEffectState->rglDirection            =direction;
	mpDIEffectState->lpEnvelope              =0;
	mpDIEffectState->cbTypeSpecificParams    =sizeof(DICONSTANTFORCE);
	mpDIEffectState->lpvTypeSpecificParams   =&cf;
	mpDIEffectState->dwStartDelay            =0;

	mpDevice->CreateEffect(GUID_ConstantForce, mpDIEffectState, &mpDIEffect, NULL);
	if(mpDIEffect != NULL) {mCanUseEffect =true;}
	
	mCanUse =true;
	mIsXInput =false;
	mpDIState =new DIJOYSTATE;

	mpDevice->Acquire();

	return true;
}


//XInput���̏�����
bool Gamepad::XIInit(const unsigned& padNum)
{
	if(XInputGetState(padNum, &mXIState) == ERROR_SUCCESS)
	{
		mIsXInput =true;
		mCanUse =true;
		mCanUseEffect =true;
		return true;
	}
	else
	{
		mIsXInput =false;
		mCanUse =false;
		mCanUseEffect =false;
		return false;
	}
}


//������񖕏�����
void Gamepad::Clear()
{
	ZeroMemory(&mIdentifier, sizeof(mIdentifier));
	ZeroMemory(&mXIState, sizeof(mXIState));
	mNumber =-1;
	mCanUse =false;
	mIsXInput =false;

	End();
}


//�I������
void Gamepad::End()
{
	SAFE_DELETE(mpDIEffectState);
	SAFE_DELETE(mpDIState);
	SAFE_UNACQUIRE(mpDevice);
	SAFE_RELEASE(mpDevice);
}


//�X�V����
void Gamepad::Update()
{
	//�g�p�\�Ȃ�
	if(mCanUse)
	{
		//XInput�Ȃ�
		if(mIsXInput)
		{
			//��Ԃ̎擾
			XInputGetState(mNumber, &mXIState);
			//�f�b�h�]�[��
			if( (mXIState.Gamepad.sThumbLX < mDeadZone.mValue && mXIState.Gamepad.sThumbLX > -mDeadZone.mValue)
				&& (mXIState.Gamepad.sThumbLY < mDeadZone.mValue && mXIState.Gamepad.sThumbLY > -mDeadZone.mValue) )
			{	
				mXIState.Gamepad.sThumbLX = 0;
				mXIState.Gamepad.sThumbLY = 0;
			}
			if((mXIState.Gamepad.sThumbRX < mDeadZone.mValue && mXIState.Gamepad.sThumbRX > -mDeadZone.mValue)
				&& (mXIState.Gamepad.sThumbRY < mDeadZone.mValue && mXIState.Gamepad.sThumbRY > -mDeadZone.mValue) ) 
			{
				mXIState.Gamepad.sThumbRX = 0;
				mXIState.Gamepad.sThumbRY = 0;
			}
			//��Ԃ�ۑ�
			mState.mAxis[0] =mXIState.Gamepad.sThumbLX;
			mState.mAxis[1] =mXIState.Gamepad.sThumbLY*-1;
			mState.mAxis[2] =mXIState.Gamepad.sThumbRX;
			mState.mAxis[3] =mXIState.Gamepad.sThumbRY*-1;
			mState.mLeftTrigger =(64 < mXIState.Gamepad.bLeftTrigger);
			mState.mRightTrigger =(64 < mXIState.Gamepad.bRightTrigger);
			mState.mPOV_Up =(mXIState.Gamepad.wButtons & 0x0001) != 0;
			mState.mPOV_Down =(mXIState.Gamepad.wButtons & 0x0002) != 0;
			mState.mPOV_Left =(mXIState.Gamepad.wButtons & 0x0004) != 0;
			mState.mPOV_Right =(mXIState.Gamepad.wButtons & 0x0008) != 0;

			//�G�t�F�N�g�̎����I��
			if(mXIEffect.mRunning)
			{
				mEffectTimer.Update();
				if(mEffectTimer.TimeIsPassed())
				{
					EffectStop();
				}
			}
		}
		//DirectInput�Ȃ�
		else
		{
			//��Ԃ̎擾
			mpDevice->Poll();
			if(FAILED(mpDevice->GetDeviceState(sizeof(DIJOYSTATE), mpDIState)))
			{
				mpDevice->Acquire();
				mpDevice->GetDeviceState(sizeof(DIJOYSTATE), mpDIState);
			}
			//��Ԃ̕ۑ�
			mState.mAxis[0] =mpDIState->lX;
			mState.mAxis[1] =mpDIState->lY;
			mState.mAxis[2] =mpDIState->lZ;
			mState.mAxis[3] =mpDIState->lRx;
			mState.mAxis[4] =mpDIState->lRy;
			mState.mAxis[5] =mpDIState->lRz;
			//�����R���g���[���[�t���O�̏�����
			mState.mPOV_Up =false;
			mState.mPOV_Down =false;
			mState.mPOV_Left =false;
			mState.mPOV_Right =false;
			//�����R���g���[���[�t���O
			switch(mpDIState->rgdwPOV[0])
			{
			case 0:
				mState.mPOV_Up =true;
				break;

			case 4500:
				mState.mPOV_Up =true;
				mState.mPOV_Right =true;
				break;

			case 9000:
				mState.mPOV_Right =true;
				break;

			case 13500:
				mState.mPOV_Right =true;
				mState.mPOV_Down =true;
				break;

			case 18000:
				mState.mPOV_Down =true;
				break;

			case 22500:
				mState.mPOV_Down =true;
				mState.mPOV_Left =true;
				break;

			case 27000:
				mState.mPOV_Left =true;
				break;

			case 31500:
				mState.mPOV_Left =true;
				mState.mPOV_Up =true;
				break;
			}
		}
	}
}


//�R���g���[���[�̃f�b�h�]�[����ݒ肷��
//percentage: 0�`10000
//5000�Ȃ�50%
void Gamepad::ResetDeadZone(unsigned percentage)
{
	if(percentage < 0) {percentage =0;}
	if(10000 < percentage) {percentage =10000;}
	mDeadZone.mValue =static_cast<int>(CWE_GAMEPAD_AXIS_MAX*(percentage*0.0001));
	mDeadZone.mPercentage =percentage;
	//DirectInput�Ȃ�
	if(!mIsXInput)
	{
		//�ݒ肷��Q�[���p�b�h�̔ԍ����Z�b�g
		CWE::Input::GetInstance().mSettingPadNumber =mNumber;
		//�f�b�h�]�[���̃Z�b�g
		mpDevice->EnumObjects(callbacks::EnumGamePadsFunctionForSetDeadZone, CWE::Graphic::GetInstance().mWindows.mMainWindow, DIDFT_AXIS);
	}
}


//�R���g���[���[�̐U���G�t�F�N�g��ݒ肷��
//true=���� false=���s
//�f�o�C�X���Ή����Ă��Ȃ��ꍇ��false
bool Gamepad::ResetEffect(const GamepadEffect& effect)
{
	//�U���G�t�F�N�g���g���Ȃ��ꍇ��l�������ȂƂ��͖ⓚ���p��false
	if((!mCanUseEffect) || (effect.mDuration<=0.0) || ((effect.mMagnitude<0)||(10000<effect.mMagnitude))) {return false;}

	//XInput�Ȃ�
	if(mIsXInput)
	{
		mXIEffect.mState.wLeftMotorSpeed =static_cast<WORD>(65535*(effect.mMagnitude*0.0001));
		mXIEffect.mState.wRightMotorSpeed =static_cast<WORD>(65535*(effect.mMagnitude*0.0001));
		mEffectTimer.Set(effect.mDuration);

		if(mXIEffect.mRunning) {XInputSetState(mNumber, &mXIEffect.mState);}
	}
	//DirectInput�Ȃ�
	else
	{
		DICONSTANTFORCE cf;

		cf.lMagnitude =effect.mMagnitude;
		mpDIEffectState->dwDuration =effect.mInfinite ? INFINITE : (DWORD)(effect.mDuration*DI_SECONDS);
		mpDIEffectState->cbTypeSpecificParams =sizeof(DICONSTANTFORCE);
		mpDIEffectState->lpvTypeSpecificParams =&cf;

		if(FAILED(mpDIEffect->SetParameters(mpDIEffectState, (DIEP_DURATION|DIEP_TYPESPECIFICPARAMS))))
		{
			return false;
		}
	}


	return true;
}


//�����H��XInput�̏����擾����
void Gamepad::GetRawXIState(XIState& state)
{
	state =mXIState;
}


//�����H��DirectInput�̏����擾����
//true=�擾���� false=�擾���s
bool Gamepad::GetRawDIState(DIState& state)
{
	if(mpDIState)
	{
		state =*mpDIState;
		return true;
	}

	return false;
}


//�C�ӂ̃{�^�����̎擾(XInput)
//true=������Ă��� false=������Ă��Ȃ�
bool Gamepad::Button(const XIPadButton& button)
{
	//�g�p�\�Ȃ�
	if(mCanUse)
	{
		//XInput�Ȃ�
		if(mIsXInput)
		{
			return (mXIState.Gamepad.wButtons & button) != 0;
		}
	}

	return false;
}


//�C�ӂ̃{�^�����̎擾(DirectInput)
//true=������Ă��� false=������Ă��Ȃ�
bool Gamepad::Button(const DIPadButton& button)
{
	//�g�p�\�Ȃ�
	if(mCanUse)
	{
		//DirectInput�Ȃ�
		if(!mIsXInput)
		{
			return (mpDIState->rgbButtons[button] & 0x80) != 0;
		}
	}

	return false;
	
}


//GetInputButtonNum�֐��Ŏ擾�����l�Ń{�^���̏����擾
bool Gamepad::Button(const InputCode& button)
{
	//�g�p�\�Ȃ�
	if(mCanUse)
	{
		//XInput�Ȃ�
		if(mIsXInput)
		{
			return (mXIState.Gamepad.wButtons & button.mInputCode) != 0;
		}
		//DirectInput�Ȃ�
		else
		{
			CWE_ASSERT(button.mInputCode<32);
			return (mpDIState->rgbButtons[button.mInputCode] & 0x80) != 0;
		}
	}

	return false;
}


//�����ꂽ�{�^���̔ԍ����擾
//true=�擾���� false=�擾���s
bool Gamepad::GetInputButtonNum(InputCode& buttonNum)
{
	//�g�p�\�Ȃ�
	if(mCanUse)
	{
		//XInput�Ȃ�
		if(mIsXInput)
		{
			InputCode i;
			i.mInputCode =GamePad_START;
			for(; i.mInputCode<=GamePad_Y; i.mInputCode<<=1)
			{
				if(Button(i))
				{
					//�����R���g���[���[�̓��͖͂�������
					if(!((i.mInputCode==1) | (i.mInputCode==2) | (i.mInputCode==4) | (i.mInputCode==8)))
					{
						buttonNum =i;
						return true;
					}
				}
			}
		}
		//DirectInput�Ȃ�
		else
		{
			for(InputCode i; i.mInputCode<128; ++i.mInputCode)
			{
				if(Button(i))
				{
					buttonNum =i;
					return true;
				}
			}
		}
	}

	return false;
}


//�G�t�F�N�g�̊J�n
void Gamepad::EffectStart()
{
	if(mCanUseEffect)
	{
		if(mIsXInput)
		{
			XInputSetState(mNumber, &mXIEffect.mState);
			mEffectTimer.Start();
			mXIEffect.mRunning =true;
		}
		else
		{
			mpDIEffect->Start(1, 0);
		}
	}
}


//�G�t�F�N�g�̏I��
void Gamepad::EffectStop()
{
	if(mCanUseEffect)
	{
		if(mIsXInput)
		{
			XINPUT_VIBRATION xief;
			ZeroMemory(&xief, sizeof(xief));
			XInputSetState(mNumber, &xief);
			mXIEffect.mRunning =false;
		}
		else
		{
			mpDIEffect->Stop();
		}
	}
}


}//CWE