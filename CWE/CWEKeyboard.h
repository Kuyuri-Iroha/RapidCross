#pragma once

#include "CWETypeDefinition.h"
#include "CWEError.h"


namespace CWE
{
class Input;

//�L�[�R�[�h�̗�
enum KeyCode
{
	Key_ESCAPE			=DIK_ESCAPE, //�uEsc�v�L�[
	Key_1				=DIK_1, //�u1�v�L�[
	Key_2				=DIK_2, //�u2�v�L�[
	Key_3				=DIK_3, //�u3�v�L�[
	Key_4				=DIK_4, //�u4�v�L�[
	Key_5				=DIK_5, //�u5�v�L�[
	Key_6				=DIK_6, //�u6�v�L�[
	Key_7				=DIK_7, //�u7�v�L�[
	Key_8				=DIK_8, //�u8�v�L�[
	Key_9				=DIK_9, //�u9�v�L�[
	Key_0				=DIK_0, //�u0�v�L�[
	Key_MINUS			=DIK_MINUS,  //�u-�v�L�[
	Key_EQUALS			=DIK_EQUALS, //�u=�v�L�[
	Key_BACKSPACE		=DIK_BACK, //�uBackspace�v�L�[
	Key_TAB				=DIK_TAB, //�uTab�v�L�[
	Key_Q				=DIK_Q, //�uQ�v�L�[
	Key_W				=DIK_W, //�uW�v�L�[
	Key_E				=DIK_E, //�uE�v�L�[
	Key_R				=DIK_R, //�uR�v�L�[
	Key_T				=DIK_T, //�uT�v�L�[
	Key_Y				=DIK_Y, //�uY�v�L�[
	Key_U				=DIK_U, //�uU�v�L�[
	Key_I				=DIK_I, //�uI�v�L�[
	Key_O				=DIK_O, //�uO�v�L�[
	Key_P				=DIK_P, //�uP�v�L�[
	Key_LBRACKET		=DIK_LBRACKET, //�u [ �v�L�[
	Key_RBRACKET		=DIK_RBRACKET, //�u ] �v�L�[
	Key_ENTER			=DIK_RETURN, //�uEnter�v�L�[
	Key_LCONTROL		=DIK_LCONTROL, //�uCtrl�v�L�[�i���j
	Key_A				=DIK_A, //�uA�v�L�[
	Key_S				=DIK_S, //�uS�v�L�[
	Key_D				=DIK_D, //�uD�v�L�[
	Key_F				=DIK_F, //�uF�v�L�[
	Key_G				=DIK_G, //�uG�v�L�[
	Key_H				=DIK_H, //�uH�v�L�[
	Key_J				=DIK_J, //�uJ�v�L�[
	Key_K				=DIK_K, //�uK�v�L�[
	Key_L				=DIK_L, //�uL�v�L�[
	Key_SEMICOLON		=DIK_SEMICOLON, //�u;�v�L�[
	Key_APOSTROPHE		=DIK_APOSTROPHE, //�u ' �v�L�[
	Key_GRAVE			=DIK_GRAVE, //�u ` �v�L�[
	Key_LSHIFT			=DIK_LSHIFT, //�uShift�v�L�[�i���j
	Key_BACKSLASH		=DIK_BACKSLASH, //�u\�v�L�[
	Key_Z				=DIK_Z, //�uZ�v�L�[
	Key_X				=DIK_X, //�uX�v�L�[
	Key_C				=DIK_C, //�uC�v�L�[
	Key_V				=DIK_V, //�uV�v�L�[
	Key_B				=DIK_B, //�uB�v�L�[
	Key_N				=DIK_N, //�uN�v�L�[
	Key_M				=DIK_M, //�uM�v�L�[
	Key_COMMA			=DIK_COMMA, //�u , �v�L�[
	Key_PERIOD			=DIK_PERIOD, //�u . �v�L�[
	Key_SLASH			=DIK_SLASH, //�u/�v�L�[
	Key_RSHIFT			=DIK_RSHIFT, //�uShift�v�L�[�i�E�j
	Key_MULTIPLY		=DIK_MULTIPLY, //�u*�v�L�[�iNumpad�j
	Key_LALT			=DIK_LMENU, //�uAlt�v�L�[�i���j
	Key_SPACE			=DIK_SPACE, //�uSpace�v�L�[
	Key_CAPSLOCK		=DIK_CAPITAL, //�uCaps Lock�v�L�[
	Key_F1				=DIK_F1, //�uF1�v�L�[
	Key_F2				=DIK_F2, //�uF2�v�L�[
	Key_F3				=DIK_F3, //�uF3�v�L�[
	Key_F4				=DIK_F4, //�uF4�v�L�[
	Key_F5				=DIK_F5, //�uF5�v�L�[
	Key_F6				=DIK_F6, //�uF6�v�L�[
	Key_F7				=DIK_F7, //�uF7�v�L�[
	Key_F8				=DIK_F8, //�uF8�v�L�[
	Key_F9				=DIK_F9, //�uF9�v�L�[
	Key_F10				=DIK_F10, //�uF10�v�L�[
	Key_NUMLOCK			=DIK_NUMLOCK, //�uNum Lock�v�L�[
	Key_SCROLL			=DIK_SCROLL, //�uScroll Lock�v�L�[
	Key_NUMPAD7			=DIK_NUMPAD7, //�u7�v�L�[�iNumpad�j
	Key_NUMPAD8			=DIK_NUMPAD8, //�u8�v�L�[�iNumpad�j
	Key_NUMPAD9			=DIK_NUMPAD9, //�u9�v�L�[�iNumpad�j
	Key_SUBTRACT		=DIK_SUBTRACT, //�u-�v�L�[�iNumpad�j
	Key_NUMPAD4			=DIK_NUMPAD4, //�u4�v�L�[�iNumpad�j
	Key_NUMPAD5			=DIK_NUMPAD5, //�u5�v�L�[�iNumpad�j
	Key_NUMPAD6			=DIK_NUMPAD6, //�u6�v�L�[�iNumpad�j
	Key_ADD				=DIK_ADD, //�u+�v�L�[�iNumpad�j
	Key_NUMPAD1			=DIK_NUMPAD1, //�u1�v�L�[�iNumpad�j
	Key_NUMPAD2			=DIK_NUMPAD2, //�u2�v�L�[�iNumpad�j
	Key_NUMPAD3			=DIK_NUMPAD3, //�u3�v�L�[�iNumpad�j
	Key_NUMPAD0			=DIK_NUMPAD0, //�u0�v�L�[�iNumpad�j
	Key_DECIMAL			=DIK_DECIMAL, //�u.�v�L�[�iNumpad�j
	Key_F11				=DIK_F11, //�uF11�v�L�[
	Key_F12				=DIK_F12, //�uF12�v�L�[
	Key_F13				=DIK_F13, //�uF13�v�L�[
	Key_F14				=DIK_F14, //�uF14�v�L�[
	Key_F15				=DIK_F15, //�uF15�v�L�[
	Key_KANA			=DIK_KANA, //�u�J�i�v�L�[�i���{��L�[�{�[�h�j
	Key_CONVERT			=DIK_CONVERT, //�u�ϊ��v�L�[�i���{��L�[�{�[�h�j
	Key_NOCONVERT		=DIK_NOCONVERT, //�u���ϊ��v�L�[�i���{��L�[�{�[�h�j
	Key_YEN				=DIK_YEN, //�u\�v�L�[�i���{��L�[�{�[�h�j
	Key_NUMPADEQUALS	=DIK_NUMPADEQUALS, //�u=�v�L�[�iNumpad�j
	Key_CIRCUMFLEX		=DIK_CIRCUMFLEX, //�u^�v�L�[�i���{��L�[�{�[�h�j
	Key_AT				=DIK_AT, //�u@�v�L�[
	Key_COLON			=DIK_COLON, //�u:�v�L�[
	Key_UNDERLINE		=DIK_UNDERLINE, //�u _ �v�L�[
	Key_KANJI			=DIK_KANJI, //�u���p/�S�p�v�L�[
	Key_STOP			=DIK_STOP, //�uStop�v�L�[
	Key_NUMPADENTER		=DIK_NUMPADENTER, //�uEnter�v�L�[�iNumpad�j
	Key_RCONTROL		=DIK_RCONTROL, //�uCtrl�v�L�[�i�E�j
	Key_NUMPADCOMMA		=DIK_NUMPADCOMMA, //�u , �v�L�[�iNumpad�j
	Key_DIVIDE			=DIK_DIVIDE, //�u/�v�L�[�iNumpad�j
	Key_SYSRQ			=DIK_SYSRQ, //�uSys Rq�v�L�[
	Key_RALT			=DIK_RMENU, //�uAlt�v�L�[�i�E�j
	Key_PAUSE			=DIK_PAUSE, //�uPause�v�L�[
	Key_HOME			=DIK_HOME, //�uHome�v�L�[
	Key_UP				=DIK_UP, //�u���v�L�[
	Key_PAGEUP			=DIK_PRIOR, //�uPage Up�v�L�[
	Key_LEFT			=DIK_LEFT, //�u���v�L�[
	Key_RIGHT			=DIK_RIGHT, //�u���v�L�[
	Key_END				=DIK_END, //�uEnd�v�L�[
	Key_DOWN			=DIK_DOWN, //�u���v�L�[
	Key_PAGEDOWN		=DIK_NEXT, //�uPage Down�v�L�[
	Key_INSERT			=DIK_INSERT, //�uInsert�v�L�[
	Key_DELETE			=DIK_DELETE, //�uDelete�v�L�[
	Key_LWIN			=DIK_LWIN, //�uWindows�v�L�[�i���j
	Key_RWIN			=DIK_RWIN, //�uWindows�v�L�[�i�E�j
	Key_APPS			=DIK_APPS, //�uMenu�v�L�[
	Key_POWER			=DIK_POWER //�uPower�v�L�[
};



//�L�[�{�[�h���͂��T�|�[�g����N���X
class Keyboard
{
private:
	//�t�����h
	friend class CWE::Input;

	
	//�L�[�{�[�h�̃f�o�C�X
	LPDIRECTINPUTDEVICE8 mpDevice;

	//�L�[�̏��
	char mKeyState[256];


	//�V���O���g���p�^�[���ɂ�����
	Keyboard();
	~Keyboard();

	//������
	bool Init();
	//�I������
	void End();
	//�X�V����
	void Update();


public:
	//�V���O���g���p�^�[���ɂ�����
	static Keyboard& GetInstance();
	Keyboard(const Keyboard&) =delete;
	Keyboard& operator=(const Keyboard&) =delete;


	//Window�L�[���{�^����L���ɂ��邩���Z�b�g����
	//true=�L�� false=����
	bool ResetWindowsKey(bool windowsKey);


	//�C�ӂ̃L�[�̏����擾
	//true=������Ă��� false=������Ă��Ȃ�
	inline bool Key(const KeyCode& keyCode) {return (mKeyState[keyCode] & 0x80) != 0;}
	//GetInputKeyCode�֐��Ŏ擾�����l�ŃL�[�̏����擾
	inline bool Key(const InputCode& keyCode) {CWE_ASSERT(keyCode.mInputCode<256); return (mKeyState[keyCode.mInputCode] & 0x80) != 0;}

	//�����ꂽ�L�[�̃L�[�R�[�h���擾
	//true=�擾���� false=�擾���s
	bool GetInputKeyCode(InputCode& keyCode);

};



}//CWE