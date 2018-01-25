#pragma once

#include "CWEIncludedAPI.h"
#include "CWEGraphicManager.h"
#include "CWESound.h"
#include "CWETypeDefinition.h"
#include "CWEUsefullyMacro.h"
#include "CWERandomizer.h"
#include "CWEMath.h"
#include "CWEFileLoader.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <deque>
#include <array>
#include <functional>


namespace CWE
{
class Graphic;
class MasterSound;
bool Update();
void End();

class Polygon;
struct Eye;


// 3D�|���S���̃��b�V��
struct Mesh
{
	ID3D11ShaderResourceView* mpTexture;
	unsigned mVertexNum;
	float mSpecular;
};


// �{�[���f�[�^
struct Bone
{
	int mSelf; //���Ȃ̃C���f�b�N�X
	int mFather; //�e�{�[���C���f�b�N�X

	std::wstring mName;
	math::Float3 mPosition; //�ʒu�ό`
	math::Quaternion mRotation; //��]�ό`

	math::Float4x4 mInvOffsetPose; //��Ώ����p���t�s��
	math::Float4x4 mOffsetRelatPose; //�������Ύp���s��
	math::Float4x4 mRelatPose; //���Ύp���s��
	math::Float4x4 mPose; //��Ύp���s��
	math::Float4x4 mMatrix; //���_�ϊ��s��

	Bone(): mSelf(-1), mFather(-1) {}
	~Bone() {}
};

// IK�{�[���f�[�^
struct IKBone
{
	Bone* mpSelf;
	float mUnitAngle;
	Bone* mpTarget;
	unsigned mLoop;
	struct Link
	{
		Bone* mpBone;
		bool mExistAngleLimited;
		math::Float3 mLimitAngleMin;
		math::Float3 mLimitAngleMax;
	};
	std::vector<Link> mLinks;

	IKBone(): mpSelf(nullptr), mpTarget(nullptr) {};
	IKBone(Bone& self): mpSelf(&self), mpTarget(nullptr) {}
	~IKBone() {}
};


// 2D,3D����Ȃ��|���S���̏��
struct PolygonData
{
	// ���_�o�b�t�@�̐�
	static const unsigned VERTEX_BUFFER_NUM =8;

	const wchar_t *mFilePath;
	unsigned mReferenceCount; //�g�p���Ă��郂�W���[���̎Q�ƃJ�E���g
	Task mEnabled; //���[�h�ɐ���������
	FileFormat mFileFormat;

	// ���\�[�X
	ID3D11Buffer *mpVertexBuffers[VERTEX_BUFFER_NUM];
	ID3D11Buffer *mpIndexBuffer;
	unsigned mIndexCount;
	std::vector<Mesh> mMesh;
	std::vector<Bone> mBones;
	std::vector<Bone*> mpHierarchy; //�{�[�����̊K�w�D��\�[�g��
	std::vector<IKBone> mIKBone;

	PolygonData():mFilePath(0), mReferenceCount(0), mEnabled(Task::Working), mFileFormat(FileFormat::Unknown), mpIndexBuffer(0), mIndexCount(0)
	{
		for(int i=0; i<VERTEX_BUFFER_NUM; ++i) 
		{
			mpVertexBuffers[i] =nullptr;
		}
	}
	~PolygonData()
	{
		for(unsigned i=0; i<VERTEX_BUFFER_NUM; ++i)
		{
			SAFE_RELEASE(mpVertexBuffers[i]);
		}
		SAFE_RELEASE(mpIndexBuffer);
		for(unsigned j=0; j<mMesh.size(); ++j) {SAFE_RELEASE(mMesh[j].mpTexture);}
	}
};


// 1�t���[���P�ʂ̃��[�V�������
struct FrameData
{
	unsigned mFrameNumber; //�t���[���ԍ�(���t���[���ڂ̏�����)
	math::Float3 mBonePos; //�{�[���̈ʒu
	math::Quaternion mBoneQuaternion; //�{�[���̃N�H�[�^�j�I��
	std::array<float, 4> mXInterpolation, //X��ԃp�����[�^�[
						  mYInterpolation, //Y��ԃp�����[�^�[
						  mZInterpolation, //Z��ԃp�����[�^�[
						  mRInterpolation; //��]��ԃp�����[�^�[

	bool operator<(const FrameData& frameData)const {return mFrameNumber < frameData.mFrameNumber;}
};


// ���[�V�������
struct MotionData
{
	const wchar_t *mFilePath;
	unsigned mReferenceCount; //�g�p���Ă��郂�W���[���̎Q�ƃJ�E���g
	Task mEnabled; //���[�h�ɐ���������
	FileFormat mFileFormat;

	// �t���[���f�[�^(<�{�[����, <���񂵂��t���[���f�[�^, �Đ����̃L�[�t���[���C���f�b�N�X>>)
	std::unordered_map<std::wstring, std::pair<std::vector<FrameData>, unsigned>> mFrameData;
	//�Ō�̃L�[�t���[��
	unsigned mLastKeyFrame;


	MotionData():mReferenceCount(0), mEnabled(Task::Working), mFileFormat(FileFormat::Unknown), mLastKeyFrame(0)
	{
	}
	~MotionData()
	{
	}
};


// Sound Data
struct SoundData
{
	const wchar_t *mFilePath;
	unsigned mReferenceCount; //�g�p���Ă��郂�W���[���̎Q�ƃJ�E���g
	Task mEnabled; //���[�h�ɐ���������
	FileFormat mFileFormat;

	IXAudio2SourceVoice* mpSourceVoice;
	CWE::WAVData mSoundData;
	XAUDIO2_BUFFER mBuffer;

	SoundData(): mReferenceCount(), mEnabled(Task::Working), mFileFormat(FileFormat::Unknown), mpSourceVoice(), mBuffer()
	{}
	~SoundData()
	{
		SafeDestroy(mpSourceVoice);
	}
};


// ���[�h�^�X�N�̊i�[�^
struct LoadTask
{
	std::function<void(Resource&, int)> mTask;
	ResourceHandle mHandle;

	LoadTask(std::function<void(Resource&, int)> task, const ResourceHandle &handle): mTask(task), mHandle(handle) {}
	~LoadTask() {}
};


// ���\�[�X�Ǘ��N���X
class Resource
{
private:
	friend bool CWE::Update();
	friend void CWE::End();

	Graphic& mGraphicM;
	MasterSound& mSoundM;

	Resource();
	~Resource();
	Resource(const Resource&) =delete;
	Resource& operator=(const Resource&) =delete;


	// ���\�[�X
	std::unordered_map<int, PolygonData> mPolygons;
	std::unordered_map<int, MotionData> mMotions;
	std::unordered_map<int, SoundData> mSounds;
	// �X���b�h
	std::thread* mpWorker;
	std::mutex mMutex;

	// �n���h�������p�ԍ�
	int mHandleNumber;

	// ���[�h�^�X�N�o�b�t�@
	std::deque<LoadTask> mTaskBuffer;


	// �I��
	void End();

	// ���\�[�X�̐���(�X���b�h�֐�)
	static void CreateAll(Resource &resource);
	// ���\�[�X�̃��[�h�̃��N�G�X�g
	void LoadRequest(const ResourceHandle &handle, const FileFormat& format);
	// �e��t�@�C�����烊�\�[�X�̐���(�{��)
	static void CreateFromPMX(Resource& resource, int handle);
	static void CreateFromPicture(Resource& resource, int handle);
	static void CreateFromVMD(Resource& resource, int handle);
	static void CreateFromWAVE(Resource& resource, int handle);
	// ���\�[�X�̃��[�h(�{��)
	void LoadExecution(bool ending);
	// �q�G�����L�[�\�[�g�pequal_range
	void EqualRange(unsigned* lower, unsigned* upper, int value, const std::vector<Bone*>& tmpB);

public:
	static Resource& GetInstance();

	// ���\�[�X�̐���
	ResourceHandle Create(const wchar_t *filePath, const FileFormat& format);
	// ���\�[�X�n���h���̃R�s�[
	const ResourceHandle& CopyPolygonHandle(const ResourceHandle &handle);
	const ResourceHandle& CopyMotionHandle(const ResourceHandle& handle);
	const ResourceHandle& CopySoundHandle(const ResourceHandle& handle);
	// ���\�[�X�̊J��
	void PolygonRelease(const ResourceHandle &handle);
	void MotionRelease(const ResourceHandle& handle);
	void SoundRelease(const ResourceHandle& handle);
	// ���\�[�X�����݂��邩(�|���S���f�[�^)
	inline bool IsPolygon(const ResourceHandle& handle) const {return mPolygons.find(handle.mHandle) != mPolygons.end();}
	// ���\�[�X�����݂��邩(���[�V�����f�[�^)
	inline bool IsMotion(const ResourceHandle& handle) const {return mMotions.find(handle.mHandle) != mMotions.end();}
	// ���\�[�X�����݂��邩(�T�E���h�f�[�^)
	inline bool IsSound(const ResourceHandle& handle) const {return mSounds.find(handle.mHandle) != mSounds.end();}
	// ���\�[�X�f�[�^�̎󂯓n��(�|���S���f�[�^)
	PolygonData& GetPolygonData(const ResourceHandle& handle);
	const PolygonData& GetPolygonData(const ResourceHandle& handle)const;
	// ���\�[�X�f�[�^�̎󂯓n��(���[�V�����f�[�^)
	MotionData& GetMotionData(const ResourceHandle& handle);
	const MotionData& GetMotionData(const ResourceHandle& handle)const;
	// ���\�[�X�f�[�^�̎󂯓n��(�T�E���h�f�[�^)
	SoundData& GetSoundData(const ResourceHandle& handle);
	const SoundData& GetSoundData(const ResourceHandle& handle)const;

};



}// CWE


