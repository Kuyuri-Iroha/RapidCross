#include "Controller.h"




// ================================= Controller ==================================

// ����f�o�C�X�̃t���O
#define GAMEPAD_ENABLED true
#define KEYBOARD_ENABLED true


// ���̓t���O�̍X�V
void Controller::InputUpdate()
{
	const bool gamepad =GAMEPAD_ENABLED,
			   keyboard =KEYBOARD_ENABLED;

	//�Q�[���p�b�h�̃A�i���O�����X�V
	UpdateGPState();

	//�u�ԓI�ȓ��͂����Z�b�g
	mInstPushFlag =ToUType(InputFlag::None);
	mInstLongFlag =ToUType(InputFlag::None);

	//�P�����������͂���Ă�����
	if(mPushFlag != ToUType(InputFlag::None))
	{
		if(!mPushed)
		{
			mPushPassed =0;
			mPushed =true;
		}
		else
		{
			if(smAllowanceFrame < mPushPassed)
			{
				mPushPassed =0;
				mPushed =false;
				mPushFlag =ToUType(InputFlag::None);
			}
			++mPushPassed;
		}
	}

	//�����������͂���Ă�����
	if(mLongFlag != ToUType(InputFlag::None))
	{
		if(!mIsLong)
		{
			mLongPassed =0;
			mIsLong =true;
		}
		else
		{
			if(smAllowanceFrame < mLongPassed)
			{
				mLongPassed =0;
				mIsLong =false;
				mLongFlag =ToUType(InputFlag::None);
			}
			++mLongPassed;
		}
	}

	//�P������
	if(Left(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Left);
		mInstPushFlag |=ToUType(InputFlag::Left);
	}
	if(Right(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Right);
		mInstPushFlag |=ToUType(InputFlag::Right);
	}
	if(Up(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Up);
		mInstPushFlag |=ToUType(InputFlag::Up);
	}
	if(Down(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Down);
		mInstPushFlag |=ToUType(InputFlag::Down);
	}
	if(SlightlyLeft(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::SlightLeft);
		mInstPushFlag |=ToUType(InputFlag::SlightLeft);
	}
	if(SlightlyRight(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::SlightRight);
		mInstPushFlag |=ToUType(InputFlag::SlightRight);
	}
	if(WeakAttack(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::WeakAttack);
		mInstPushFlag |=ToUType(InputFlag::WeakAttack);
	}
	if(SrgAttack(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::SrgAttack);
		mInstPushFlag |=ToUType(InputFlag::SrgAttack);
	}
	if(Guard(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Guard);
		mInstPushFlag |=ToUType(InputFlag::Guard);
	}
	if(Menu(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Menu);
		mInstPushFlag |=ToUType(InputFlag::Menu);
	}
	if(Back(gamepad, keyboard, false))
	{
		mPushFlag |=ToUType(InputFlag::Back);
		mInstPushFlag |=ToUType(InputFlag::Back);
	}

	//������
	if(Left(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::Left);
		mInstLongFlag |=ToUType(InputFlag::Left);
	}
	if(Right(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::Right);
		mInstLongFlag |=ToUType(InputFlag::Right);
	}
	if(Up(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::Up);
		mInstLongFlag |=ToUType(InputFlag::Up);
	}
	if(Down(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::Down);
		mInstLongFlag |=ToUType(InputFlag::Down);
	}
	if(SlightlyLeft(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::SlightLeft);
		mInstLongFlag |=ToUType(InputFlag::SlightLeft);
	}
	if(SlightlyRight(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::SlightRight);
		mInstLongFlag |=ToUType(InputFlag::SlightRight);
	}
	if(WeakAttack(gamepad, keyboard, true))
	{
		mPressing |=ToUType(InputFlag::WeakAttack);
		mLongFlag |=ToUType(InputFlag::WeakAttack);
		mInstLongFlag |=ToUType(InputFlag::WeakAttack);
	}
	if(SrgAttack(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::SrgAttack);
		mInstLongFlag |=ToUType(InputFlag::SrgAttack);
	}
	if(Guard(gamepad, keyboard, true))
	{
		mLongFlag |=ToUType(InputFlag::Guard);
		mInstLongFlag |=ToUType(InputFlag::Guard);
	}
	if(Menu(gamepad, keyboard, true))
	{
		mPressing |=ToUType(InputFlag::Menu);
		mLongFlag |=ToUType(InputFlag::Menu);
		mInstLongFlag |=ToUType(InputFlag::Menu);
	}
	else
	{
		mPressing &= ~ToUType(InputFlag::Menu);
	}
	if(Back(gamepad, keyboard, true))
	{
		mPressing |=ToUType(InputFlag::Back);
	}
	else
	{
		mPressing &= ~ToUType(InputFlag::Back);
	}
}


// Clear the input flags
void Controller::ClearFlags()
{
	mInstPushFlag =ToUType(InputFlag::None);
	mInstLongFlag =ToUType(InputFlag::None);
	mPushFlag =ToUType(InputFlag::None);
	mLongFlag =ToUType(InputFlag::None);
	mPressing =ToUType(InputFlag::None);
}


// �_�b�V��
bool Controller::Dash(bool isRight) const
{
	return isRight ? (mInstLongFlag & ToUType(InputFlag::Right))!=0 :
						(mInstLongFlag & ToUType(InputFlag::Left))!=0;
}


// ���s
bool Controller::Walk(bool isRight) const
{
	return isRight ? (mInstLongFlag & ToUType(InputFlag::SlightRight))!=0 :
						(mInstLongFlag & ToUType(InputFlag::SlightLeft))!=0;
}


// ���]
bool Controller::Reverse(bool isRight) const
{
	return isRight ? ((mInstLongFlag & ToUType(InputFlag::Left)) || (mInstLongFlag & ToUType(InputFlag::SlightLeft)))!=0 :
						((mInstLongFlag & ToUType(InputFlag::Right)) || (mInstLongFlag & ToUType(InputFlag::SlightRight))!=0) &&
			!(mInstLongFlag & ToUType(InputFlag::Up));
}


// �W�����v
bool Controller::Jump() const
{
	return (mInstPushFlag & ToUType(InputFlag::Up))!=0;
}


// ���
bool Controller::Back(bool isRight) const
{
	return isRight ? (mInstLongFlag & ToUType(InputFlag::Left))!=0 :
						(mInstLongFlag & ToUType(InputFlag::Right))!=0;
}


// �킸���Ɍ��
bool Controller::SlightlyBack(bool isRight) const
{
	return isRight ? (mInstLongFlag & ToUType(InputFlag::SlightLeft))!=0 :
						(mInstLongFlag & ToUType(InputFlag::SlightRight))!=0;
}


// ��
bool Controller::Down() const
{
	return (mInstLongFlag & ToUType(InputFlag::Down))!=0;
}


//Instant down
bool Controller::InstDown() const
{
	return (mInstPushFlag & ToUType(InputFlag::Down))!=0;
}


//����U��
bool Controller::SideWeakAttack() const
{
	return (mInstPushFlag & ToUType(InputFlag::WeakAttack))!=0;
}


//������A
bool Controller::ALong() const
{
	return (mPressing & ToUType(InputFlag::WeakAttack)) != 0;
}


//���U��
bool Controller::SideAttack(bool isRight) const
{
	return (isRight ? (mPushFlag & ToUType(InputFlag::Right))!=0 :
						(mPushFlag & ToUType(InputFlag::Left))!=0) &&
			(mPushFlag & ToUType(InputFlag::WeakAttack))!=0;
}


//��U��
bool Controller::UpperAttack() const
{
	return (mPushFlag & ToUType(InputFlag::Up))!=0 &&
			(mPushFlag & ToUType(InputFlag::WeakAttack))!=0;
}


//���U��
bool Controller::LowerAttack() const
{
	return (mPushFlag & ToUType(InputFlag::Down))!=0 &&
			(mPushFlag & ToUType(InputFlag::WeakAttack))!=0;
}


//�K�[�h
bool Controller::Guard() const
{
	return (mInstPushFlag & ToUType(InputFlag::Guard))!=0;
}


//���j���[
bool Controller::Menu() const
{
	return (mInstPushFlag & ToUType(InputFlag::Menu))!=0;
}


bool Controller::Back() const
{
	return (mInstPushFlag & ToUType(InputFlag::Back)) != 0;
}


bool Controller::BackLong() const
{
	return (mPressing & ToUType(InputFlag::Back)) != 0;
}



// ================================= Controller1 =================================

// �Q�[���p�b�h�|�C���^�̎���
CWE::Gamepad* Controller1::smpGamepad1 =nullptr;

//�R���X�g���N�^
Controller1::Controller1()
{
	mFlags.resize(NumberOfIndex);
}


// �f�X�g���N�^
Controller1::~Controller1()
{

}


// �Q�[���p�b�h�̃Z�b�g
void Controller1::SetGamepad()
{
	if(!smpGamepad1)
	{
		smpGamepad1 =CWE::Input::GetInstance().GetGamepad();
	}
}


// �Q�[���p�b�h�̍폜
void Controller1::RemoveGamepad()
{
	if(smpGamepad1)
	{
		CWE::Input::GetInstance().ReleaseGamepad(smpGamepad1);
		smpGamepad1 =nullptr;
	}
}


// ��
bool Controller1::Left(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[0] < -smAxisRefVal);
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_LEFT, Index_Left, useGamepad, useKeyboard, longPress);
}


// �E
bool Controller1::Right(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisRefVal < gpState.mAxis[0]);
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_RIGHT, Index_Right, useGamepad, useKeyboard, longPress);
}


// ��
bool Controller1::Up(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[1] < -smAxisLowest-15000);
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_UP, Index_Up, useGamepad, useKeyboard, longPress);
}


// ��
bool Controller1::Down(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisRefVal < gpState.mAxis[1]);
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_DOWN, Index_Down, useGamepad, useKeyboard, longPress);
}


// �኱��
bool Controller1::SlightlyLeft(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[0] < -smAxisLowest && -smAxisRefVal <= gpState.mAxis[0]);
	};
	
	return InputJudge(smpGamepad1, judge, CWE::Key_NUMPAD4, Index_SlightLeft, useGamepad, useKeyboard, longPress);
}


// �኱�E
bool Controller1::SlightlyRight(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisLowest < gpState.mAxis[0] && gpState.mAxis[0] <= smAxisRefVal);
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_NUMPAD6, Index_SlightRight, useGamepad, useKeyboard, longPress);
}


// ��U��
bool Controller1::WeakAttack(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_A));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_0));
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_PAGEUP, Index_WeakAttack, useGamepad, useKeyboard, longPress);
}


// ���U��
bool Controller1::SrgAttack(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_X));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_1));
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_INSERT, Index_SrgAttack, useGamepad, useKeyboard, longPress);
}


// �K�[�h
bool Controller1::Guard(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_RSHOULDER));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_9));
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_INSERT, Index_Guard, useGamepad, useKeyboard, longPress);
}


// ���j���[
bool Controller1::Menu(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_START));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_11));
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_ESCAPE, Index_Menu, useGamepad, useKeyboard, longPress);
}


bool Controller1::Back(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_BACK));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_10));
	};

	return InputJudge(smpGamepad1, judge, CWE::Key_DELETE, Index_Back, useGamepad, useKeyboard, longPress);
}



// ================================= Controller2 =================================

// �Q�[���p�b�h�|�C���^�̎���
CWE::Gamepad* Controller2::smpGamepad2 =nullptr;

// �R���X�g���N�^
Controller2::Controller2()
{
	mFlags.resize(NumberOfIndex);
}


// �f�X�g���N�^
Controller2::~Controller2()
{

}


// �Q�[���p�b�h�̃Z�b�g
void Controller2::SetGamepad()
{
	if(!smpGamepad2)
	{
		smpGamepad2 =CWE::Input::GetInstance().GetGamepad();
	}
}


// �Q�[���p�b�h�̍폜
void Controller2::RemoveGamepad()
{
	if(smpGamepad2)
	{
		CWE::Input::GetInstance().ReleaseGamepad(smpGamepad2);
		smpGamepad2 =nullptr;
	}
}


// ��
bool Controller2::Left(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[0] < -smAxisRefVal);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_A, Index_Left, useGamepad, useKeyboard, longPress);
}


// �E
bool Controller2::Right(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisRefVal < gpState.mAxis[0]);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_D, Index_Right, useGamepad, useKeyboard, longPress);
}


// ��
bool Controller2::Up(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[1] < -smAxisLowest-15000);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_W, Index_Up, useGamepad, useKeyboard, longPress);
}


// ��
bool Controller2::Down(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisRefVal < gpState.mAxis[1]);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_S, Index_Down, useGamepad, useKeyboard, longPress);
}


// �኱��
bool Controller2::SlightlyLeft(bool useGamepad, bool useKeyboard, bool longPress)
{

	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (gpState.mAxis[0] < -smAxisLowest && -smAxisRefVal <= gpState.mAxis[0]);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_F, Index_SlightLeft, useGamepad, useKeyboard, longPress);
}


// �኱�E
bool Controller2::SlightlyRight(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		return (smAxisLowest < gpState.mAxis[0] && gpState.mAxis[0] <= smAxisRefVal);
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_H, Index_SlightRight, useGamepad, useKeyboard, longPress);
}


// ��U��
bool Controller2::WeakAttack(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_A));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_0));
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_Z, Index_WeakAttack, useGamepad, useKeyboard, longPress);
}


// ���U��
bool Controller2::SrgAttack(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_X));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_1));
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_X, Index_SrgAttack, useGamepad, useKeyboard, longPress);
}


// �K�[�h
bool Controller2::Guard(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_RSHOULDER));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_9));
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_INSERT, Index_Guard, useGamepad, useKeyboard, longPress);
}


// ���j���[
bool Controller2::Menu(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_START));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_11));
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_ESCAPE, Index_Menu, useGamepad, useKeyboard, longPress);
}


bool Controller2::Back(bool useGamepad, bool useKeyboard, bool longPress)
{
	auto judge =[](CWE::Gamepad* gamepad, const CWE::GamepadState& gpState){
		if(gamepad->GetIsXInput())
		{
			return (gamepad->Button(CWE::XIPadButton::GamePad_BACK));
		}
		return (gamepad->Button(CWE::DIPadButton::GamePad_10));
	};

	return InputJudge(smpGamepad2, judge, CWE::Key_DELETE, Index_Back, useGamepad, useKeyboard, longPress);
}