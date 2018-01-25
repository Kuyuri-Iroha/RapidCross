#include "CWEMotion.h"
#include "CWEMath.h"


namespace CWE
{


// �R���X�g���N�^
Motion::Motion():
	mHandle(), mResource(Resource::GetInstance()),
	mOldFrame(0.0f)
{
}


// �R�s�[�R���X�g���N�^
Motion::Motion(const Motion& original):
	mHandle(), mResource(Resource::GetInstance()),
	mOldFrame(0.0f)
{
	mHandle =mResource.CopyMotionHandle(original.mHandle);
}

Motion& Motion::operator=(const Motion& original)
{
	mHandle =mResource.CopyMotionHandle(original.mHandle);
	return *this;
}


// �f�X�g���N�^
Motion::~Motion()
{
	Release();
}


// �t�@�C���̃��[�h
void Motion::Load(const wchar_t *filePath, const FileFormat &format)
{
	mHandle =mResource.Create(filePath, format);
}


// ���݂̃n���h���̎Q�Ɛ悪�g�p�\���ǂ���
const Task& Motion::LoadResult()const
{
	return mResource.GetMotionData(mHandle).mEnabled;
}


// �f�[�^�̃R�s�[
void Motion::Copy(const Motion& original)
{
	mHandle =mResource.CopyMotionHandle(original.mHandle);
}


// �f�[�^�̊J��
void Motion::Release()
{
	mResource.MotionRelease(mHandle);
}


// �Đ��ʒu�����[�v�̍ŏ��ɖ߂�
void Motion::Restart()
{
	mOldFrame =0.0f;
	for(auto& a:mResource.GetMotionData(mHandle).mFrameData)
	{
		a.second.second =0;
	}
}


// �Đ�
bool Motion::Update(Bone& bone, float frame)
{
	MotionData& data =mResource.GetMotionData(mHandle);
	if(data.mEnabled != Task::Success) {return false;}
	// �{�[���Ƀ��[�V������K�p
	return Apply(bone, frame, data);
}



// ======================= �v���C�x�[�g =========================


/// �L�[�t���[���̃��Z�b�g
/*
void Motion::ResetKeyFrames(MotionData& data)
{
	unsigned low=0, high=0, mid =0;
	for(std::map<std::wstring, std::pair<std::vector<FrameData>, unsigned>>::iterator itr=data.mFrameData.begin(); itr!=data.mFrameData.end(); ++itr)
	{
		// ���ꉻ�񕪒T��
		low =0;
		high =itr->second.first.size();
		while(1 < high-low) //�͈͂�1�ȉ��ɂȂ����犮��
		{
			mid =low+((high-low)/2);
			if(mOldFrame < static_cast<double>(itr->second.first[mid].mFrameNumber)) {high =mid;}
			else if(static_cast<double>(itr->second.first[mid].mFrameNumber) < mOldFrame) {low =mid;}
			else {low =mid; break;}
		}

		// �Đ��ʒu�̃L�[�t���[���C���f�b�N�X
		itr->second.second =low;
	}
}
*/


// VMD�p�x�W�F���
float Motion::BezierInterpolation(float x1, float y1, float x2, float y2, float x)
{
	//�f�[�^��0.0�`1.0��
	x1 /=127.0f;
	y1 /=127.0f;
	x2 /=127.0f;
	y2 /=127.0f;

	float param = 0.5f;
	float approx =0.0f;

	//�x�W�F�Ȑ� X�֐�
	auto fx =[x1, x2](float t){
		float s =1.0f -t;
		return (3 *s*s *t *x1) + (3 *s *t*t *x2) + (t*t*t);
	};
	//�x�W�F�Ȑ� Y�֐�
	auto fy =[y1, y2](float t){
		float s =1.0f -t;
		return (3 *s*s *t *y1) + (3 *s *t*t *y2) + (t*t*t);
	};

	//�j���[�g���@���g���ĕ����_����p�����[�^���Z�o
	for (int i = 0; i < 15; i++)
	{
		approx =fx(param);

		if(math::NearlyEqual(approx, x)) {break;}

		if(approx > x)
		{
			param -= 1.0f / float(4 << i);
		}
		else //approx < x
		{
			param += 1.0f / float(4 << i);
		}
	}

	return fy(param);
}


// �{�[���ւ̓K�p
bool Motion::Apply(Bone& bone, float frame, MotionData& data)
{
	if(data.mFrameData.find(bone.mName) == data.mFrameData.end()) {return false;} //�w�肳�ꂽ�{�[�����Ȃ���Ζ���
	std::pair<std::vector<FrameData>, unsigned>& refF =data.mFrameData.at(bone.mName);

	// VMD�p�x�W�F���
	float bezierX =0.0f;

	// �ꎞ�ۑ�
	bool order =mOldFrame <= frame;
	mOldFrame =frame;
	bool result =true;
	unsigned numFrames =refF.first.size();
	unsigned nextKeyFrame =refF.second +(order?1:-1);

	// ���[�V�������I����Ă��邩
	if(float(data.mLastKeyFrame) <= mOldFrame)
	{
		mOldFrame =float(data.mLastKeyFrame);
		result =false;
	}
	else if(mOldFrame < 0.0f)
	{
		mOldFrame =0.0f;
		result =false;
	}

	if(order) //���Đ�
	{
		// �L�[�t���[���̈ڍs����
		while(true)
		{
			//�Ō�̃L�[�t���[���������ꍇ
			if(numFrames <= refF.second+1)
			{
				nextKeyFrame =refF.second;
				break;
			}

			if((mOldFrame -refF.first[refF.second+1].mFrameNumber <= 0.0f))
			{
				break; //�ړI�̃t���[���ɂȂ����甲���o��
			}
			++refF.second;
			nextKeyFrame =refF.second+1;
		}

		//�x�W�F�Ȑ���X���W���Z�o
		if(refF.second == nextKeyFrame) //0���Z���
		{
			bezierX =1.0f;
		}
		else
		{
			bezierX =(mOldFrame -refF.first[refF.second].mFrameNumber) /(refF.first[nextKeyFrame].mFrameNumber -refF.first[refF.second].mFrameNumber);
		}
	}
	else //�t�Đ�
	{
		// �L�[�t���[���̈ڍs����
		while(true)
		{
			//�Ō�̃L�[�t���[���������ꍇ
			if(refF.second <= 0)
			{
				nextKeyFrame =refF.second;
				break;
			}

			if((refF.first[refF.second].mFrameNumber -mOldFrame <= 0.0f))
			{
				break; //�ړI�̃t���[���ɂȂ����甲���o��
			}
			--refF.second;
			nextKeyFrame =refF.second+1;
		}

		//�x�W�F�Ȑ���X���W���Z�o
		if(refF.second == nextKeyFrame) //0���Z���
		{
			bezierX =0.0f;
		}
		else
		{
			bezierX =(mOldFrame -refF.first[refF.second].mFrameNumber) /(refF.first[nextKeyFrame].mFrameNumber -refF.first[refF.second].mFrameNumber);
		}
	}

	//Clamp
	if(bezierX <= 0.0f) {bezierX =0.0f;}
	else if(1.0f <= bezierX) {bezierX =1.0f;}

	//���W���
	math::VectorLerp(&bone.mPosition, refF.first[refF.second].mBonePos, refF.first[nextKeyFrame].mBonePos,
		BezierInterpolation(refF.first[refF.second].mXInterpolation[0], refF.first[refF.second].mXInterpolation[1], refF.first[refF.second].mXInterpolation[2], refF.first[refF.second].mXInterpolation[3], bezierX),
		BezierInterpolation(refF.first[refF.second].mYInterpolation[0], refF.first[refF.second].mYInterpolation[1], refF.first[refF.second].mYInterpolation[2], refF.first[refF.second].mYInterpolation[3], bezierX),
		BezierInterpolation(refF.first[refF.second].mZInterpolation[0], refF.first[refF.second].mZInterpolation[1], refF.first[refF.second].mZInterpolation[2], refF.first[refF.second].mZInterpolation[3], bezierX)
		);
		
	//��]���
	math::QuaternionSlerp(&bone.mRotation, refF.first[refF.second].mBoneQuaternion, refF.first[nextKeyFrame].mBoneQuaternion,
		BezierInterpolation(refF.first[refF.second].mRInterpolation[0], refF.first[refF.second].mRInterpolation[1], refF.first[refF.second].mRInterpolation[2], refF.first[refF.second].mRInterpolation[3], bezierX)
		);

	math::MatrixTranslationR(&bone.mPose, bone.mPosition, bone.mRotation); //mPose���ꎞ�I�ɗ��p
	math::MatrixMultiply(&bone.mRelatPose, bone.mPose, bone.mRelatPose);
	
	return result;
}


// �{�[�����W�Ɖ�]���擾
void Motion::GetNowPose(const Bone& bone, math::Float3& pos, math::Quaternion& rotation)
{
	MotionData& data =mResource.GetMotionData(mHandle);
	if(data.mEnabled != Task::Success) {return;}
	if(data.mFrameData.find(bone.mName) == data.mFrameData.end()) {return;} //�w�肳�ꂽ�{�[�����Ȃ���Ζ���
	std::pair<std::vector<FrameData>, unsigned>& refF =data.mFrameData.at(bone.mName);
	float bezierX =0.0f;
	unsigned nextKeyFrame =refF.second;

	//�x�W�F�Ȑ���X���W���Z�o
	if(float(refF.first.size()-1) <= mOldFrame) //�ŏI�t���[�����ǂ���
	{
		mOldFrame =static_cast<float>(refF.first.size());
		bezierX =1.0f;
	}
	else
	{
		++nextKeyFrame;
		bezierX =(mOldFrame -refF.first[refF.second].mFrameNumber) /(refF.first[nextKeyFrame].mFrameNumber -refF.first[refF.second].mFrameNumber);
	}

	if(bezierX <= 0.0f) {bezierX =0.0f;}
	else if(1.0f <= bezierX) {bezierX =1.0f;}

	//���W���
	math::VectorLerp(&pos, refF.first[refF.second].mBonePos, refF.first[nextKeyFrame].mBonePos,
		BezierInterpolation(refF.first[refF.second].mXInterpolation[0], refF.first[refF.second].mXInterpolation[1], refF.first[refF.second].mXInterpolation[2], refF.first[refF.second].mXInterpolation[3], bezierX),
		BezierInterpolation(refF.first[refF.second].mYInterpolation[0], refF.first[refF.second].mYInterpolation[1], refF.first[refF.second].mYInterpolation[2], refF.first[refF.second].mYInterpolation[3], bezierX),
		BezierInterpolation(refF.first[refF.second].mZInterpolation[0], refF.first[refF.second].mZInterpolation[1], refF.first[refF.second].mZInterpolation[2], refF.first[refF.second].mZInterpolation[3], bezierX)
		);
		
	//��]���
	math::QuaternionSlerp(&rotation, refF.first[refF.second].mBoneQuaternion, refF.first[nextKeyFrame].mBoneQuaternion,
		BezierInterpolation(refF.first[refF.second].mRInterpolation[0], refF.first[refF.second].mRInterpolation[1], refF.first[refF.second].mRInterpolation[2], refF.first[refF.second].mRInterpolation[3], bezierX)
		);
}


// �ŏI�t���[�����擾
unsigned Motion::GetLastKeyFrame() const
{
	MotionData& data =mResource.GetMotionData(mHandle);
	if(data.mEnabled != Task::Success) {return 0;}

	return data.mLastKeyFrame;
}



}
