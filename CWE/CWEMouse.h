#pragma once

#include "CWETypeDefinition.h"
#include "CWEError.h"



namespace CWE
{
class Input;
namespace callbacks//�g�p�֎~
{
LRESULT CALLBACK CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}


//�}�E�X�̃{�^�����͗p�}�X�N
enum MouseButton
{
	Mouse_Left =0x00,
	Mouse_Right =0x01,
	Mouse_Middle =0x02
};


//�}�E�X���͂��T�|�[�g����N���X
class Mouse
{
private:
	//�t�����h
	friend class CWE::Input;
	friend LRESULT CALLBACK callbacks::CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//(�Ăяo���֎~)


	//�}�E�X�̃f�o�C�X
	LPDIRECTINPUTDEVICE8 mpDevice;
	//���݂̃}�E�X���
	DIMOUSESTATE2 mState;
	//�N���C�A���g�̈�ɂ�����}�E�X�̍��W
	POINT mMousePos;

	//�}�E�X�J�[�\�����g�����������L�����ǂ���
	bool mPointerMode;

	//�}�E�X�̍��W���(�J�[�\�����L�����ɂ���ĈقȂ�)
	int mX, mY;


	//�V���O���g���p�^�[���ɂ�����
	Mouse();
	~Mouse();

	//������
	bool Init(const HWND window);
	//�I������
	void End();
	//�X�V����
	void Update();


public:
	//�V���O���g���p�^�[���ɂ�����
	static Mouse& GetInstance();
	Mouse(const Mouse&) =delete;
	Mouse& operator=(const Mouse&) =delete;

	//�}�E�X�J�[�\�����g���������̎g�p���Z�b�g	
	//�f�t�H���g�́u�g�p����v
	bool ResetPointerMode(const bool& usePointer);
	//�}�E�X�̍��W�f�[�^�̎擾
	inline void GetPoint(int& x, int& y) {x =mX; y =mY;}
	//�C�ӂ̃}�E�X�{�^���̏����擾
	//true=������Ă��� false=������Ă��Ȃ�
	inline bool Button(const MouseButton& button) {return (mState.rgbButtons[button] & 0x80) != 0;}
	//GetInputButtonNum�֐��Ŏ擾�����l�Ń{�^���̏����擾
	inline bool Button(const InputCode& button) {CWE_ASSERT(button.mInputCode<8); return (mState.rgbButtons[button.mInputCode] & 0x80) != 0;}

	//�����ꂽ�{�^���̔ԍ����擾
	//true=�擾���� false=�擾���s
	bool GetInputButtonNum(InputCode& buttonNum);
};


}//CWE

