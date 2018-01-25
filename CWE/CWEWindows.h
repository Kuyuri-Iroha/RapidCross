#pragma once

#include "CWEIncludedAPI.h"


namespace CWE
{

//�R�[���o�b�N�֐������p���O���
namespace callbacks//�g�p�֎~
{
LRESULT CALLBACK CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}


class Mouse;
class Keyboard;
class Gamepad;




// Windows�p���N���X(���C�u�����O������̐����s��)
class Windows
{
private:
	Windows();
	~Windows();
	Windows(const Windows&) =delete;
	Windows& operator=(const Windows&) =delete;

	friend LRESULT CALLBACK callbacks::CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//(�Ăяo���֎~)
	friend class Graphic;
	friend class Input;
	friend bool Init(unsigned width, unsigned height, bool windowMode, const wchar_t windowTitle[128]);
	friend bool Update();
	friend void End();
	friend class Mouse;
	friend class Keyboard;
	friend class Gamepad;


	HINSTANCE mInstance; //�\�t�g�̃C���X�^���X�n���h��
	HWND mMainWindow; //���C���E�C���h�E�n���h��
	MSG m_WinMessage; //�E�C���h�E���b�Z�[�W
	wchar_t mClassName[128]; //���C���E�C���h�E�̃N���X��
	wchar_t mWindowTitle[128]; //�E�C���h�E�^�C�g��
	WORD mIconID; //�g�p����A�C�R����ID
	int mInitialWindowSizeX, //�E�B���h�E�쐬���̃E�B���h�E��X���W�̃T�C�Y
		mInitialWindowSizeY; //�E�B���h�E�쐬���̃E�B���h�E��Y���W�̃T�C�Y
	int mWindowSizeX, //�E�B���h�E��X���W�̃T�C�Y
		mWindowSizeY; //�E�B���h�E��Y���W�̃T�C�Y
	double mWindowSizeExRateX, //�E�B���h�E�̏����T�C�Y�ɑ΂��錻�݂̃E�B���h�E�T�C�Y�̔䗦X
		   mWindowSizeExRateY; //�E�B���h�E�̏����T�C�Y�ɑ΂��錻�݂̃E�B���h�E�T�C�Y�̔䗦Y
	POINT mClientPos; //�E�C���h�E�̃N���C�A���g�̈�̍���̍��W
	bool mIsWindowMode; //�E�C���h�E���[�h�t���O

	bool mIsMouseVisible; //�}�E�X�|�C���^�̕\���t���O
	bool mIsEndRequest; //�\�t�g�̏I�����v�����ꂽ���ǂ���

		
	struct WinMsgState
	{
		bool mQuitMessage; //WM_QUIT���b�Z�[�W�������Ă������ǂ���
		bool mActive; //WM_ACTIVATE���ɃE�C���h�E���A�N�e�B�u�ɂȂ������ǂ���

	}mWinMsgState;


	//������
	//�Ԃ�l: true=����, false=���s
	bool Init(const unsigned& width, const unsigned& height, const wchar_t windowTitle[128]);

	//�I������
	void End();

	//���b�Z�[�W����
	void ProcessMessage();

public:
	//�\�t�g�̏I���v��������
	inline void EndRequest() {mIsEndRequest =true;}

	//�\�t�g�A�C�R���̃Z�b�g
	inline void SetIconImage(WORD iconHandle) {mIconID =iconHandle;}

	// ���݂̃E�C���h�E�T�C�Y���擾
	inline int GetWindowSizeX() {return mWindowSizeX;}
	inline int GetWindowSizeY() {return mWindowSizeY;}

};


}