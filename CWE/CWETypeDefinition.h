#pragma once

#include "CWEIncludedAPI.h"

namespace CWE
{

// �^�X�N�̏��
enum class Task
{
	Success,
	Failed,
	Working,
};


// �Ή����Ă���t�@�C���t�H�[�}�b�g
enum class FileFormat
{
	PMX,
	Picture,
	VMD,
	WAVE,
	Unknown,
};


// �|���S���̃n���h���f�[�^���i�[
struct ResourceHandle
{
private:
	friend class Resource;
	int mHandle;

public:
	ResourceHandle():mHandle(-1){}
	ResourceHandle(const ResourceHandle &original): mHandle(original.mHandle){}
	~ResourceHandle() {}
};


// ���̓R�[�h�i�[�p�\����
struct InputCode
{
private:
	friend class Keyboard;
	friend class Mouse;
	friend class Gamepad;
	unsigned int mInputCode;

public:
	InputCode():mInputCode(0){}
	~InputCode(){}
};

//�����H��XInput�̃f�[�^�����������߂̍\����
typedef XINPUT_STATE XIState;

//�����H��DirectInput�̃f�[�^�����������߂̍\����
typedef DIJOYSTATE DIState;


}//CWE
