#include "Motion.h"




// ���[�V�������Đ����A�ړ�/��]�ʂ��擾
bool Motion::Play(CWE::Polygon& applied, bool order, float speed, CWE::math::Matrix& movement)
{
	using namespace CWE::math;

	//���[�V�����̓K�p����
	const bool result =applied.MotionUpdate(mMotion, mNowFrame);
	std::vector<CWE::Bone>& bones =applied.GetBoneData(nullptr);
	Matrix inv;
	if(!CWE::math::MatrixIsIdentity(mOldPos))
	{
		inv =MatrixInverse(LoadMatrix(mOldPos));
		movement =MatrixMultiply(inv, LoadMatrix(applied.GetBone(0).mPose));
		mOldPos =bones[0].mPose;
	}
	else
	{
		mOldPos =mOldPos =bones[0].mPose;
		movement =MatrixIdentity();
	}

	//�S�Ă̐e�{�[���̕ύX�𖳌���
	MatrixIdentity(bones[0].mPose);

	//�ύX��S�Ẵ{�[���ɓK�p
	applied.FKApply();

	//�Đ����t���[�����X�V
	if(result)
	{
		mNowFrame +=order?speed:-speed;
	}

	return result;
}


// ���[�V������K�p���A�ړ�/��]�ʂ��擾
bool Motion::Apply(CWE::Polygon& applied, float frame, CWE::math::Matrix& movement)
{
	using namespace CWE::math;

	//�Đ����t���[�����X�V
	mNowFrame =frame;

	//���[�V�����̓K�p����
	const bool result =applied.MotionUpdate(mMotion, mNowFrame);
	std::vector<CWE::Bone>& bones =applied.GetBoneData(nullptr);
	Matrix inv;
	if(!CWE::math::MatrixIsIdentity(mOldPos))
	{
		inv =MatrixInverse(LoadMatrix(mOldPos));
		movement =MatrixMultiply(inv, LoadMatrix(applied.GetBone(0).mPose));
		mOldPos =bones[0].mPose;
	}
	else
	{
		mOldPos =mOldPos =bones[0].mPose;
		movement =MatrixIdentity();
	}

	//�S�Ă̐e�{�[���̕ύX�𖳌���
	MatrixIdentity(bones[0].mPose);

	//�ύX��S�Ẵ{�[���ɓK�p
	applied.FKApply();

	return result;
}


// ���X�^�[�g
void Motion::Restart()
{
	mMotion.Restart();
	mNowFrame =0.0f;
	CWE::math::MatrixIdentity(mOldPos);
}



// ============================ MotionInter ======================================

// �R���X�g���N�^
MotionInter::MotionInter():
	mNowCount(0), mInterCount(0)
{

}

// �f�X�g���N�^
MotionInter::~MotionInter()
{

}


// ���Z�b�g
void MotionInter::Reset(unsigned interCount)
{
	if(interCount == 0) {interCount =1;} //0�t���[����1�t���[���Ɠ��`��
	mInterCount =interCount+1; //�n�܂�ƏI�����܂܂Ȃ���
	mNowCount =mInterCount-1;
}


// ���s
bool MotionInter::Exec(CWE::Polygon& model, CWE::Motion& from, CWE::Motion& to)
{
	model.MotionLerp(to, from, float(mNowCount)/float(mInterCount));
	--mNowCount;
	if(mNowCount == 0) {return false;} //��ԏI��

	return true;
}