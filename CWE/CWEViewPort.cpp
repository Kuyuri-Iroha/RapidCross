#include "CWEViewPort.h"
#include "CWEGraphicManager.h"
#include <algorithm>
#include "CWEError.h"



namespace CWE
{

// ========= �ڃI�u�W�F�N�g ==============

// �f�t�H���g�R���X�g���N�^
Eye::Eye():
mPos(0.0f, 0.0f, 10.0f), mFocusPos(), mUpDir(), mNormalizedPos(0.0f, 0.0f, 1.0f), mNormalizedPosIs(false), mDistance(10.0f)
{
}

// �R���X�g���N�^(�x�N�g������)
Eye::Eye(const math::Float3& pos, const math::Float3& focusPos, const math::Float3& upDir):
mPos(pos), mFocusPos(focusPos), mUpDir(upDir), mNormalizedPos(0.0f, 0.0f, 1.0f), mNormalizedPosIs(false), mDistance(10.0f)
{
}

// �f�X�g���N�^
Eye::~Eye()
{
}


// -------------- ���_���W�ړ��֐� ----------------

//���݈ʒu����̈ړ�
void Eye::PosMove(float x, float y, float z)
{
	mPos.x() +=x;
	mPos.y() +=y;
	mPos.z() +=z;
	mNormalizedPosIs =false;
}

// �ʒu�̕ύX
void Eye::PosReset(float x, float y, float z)
{
	mPos.x() =x;
	mPos.y() =y;
	mPos.z() =z;
	mNormalizedPosIs =false;
}

// �p�x��p���Č��݈ʒu����œ_�𒆐S�ɉ�]�ړ�
// ( 0<r ���L���l)
void Eye::PosMoveByAngle(float xAngle, float yAngle, float r)
{
	//��]���a�̕ύX
	if(0<r)
	{
		mDistance =r;
	}

	// ���K������Ă��Ȃ���ΐ��K��
	if(!mNormalizedPosIs)
	{
		mNormalizedPos.x() =mPos.x() -mFocusPos.x();
		mNormalizedPos.y() =mPos.y() -mFocusPos.y();
		mNormalizedPos.z() =mPos.z() -mFocusPos.z();
		math::VectorNormalize(&mNormalizedPos, mNormalizedPos);
		mNormalizedPosIs =true;
	}
	
	//�N�H�[�^�j�I���ɂ���]
	if(xAngle!=0.0f) {math::QuaternionRotationY(&mNormalizedPos, xAngle);}
	if(yAngle!=0.0f) {math::QuaternionRotationX(&mNormalizedPos, yAngle);}
	
	//���K���x�N�g�����王�_�ʒu���Z�o
	mPos.x() =mNormalizedPos.x() *mDistance;
	mPos.y() =mNormalizedPos.y() *mDistance;
	mPos.z() =mNormalizedPos.z() *mDistance;
	math::VectorAdd(&mPos, mPos, mFocusPos);
}



// -------------- �œ_���W�ړ��֐� ----------------

// ���݈ʒu����̈ړ�
void Eye::FocusMove(float x, float y, float z)
{
	mFocusPos.x() +=x;
	mFocusPos.y() +=y;
	mFocusPos.z() +=z;
	mNormalizedPosIs =false;
}

// �ʒu�̕ύX
void Eye::FocusReset(float x, float y, float z)
{
	mFocusPos.x() =x;
	mFocusPos.y() =y;
	mFocusPos.z() =z;
	mNormalizedPosIs =false;
}

// ���_�ʒu�Ƌ��Ɍ��݈ʒu����̈ړ�
void Eye::FocusMoveWithPos(float x, float y, float z)
{
	mPos.x() +=x;
	mPos.y() +=y;
	mPos.z() +=z;
	mFocusPos.x() +=x;
	mFocusPos.y() +=y;
	mFocusPos.z() +=z;
}


// ���_�ʒu�Ƌ��Ɉʒu�̕ύX
void Eye::FocusResetWithPos(float x, float y, float z)
{
	mPos.x() =x +(mPos.x() -mFocusPos.x());
	mPos.y() =y +(mPos.y() -mFocusPos.y());
	mPos.z() =z +(mPos.z() -mFocusPos.z());
	mFocusPos.x() =x;
	mFocusPos.y() =y;
	mFocusPos.z() =z;
}


// --------- ������ݒ�֐� ---------------

// ���݂̊p�x����ɕύX
/*
void Eye::UpDirMove(float rad)
{
	mUpDir.z +=rad;

	//�p�x��0<=��<360�ɒ���
	if((CWE_PI_F*2) <= mUpDir.z) {mUpDir.z -=(CWE_PI_F*2);}
	if(mUpDir.z < 0.0f) {mUpDir.z +=(CWE_PI_F*2);}

	//sinf, cosf�̎d�l��̖��ւ̑Ώ�
	if((4.71228409f < mUpDir.z && mUpDir.z < 4.71249390f) || (1.57069170f < mUpDir.z && mUpDir.z < 1.57090104f))
	{
		mUpDir.x =0.0f;
	}
	else
	{
		mUpDir.x =cosf(mUpDir.z);
	}

	if((3.14148808f < mUpDir.z && mUpDir.z < 3.14169741f) || (mUpDir.z < 0.0001047197f))
	{
		mUpDir.y =0.0f;
	}
	else
	{
		mUpDir.y =mUpDir.z;
	}
}
*/


// ������̕ύX
void Eye::UpDirReset(float x, float y)
{
	mUpDir.x() =x;
	mUpDir.y() =y;
}


// �r���[�s��쐬
void Eye::LookAtLHTp(math::Float4x4* view)
{
	math::LookAtLHTp(view, mPos, mFocusPos, mUpDir);
}


// ���_�Əœ_�̋������擾
float Eye::GetDistance()const
{
	return mNormalizedPosIs ? mDistance : math::VectorDistance(mPos, mFocusPos);
}


// ���_�Əœ_�̋�����2����擾
float Eye::GetSqrDistance()const
{
	return mNormalizedPosIs ? mDistance*mDistance : math::VectorDistanceSq(mPos, mFocusPos);
}


}// CWE