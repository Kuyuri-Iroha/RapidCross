#pragma once


#include "CWEGraphicResource.h"
#include "CWETypeDefinition.h"


namespace CWE
{


// ���[�V��������Ǘ��N���X
class Motion
{
private:
	ResourceHandle mHandle;
	// ���\�[�X�N���X�ɃA�N�Z�X����ׂ̎Q��
	Resource& mResource;
	// �Đ����̃t���[��
	float mOldFrame;

	/// �L�[�t���[���̃��Z�b�g
///	void ResetKeyFrames(MotionData& data);
	// VMD�p�x�W�F���
	float BezierInterpolation(float x1, float y1, float x2, float y2, float x);
	// �{�[���ւ̓K�p
	bool Apply(Bone& bone, float frame, MotionData& data);

public:
	Motion();
	Motion(const Motion& original);
	Motion& operator=(const Motion& original);
	~Motion();


	// �t�@�C���̃��[�h(�d���Ȃ�)
	void Load(const wchar_t *filePath, const FileFormat &format);

	// �n���h���̎Q�Ɛ悪���݂��邩
	bool Is() const {return mResource.IsMotion(mHandle);}

	// ���݂̃n���h���̎Q�Ɛ悪�g�p�\���ǂ���
	const Task& LoadResult()const;

	// �f�[�^�̃R�s�[
	void Copy(const Motion& original);

	// �f�[�^�̊J��
	void Release();

	// �Đ��ʒu��������Ԃɖ߂�
	void Restart();

	// �Đ�
	// �߂�l�Ftrue=�Đ���, false=�I��
	bool Update(Bone& bone, float frame);

	// �{�[�����W�Ɖ�]���擾
	void GetNowPose(const Bone& bone, math::Float3& pos, math::Quaternion& rotation);

	// �Đ����̃t���[�����擾
	inline float GetNowFrame()const {return mOldFrame;}

	// �ŏI�L�[�t���[�����擾
	unsigned GetLastKeyFrame() const;

};



}
