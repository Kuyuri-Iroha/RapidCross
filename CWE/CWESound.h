#pragma once

#include "CWEFileLoader.h"
#include <list>
#include "CWEGraphicResource.h"
#include "CWETypeDefinition.h"

namespace CWE
{

class Graphic;

// User sound class
class Sound
{
	friend class MasterSound;

private:
	Resource& mResource;
	ResourceHandle mHandle;

public:
	Sound();
	Sound(const Sound &original);
	Sound& operator=(const Sound& original);
	~Sound();
	
	// �t�@�C���̃��[�h(�V�K���\�[�X�̐���)
	void Load(const wchar_t *filePath, const FileFormat &format);
	// �n���h���̎Q�Ɛ悪���݂��邩�ǂ���
	bool Is() const;
	// ���݂̃n���h���̎Q�Ɛ悪�g�p�\���ǂ���
	const Task& LoadResult()const;
	// �f�[�^�̃R�s�[
	void Copy(const Sound &original);
	// �f�[�^�̊J��
	void Release();

	float GetVolume() const;
	bool SetVolume(float volume);
	bool Start(bool loop, unsigned loopBegin, unsigned loopLength);
	bool Stop();
	bool Ended();

};


// Sounds operator
class MasterSound
{
public:
	static MasterSound& GetInstance();
	bool Init();
	void End();

private:
	IXAudio2* mpXAudio2;
	IXAudio2MasteringVoice* mpMasteringVoice;

	MasterSound();
	~MasterSound();

	friend class CWE::Resource;

};


}