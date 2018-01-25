#pragma once

#include "CWETimer.h"
#include "CWETypeDefinition.h"
#include <array>



namespace CWE
{
namespace callbacks
{
BOOL CALLBACK EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext);
BOOL CALLBACK EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);
BOOL CALLBACK EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);
}
class Input;


enum XIPadButton
{
	/*
	GamePad_DPAD_UP		=XINPUT_GAMEPAD_DPAD_UP,
	GamePad_DPAD_DOWN	=XINPUT_GAMEPAD_DPAD_DOWN,
	GamePad_DPAD_LEFT	=XINPUT_GAMEPAD_DPAD_LEFT,
	GamePad_DPAD_RIGHT	=XINPUT_GAMEPAD_DPAD_RIGHT,
	*/

	GamePad_START		=XINPUT_GAMEPAD_START,
	GamePad_BACK		=XINPUT_GAMEPAD_BACK,
	GamePad_LTHUMB		=XINPUT_GAMEPAD_LEFT_THUMB,
	GamePad_RTHUMB		=XINPUT_GAMEPAD_RIGHT_THUMB,
	GamePad_LSHOULDER	=XINPUT_GAMEPAD_LEFT_SHOULDER,
	GamePad_RSHOULDER	=XINPUT_GAMEPAD_RIGHT_SHOULDER,
	GamePad_A			=XINPUT_GAMEPAD_A,
	GamePad_B			=XINPUT_GAMEPAD_B,
	GamePad_X			=XINPUT_GAMEPAD_X,
	GamePad_Y			=XINPUT_GAMEPAD_Y,
};


enum DIPadButton
{
	GamePad_0  =0x00,
	GamePad_1  =0x01,
	GamePad_2  =0x02,
	GamePad_3  =0x03,
	GamePad_4  =0x04,
	GamePad_5  =0x05,
	GamePad_6  =0x06,
	GamePad_7  =0x07,
	GamePad_8  =0x08,
	GamePad_9  =0x09,
	GamePad_10 =0x0A,
	GamePad_11 =0x0B,
};


struct GamepadState
{
	int mAxis[6]; //-32768�`32767
	bool mPOV_Up;
	bool mPOV_Down;
	bool mPOV_Left;
	bool mPOV_Right;
	bool mLeftTrigger; //XInput
	bool mRightTrigger; //XInput
};


struct GamepadEffect
{
	//0�`10000
	int mMagnitude;
	//Second
	double mDuration;
	bool mInfinite;
};


class Gamepad
{
private:
	friend class CWE::Input;
	friend BOOL CALLBACK callbacks::EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext);
	friend BOOL CALLBACK callbacks::EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);
	friend BOOL CALLBACK callbacks::EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);


	LPDIRECTINPUTDEVICE8W mpDevice;
	LPDIRECTINPUTEFFECT mpDIEffect;

	DIJOYSTATE* mpDIState;
	XINPUT_STATE mXIState;
	GamepadState mState;
	DIEFFECT* mpDIEffectState;
	struct XIEffect
	{
		XINPUT_VIBRATION mState;
		bool mRunning;
	};
	XIEffect mXIEffect;

	Timer mEffectTimer;

	//���ʔԍ�
	int mNumber;
	GUID mIdentifier;

	bool mCanUse;
	bool mIsXInput;
	bool mCanUseEffect;

	struct DeadZone
	{
		int mValue;
		unsigned int mPercentage; //�f�t�H���g��3000
	};
	DeadZone mDeadZone;


	Gamepad();
	~Gamepad();

	bool DIInit();
	bool XIInit(const unsigned& padNum);
	void Clear();
	void End();
	void Update();

public:
	Gamepad(const Gamepad&) =delete;
	Gamepad& operator=(const Gamepad&) =delete;

	//percentage: 0�`10000
	//5000�Ȃ�50%
	void ResetDeadZone(unsigned percentage);

	//�R���g���[���[�̐U���G�t�F�N�g��ݒ肷��
	//true=���� false=���s
	//�f�o�C�X���Ή����Ă��Ȃ��ꍇ��false
	bool ResetEffect(const GamepadEffect& effect);

	//���݂��̃Q�[���p�b�h�����p�\���ǂ����̎擾
	inline const bool& GetCanUse(){return mCanUse;}

	//���̃Q�[���p�b�h�̎擾���@��XInput��DirectInput���擾����
	//true=XInput false=DirectInput
	inline const bool& GetIsXInput(){return mIsXInput;}

	//�����H��XInput�̏����擾����
	void GetRawXIState(XIState& state);

	//�����H��DirectInput�̏����擾����
	//true=�擾���� false=�擾���s
	bool GetRawDIState(DIState& state);

	//�Q�[���p�b�h��Button�֐��Ŏ擾�ł��Ȃ������擾����
	inline void GetState(GamepadState& state) {state =mState;}


	//�C�ӂ̃{�^�����̎擾(XInput)
	//true=������Ă��� false=������Ă��Ȃ�
	bool Button(const XIPadButton& button);
	//�C�ӂ̃{�^�����̎擾(DirectInput)
	//true=������Ă��� false=������Ă��Ȃ�
	bool Button(const DIPadButton& button);
	//GetInputButtonNum�֐��Ŏ擾�����l�Ń{�^���̏����擾
	bool Button(const InputCode& button);

	//�����ꂽ�{�^���̔ԍ����擾
	//true=�擾���� false=�擾���s
	bool GetInputButtonNum(InputCode& buttonNum);

	//�G�t�F�N�g�̊J�n
	void EffectStart();
	//�G�t�F�N�g�̏I��
	void EffectStop();

};



}//CWE

