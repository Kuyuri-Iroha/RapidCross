#pragma once

#include "CWEIncludedAPI.h"
#include "CWEGamePad.h"
#include "CWEMacroConstants.h"


namespace CWE
{
namespace callbacks//�g�p�֎~
{
LRESULT CALLBACK CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext);
BOOL CALLBACK EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);
BOOL CALLBACK EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);
}

class Keyboard;
class Mouse;
class Gamepad;


//���̓��W���[���Ǘ��N���X
class Input
{
private:
	//�t�����h
	friend bool Init(unsigned width, unsigned height, bool windowMode, const wchar_t windowTitle[128]);
	friend bool Update();
	friend void End();
	friend class Keyboard;
	friend class Mouse;
	friend class Gamepad;

	//�Q��
	Keyboard& mKeyboard;
	Mouse& mMouse;


	//DirectInput�̃f�o�C�X
	LPDIRECTINPUT8 mpDevice;

//========================== �Q�[���p�b�h�Ǘ��p==================================================

	friend LRESULT CALLBACK callbacks::CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//(�Ăяo���֎~)
	friend BOOL CALLBACK callbacks::EnumGamePads(const DIDEVICEINSTANCE* pDIDInstance, VOID* pContext);//(�Ăяo���֎~)
	friend BOOL CALLBACK callbacks::EnumGamePadsFunction(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);//(�Ăяo���֎~)
	friend BOOL CALLBACK callbacks::EnumGamePadsFunctionForSetDeadZone(const DIDEVICEOBJECTINSTANCE* pDIDObInstance, VOID* pContext);//(�Ăяo���֎~)


	//�Q�[���p�b�h�̃f�o�C�X
	Gamepad mGamePads[CWE_GAMEPAD_MAX];

	//�ڑ����̃Q�[���p�b�h��
	unsigned mPadsNum;
	//���p���̃Q�[���p�b�h��
	unsigned mUsingPadsNum;
	//�ݒ蒆�̃Q�[���p�b�h�ԍ�
	unsigned mSettingPadNumber;
	//�f�o�C�X�̐ڑ���ԂɕύX����������
	bool mIsChangeConnection;

	//�������p�̔ėp�ϐ�
	unsigned char i;

	
	//�Q�[���p�b�h�̓o�^
	void GamepadRegister();

//========================================================

	//������
	bool Init();
	//�I������
	void End();
	//�X�V����
	void Update();
	
	Input();
	~Input();
	Input(const Input&) =delete;
	Input& operator=(const Input&) =delete;

public:
	//�V���O���g���p�^�[���̎���
	static Input& GetInstance();


	//�Q�[���p�b�h�N���X�̏���񋟂���֐�
	//�K��CGamePad*�^�̕ϐ��Ɋi�[���Ďg������
	Gamepad* const GetGamepad();

	//�擾�����Q�[���p�b�h�������
	void ReleaseGamepad(Gamepad* pGamepad);

	//�Q�[���p�b�h�f�o�C�X�̐ڑ���ԂɕύX�����������Ƃ�m�点��
	//true=�ύX���� false=�ύX�Ȃ�
	bool GetChangeConnection();

};



}// CWE

