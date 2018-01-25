#include "Gumi.h"
#include "Stage.h"
#include "Information.h"
#include <algorithm>



// ================================== Gumi ================================================

using namespace CWE;

// �d��
const float Gumi::smWeight =50.0f;
// �W�����v��
const float Gumi::smJumpingAbility1 =3.0f;
const float Gumi::smJumpingAbility2 =3.0f;


const wchar_t gGumi1[36] =L"Resource\\m_GUMI_V3_201306\\GUMI1.pmx";
const wchar_t gGumi2[36] =L"Resource\\m_GUMI_V3_201306\\GUMI2.pmx";

// GUMI�̐F����
auto ColorNumToFile(unsigned colNum)
{
	switch(colNum)
	{
	case 0:
		return gGumi1;

	case 1:
		return gGumi2;
	}

	return L"";
}

// �R���X�g���N�^
Gumi::Gumi(bool left, Controller* controller, unsigned colorNumber):
	PlayerBase(left, controller, ColorNumToFile(colorNumber)),
	mInterFrom(MotionType::Idling), mInterTo(MotionType::NoMotion),
	mNowMotionFrame(0.0f), mMotionEnded(false),
	mNowMotionType(MotionType::Idling), mPrevMotionType(MotionType::Idling),
	mMoveSpeedX(), mMoveSpeedY(), mJumpedTwice(), mMoveBack(false)
{
	//���[�V�����f�[�^�̃��[�h
	mMotions[mInd_Idling].Load(L"Resource\\Motion\\Idling.vmd", FileFormat::VMD);
	mMotions[mInd_Walk].Load(L"Resource\\Motion\\Walk.vmd", FileFormat::VMD);
	mMotions[mInd_TurnAround].Load(L"Resource\\Motion\\TurnAround.vmd", FileFormat::VMD);
	mMotions[mInd_Dash].Load(L"Resource\\Motion\\Dash.vmd", FileFormat::VMD);
	mMotions[mInd_ToDash1].Load(L"Resource\\Motion\\ToDash1.vmd", FileFormat::VMD);
	mMotions[mInd_Brake1].Load(L"Resource\\Motion\\Brake1.vmd", FileFormat::VMD);
	mMotions[mInd_Brake2].Load(L"Resource\\Motion\\Brake2.vmd", FileFormat::VMD);
	mMotions[mInd_Brake2ToIdling].Load(L"Resource\\Motion\\Brake2ToIdling.vmd", FileFormat::VMD);
	mMotions[mInd_ToDash2].Load(L"Resource\\Motion\\ToDash2.vmd", FileFormat::VMD);
	mMotions[mInd_JumpPre].Load(L"Resource\\Motion\\JumpPre.vmd", FileFormat::VMD);
	mMotions[mInd_Jump1].Load(L"Resource\\Motion\\Jump1.vmd", FileFormat::VMD);
	mMotions[mInd_Jump2].Load(L"Resource\\Motion\\Jump2.vmd", FileFormat::VMD);
	mMotions[mInd_Air].Load(L"Resource\\Motion\\Air.vmd", FileFormat::VMD);
	mMotions[mInd_Landing].Load(L"Resource\\Motion\\Landing.vmd", FileFormat::VMD);
	mMotions[mInd_SideWeakPunch1].Load(L"Resource\\Motion\\Side_Weak_Punch1.vmd", FileFormat::VMD);
	mMotions[mInd_SideWeakPunch1ToIdling].Load(L"Resource\\Motion\\Side_Weak_Punch1_To_Idling.vmd", FileFormat::VMD);
	mMotions[mInd_SideWeakPunch2].Load(L"Resource\\Motion\\Side_Weak_Punch2.vmd", FileFormat::VMD);
	mMotions[mInd_SideWeakPunch2ToIdling].Load(L"Resource\\Motion\\Side_Weak_Punch2_To_Idling.vmd", FileFormat::VMD);
	mMotions[mInd_DashAttack1].Load(L"Resource\\Motion\\DashAttack1.vmd", FileFormat::VMD);
	mMotions[mInd_DashAttackImpact].Load(L"Resource\\Motion\\DashAttackImpact.vmd", FileFormat::VMD);
	mMotions[mInd_DashAttack2].Load(L"Resource\\Motion\\DashAttack2.vmd", FileFormat::VMD);
	mMotions[mInd_DashAttackToIdling].Load(L"Resource\\Motion\\DashAttackToIdling.vmd", FileFormat::VMD);
	mMotions[mInd_SideAttack1].Load(L"Resource\\Motion\\SideAttack1.vmd", FileFormat::VMD);
	mMotions[mInd_SideAttackImpact1].Load(L"Resource\\Motion\\SideAttackImpact1.vmd", FileFormat::VMD);
	mMotions[mInd_SideAttack2].Load(L"Resource\\Motion\\SideAttack2.vmd", FileFormat::VMD);
	mMotions[mInd_SideAttackImpact2].Load(L"Resource\\Motion\\SideAttackImpact2.vmd", FileFormat::VMD);
	mMotions[mInd_SideAttack3].Load(L"Resource\\Motion\\SideAttack3.vmd", FileFormat::VMD);
	mMotions[mInd_UpperAttack].Load(L"Resource\\Motion\\UpperAttack.vmd", FileFormat::VMD);
	mMotions[mInd_AerialSideWeakAttack].Load(L"Resource\\Motion\\AerialSideWeakAttack.vmd", FileFormat::VMD);
	mMotions[mInd_AerialLowerAttack].Load(L"Resource\\Motion\\AerialLowerAttack.vmd", FileFormat::VMD);
	mMotions[mInd_LowerAttackBackStep].Load(L"Resource\\Motion\\LowerAttackBackStep.vmd", FileFormat::VMD);
	mMotions[mInd_Guard].Load(L"Resource\\Motion\\Guard.vmd", FileFormat::VMD);
	mMotions[mInd_Damaged].Load(L"Resource\\Motion\\Damaged.vmd", FileFormat::VMD);
	mMotions[mInd_GreatDamaged].Load(L"Resource\\Motion\\GreatDamaged.vmd", FileFormat::VMD);

	//�T�E���h�f�[�^�̃��[�h
	mSoundEff[seInd_Landing].Load(L"Resource\\Sound\\GUMI\\landing.wav", FileFormat::WAVE);
	mSoundEff[seInd_Attack].Load(L"Resource\\Sound\\GUMI\\attack.wav", FileFormat::WAVE);
	mSoundEff[seInd_Damage].Load(L"Resource\\Sound\\GUMI\\damage.wav", FileFormat::WAVE);
	mSoundEff[seInd_GreatDamage].Load(L"Resource\\Sound\\GUMI\\greateDamage.wav", FileFormat::WAVE);
	mSoundEff[seInd_Jump].Load(L"Resource\\Sound\\GUMI\\jump.wav", FileFormat::WAVE);
	mSoundEff[seInd_UpperKick].Load(L"Resource\\Sound\\GUMI\\upperKick.wav", FileFormat::WAVE);
	mSoundEff[seInd_LegDrop].Load(L"Resource\\Sound\\GUMI\\legDrop.wav", FileFormat::WAVE);
	mSoundEff[seInd_Guard].Load(L"Resource\\Sound\\GUMI\\guard.wav", FileFormat::WAVE);
	mSoundEff[seInd_Footsteps].Load(L"Resource\\Sound\\GUMI\\footsteps.wav", FileFormat::WAVE);
	mSoundEff[seInd_Brake].Load(L"Resource\\Sound\\GUMI\\brake.wav", FileFormat::WAVE);

	//�R���W�����̐ݒ�
	mUpperBody.mRadius =2.5f;
	mLowerBody.mRadius =2.5f;
	mUpperBody.mAggression =false;
	mLowerBody.mAggression =false;

	mAggressors.resize(agInd_TheNumber);
	mAggressors[agInd_RightArm].mRadius =1.5f;
	mAggressors[agInd_RightArm].mAggression =false;
	mAggressors[agInd_LeftArm].mRadius =1.5f;
	mAggressors[agInd_LeftArm].mAggression =false;
	mAggressors[agInd_RightLeg].mRadius =3.5f;
	mAggressors[agInd_RightLeg].mAggression =false;
	mAggressors[agInd_LeftLeg].mRadius =3.5f;
	mAggressors[agInd_LeftLeg].mAggression =false;
}


// �f�X�g���N�^
Gumi::~Gumi()
{
	for(auto& se : mSoundEff)
	{
		se.Release();
	}
	for(auto& motion : mMotions)
	{
		motion.Release();
	}
}


// �����蔻��̍X�V
void Gumi::CollisionUpdate()
{
	using namespace math;
	auto gumiBones =mModel.GetBoneData(nullptr);

	Float3 pose{};
	QuaternionA qRotate;
	if(!mIsRight)
	{
		QuaternionRotationRollPitchYaw(&qRotate, 0.0f, ConvertToRadians(180.0f), 0.0f);
	}

	//��_���[�W
		//��
	VectorConvert(&pose, gumiBones[90].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mUpperBody.mSegment.mP1, pose, mPos);

		//�㔼�g
	VectorConvert(&pose, gumiBones[27].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mUpperBody.mSegment.mP2, pose, mPos);

		//�����g
	VectorConvert(&pose, gumiBones[4].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mLowerBody.mSegment.mP1, pose, mPos);

		//�S�Ă̐e
	VectorConvert(&pose, gumiBones[0].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	mLowerBody.mSegment.mP2 =mPos;

	//�^�_���[�W
		//�E�r
	VectorConvert(&pose, gumiBones[32].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_RightArm].mSegment.mP1, pose, mPos);

		//�E���
	VectorConvert(&pose, gumiBones[42].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_RightArm].mSegment.mP2, pose, mPos);
	
		//���r
	VectorConvert(&pose, gumiBones[61].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_LeftArm].mSegment.mP1, pose, mPos);

		//�����
	VectorConvert(&pose, gumiBones[71].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_LeftArm].mSegment.mP2, pose, mPos);

		//�E��
	VectorConvert(&pose, gumiBones[20].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_RightLeg].mSegment.mP1, pose, mPos);

		//�E����
	VectorConvert(&pose, gumiBones[22].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_RightLeg].mSegment.mP2, pose, mPos);

		//����
	VectorConvert(&pose, gumiBones[24].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_LeftLeg].mSegment.mP1, pose, mPos);

		//������
	VectorConvert(&pose, gumiBones[26].mPose(3));
	Vector3Rotate(&pose, pose, qRotate);
	VectorAdd(&mAggressors[agInd_LeftLeg].mSegment.mP2, pose, mPos);
}


// �Փ˂ւ̔���
void Gumi::CollideReaction()
{
#define LOW_IMPACT 1.5f
#define HIGH_IMPACT 3.0f

	if(mPhnomenon.mDamaged)
	{
		if(mNowMotionType == MotionType::Guard)
		{
			mMoveSpeedX =mPhnomenon.mDamagedFromRight ? mIsRight?-LOW_IMPACT:LOW_IMPACT : mIsRight?LOW_IMPACT:-LOW_IMPACT;
			if(mMoveSpeedX <= 0.0f) {mMoveBack =true;}
			mPhnomenon.mDamaged =false;
			mSoundEff[seInd_Guard].Start(false, 0, 0);
		}
	}
	else if(mPhnomenon.mGreatDamaged)
	{
		if(mNowMotionType == MotionType::Guard)
		{
			mMoveSpeedX =mPhnomenon.mDamagedFromRight ? mIsRight?-HIGH_IMPACT:HIGH_IMPACT : mIsRight?HIGH_IMPACT:-HIGH_IMPACT;
			if(mMoveSpeedX <= 0.0f) {mMoveBack =true;}
			mPhnomenon.mGreatDamaged =false;
			mSoundEff[seInd_Guard].Start(false, 0, 0);
		}
	}
}


// ���[�V�����^�C�v����Ή����郂�[�V�����C���f�b�N�X���擾
const Gumi::MotionIndex Gumi::GetMotionIndex(const MotionType mType) const
{
	MotionIndex result;

	switch(mType)
	{
	case MotionType::Idling:
		result =mInd_Idling;
		break;

	case MotionType::Walk:
		result =mInd_Walk;
		break;

	case MotionType::TurnAround:
		result =mInd_TurnAround;
		break;

	case MotionType::Dash:
		result =mInd_Dash;
		break;

	case MotionType::ToDash1:
		result =mInd_ToDash1;
		break;

	case MotionType::Brake1:
		result =mInd_Brake1;
		break;

	case MotionType::Brake2:
		result =mInd_Brake2;
		break;

	case MotionType::Brake2ToIdling:
		result =mInd_Brake2ToIdling;
		break;

	case MotionType::ToDash2:
		result =mInd_ToDash2;
		break;

	case MotionType::JumpPre:
		result =mInd_JumpPre;
		break;

	case MotionType::Jump1:
		result =mInd_Jump1;
		break;

	case MotionType::Jump2:
		result =mInd_Jump2;
		break;

	case MotionType::Air:
		result =mInd_Air;
		break;

	case MotionType::Landing:
		result =mInd_Landing;
		break;

	case MotionType::SideWeakPunch1:
		result =mInd_SideWeakPunch1;
		break;

	case MotionType::SideWeakPunch1ToIdling:
		result =mInd_SideWeakPunch1ToIdling;
		break;

	case MotionType::SideWeakPunch2:
		result =mInd_SideWeakPunch2;
		break;

	case MotionType::SideWeakPunch2ToIdling:
		result =mInd_SideWeakPunch2ToIdling;
		break;

	case MotionType::DashAttack1:
		result =mInd_DashAttack1;
		break;

	case MotionType::DashAttackImpact:
		result =mInd_DashAttackImpact;
		break;

	case MotionType::DashAttack2:
		result =mInd_DashAttack2;
		break;

	case MotionType::DashAttackToIdling:
		result =mInd_DashAttackToIdling;
		break;

	case MotionType::SideAttack1:
		result =mInd_SideAttack1;
		break;

	case MotionType::SideAttackImpact1:
		result =mInd_SideAttackImpact1;
		break;

	case MotionType::SideAttack2:
		result =mInd_SideAttack2;
		break;

	case MotionType::SideAttackImpact2:
		result =mInd_SideAttackImpact2;
		break;

	case MotionType::SideAttack3:
		result =mInd_SideAttack3;
		break;

	case MotionType::UpperAttack:
		result =mInd_UpperAttack;
		break;

	case MotionType::Guard:
		result =mInd_Guard;
		break;

	case MotionType::Damaged:
		result =mInd_Damaged;
		break;
		
	case MotionType::GreatDamaged:
		result =mInd_GreatDamaged;
		break;

	default:
		result =mInd_Unknown;
		break;
	}

	return result;
}


// ��ԃ��[�V�����̐ݒ�
void Gumi::SetInterMotion(MotionType from, MotionType to, unsigned frame, MotionType interType)
{
	CWE_ASSERT(interType == MotionType::Inter || interType == MotionType::InterNoIncpt);

	mInterFrom =(GetMotionIndex(from) != mInd_Unknown) ? from : mInterTo;
	mInterTo =to;
	mInter.Reset(frame);
	mNowMotionType =interType;
}


// ���[�V�����̍Đ�
void Gumi::PlayMotion(CWE::Motion& motion)
{
	if(mNowMotionType != mPrevMotionType) //�ύX����Ă�����
	{
		motion.Restart();
	}

	if(!mModel.MotionUpdate(motion, mNowMotionFrame)) //�Đ����I��������
	{
		if(mNowMotionType == MotionType::Idling ||
			mNowMotionType == MotionType::Dash ||
			mNowMotionType == MotionType::Walk)
		{
			motion.Restart();
			mNowMotionFrame =0.0f;
		}
		mMotionEnded =true;
	}
}


// ��Ԃ̍Đ�
void Gumi::PlayInter(CWE::Motion& from, CWE::Motion& to)
{
	if(!mInter.Exec(mModel, from, to)) //�Đ����I��������
	{
		mMotionEnded =true;
	}
}


// ���͂ɑ΂��郂�[�V�����̕ύX
void Gumi::InputReaction(RequestMotion& request)
{
	// ���[�V�����̕ύX
	if(GetController()->LowerAttack()) {request =RequestMotion::LowerAttack;}
	else if(GetController()->UpperAttack()) {request =RequestMotion::UpperAttack;}
	else if(GetController()->SideAttack(mIsRight)) {request =RequestMotion::SideAttack;}
	else if(GetController()->SideWeakAttack()) {request =RequestMotion::SideWeakAttack;}
	else if(GetController()->Guard()) {request =RequestMotion::Guard;}
	else if(GetController()->Jump()) {request =RequestMotion::Jump;}
	else if(GetController()->Reverse(mIsRight)) {request =RequestMotion::TurnAround;}
	else if(GetController()->Dash(mIsRight)) {request =RequestMotion::Dash;}
	else if(GetController()->Walk(mIsRight)) {request =RequestMotion::Walk;}
	else {request =RequestMotion::ToNext;}
}


// �󓮓I�ɋN���郂�[�V�����̕ύX
void Gumi::PassiveReaction(PassiveRequest& request)
{
	if(mPhnomenon.mDamaged)
	{
		request =PassiveRequest::Damaged;
		mHP -=smDamage;
		mPhnomenon.mDamaged =false;
	}
	else if(mPhnomenon.mGreatDamaged)
	{
		request =PassiveRequest::GreatDamaged;
		mHP -=smDamage *7;
		mPhnomenon.mGreatDamaged =false;
	}
	else if((mNowMotionType == MotionType::Jump1 || mNowMotionType == MotionType::Jump2 || mNowMotionType == MotionType::Air || mNowMotionType == MotionType::AerialLowerAttack) &&
		mPos.y() <= 0.0f)
	{
		request =PassiveRequest::Landing;
	}
}


#define WALK_SPEED 0.15f
#define DASH_SPEED 2.00f
#define BRAKE_FRICTION 1.5f
#define WEIGHT_REF_VAL 30.0f

// ���[�V�����̍X�V
void Gumi::MotionUpdate(bool& motionMoveX, bool& motionMoveY, float& friction)
{
	if(mNowMotionType != mPrevMotionType) //���[�V�������؂�ւ�����ꍇ
	{
		mNowMotionFrame =0.0f;
	}
	else
	{
		mNowMotionFrame +=0.5f;
	}

	//���[�V�����̍Đ�
	switch(mNowMotionType)
	{
	//�A�C�h�����O
	case MotionType::Idling:
		PlayMotion(mMotions[mInd_Idling]);
		break;

	//���s
	case MotionType::Walk:
		PlayMotion(mMotions[mInd_Walk]);
		if(math::NearlyEqual(13.000, mMotions[mInd_Walk].GetNowFrame()))
		{
			mSoundEff[seInd_Footsteps].Start(false, 0, 0);
		}
		else if(math::NearlyEqual(28.000, mMotions[mInd_Walk].GetNowFrame()))
		{
			mSoundEff[seInd_Footsteps].Start(false, 0, 0);
		}
		mMoveSpeedX =WALK_SPEED;
		motionMoveX =true;
		break;

	//�U�����
	case MotionType::TurnAround:
		PlayMotion(mMotions[mInd_TurnAround]);
		break;

	//���s
	case MotionType::Dash:
		PlayMotion(mMotions[mInd_Dash]);
		if(math::NearlyEqual(6.000, mMotions[mInd_Dash].GetNowFrame()))
		{
			mSoundEff[seInd_Footsteps].Start(false, 0, 0);
		}
		else if(math::NearlyEqual(14.000, mMotions[mInd_Dash].GetNowFrame()))
		{
			mSoundEff[seInd_Footsteps].Start(false, 0, 0);
		}
		mMoveSpeedX =DASH_SPEED;
		motionMoveX =true;
		break;

	//���s�O�̏R��o��1
	case MotionType::ToDash1:
		PlayMotion(mMotions[mInd_ToDash1]);
		mMoveSpeedX =DASH_SPEED +1.0f;
		motionMoveX =true;
		break;

	//���̏�u���[�L
	case MotionType::Brake1:
		if(!(mMoveSpeedX <= 0.0f)) //��~���Ă��Ȃ�������
		{
			mNowMotionFrame =0.0f;
		}
		PlayMotion(mMotions[mInd_Brake1]);
		friction =BRAKE_FRICTION;
		break;

	//�U������u���[�L
	case MotionType::Brake2:
		if(9.0f <= mNowMotionFrame && !(mMoveSpeedX <= 0.0f)) //����̃t���[���ɒB���Ă��Ē�~���Ă��Ȃ�������
		{
			mNowMotionFrame -=0.5; //���[�V�������~
		}
		PlayMotion(mMotions[mInd_Brake2]);
		friction =BRAKE_FRICTION;
		break;

	//�U������u���[�L����A�C�h�����O
	case MotionType::Brake2ToIdling:
		PlayMotion(mMotions[mInd_Brake2ToIdling]);
		break;

	//���s�O�̏R��o��2
	case MotionType::ToDash2:
		PlayMotion(mMotions[mInd_ToDash2]);
		mMoveSpeedX =DASH_SPEED +1.0f;
		motionMoveX =true;
		break;

	//�W�����v�̗\������
	case MotionType::JumpPre:
		PlayMotion(mMotions[mInd_JumpPre]);
		break;

	//�W�����v1�i
	case MotionType::Jump1:
		PlayMotion(mMotions[mInd_Jump1]);
		break;

	//�W�����v2�i
	case MotionType::Jump2:
		PlayMotion(mMotions[mInd_Jump2]);
		break;

	//��
	case MotionType::Air:
		if(mPos.y()) //���n���Ă��Ȃ�������
		{
			mNowMotionFrame =0.0f;
		}
		PlayMotion(mMotions[mInd_Air]);
		break;

	//���n
	case MotionType::Landing:
		PlayMotion(mMotions[mInd_Landing]);
		friction =BRAKE_FRICTION;
		break;

	//����U��1
	case MotionType::SideWeakPunch1:
		PlayMotion(mMotions[mInd_SideWeakPunch1]);
		mAggressors[agInd_LeftArm].mAggression =true;
		break;

	//����U��1���
	case MotionType::SideWeakPunch1ToIdling:
		PlayMotion(mMotions[mInd_SideWeakPunch1ToIdling]);
		break;

	//����U��2
	case MotionType::SideWeakPunch2:
		PlayMotion(mMotions[mInd_SideWeakPunch2]);
		mAggressors[agInd_RightArm].mAggression =true;
		break;

	//����U��2���
	case MotionType::SideWeakPunch2ToIdling:
		PlayMotion(mMotions[mInd_SideWeakPunch2ToIdling]);
		break;

	//�_�b�V���U��1
	case MotionType::DashAttack1:
		PlayMotion(mMotions[mInd_DashAttack1]);
		friction =0.4f;
		break;

	//�_�b�V���U���C���p�N�g
	case MotionType::DashAttackImpact:
		PlayMotion(mMotions[mInd_DashAttackImpact]);
		mAggressors[agInd_RightArm].mAggression =true;
		friction =0.7f;
		break;

	//�_�b�V���U��2
	case MotionType::DashAttack2:
		PlayMotion(mMotions[mInd_DashAttack2]);
		friction =1.5f;
		break;

	//�_�b�V���U�����
	case MotionType::DashAttackToIdling:
		PlayMotion(mMotions[mInd_DashAttackToIdling]);
		break;

	//���U��1
	case MotionType::SideAttack1:
		PlayMotion(mMotions[mInd_SideAttack1]);
		break;

	//���U���C���p�N�g1
	case MotionType::SideAttackImpact1:
		PlayMotion(mMotions[mInd_SideAttack1]);
		mAggressors[agInd_RightArm].mAggression =true;
		break;

	//���U��2
	case MotionType::SideAttack2:
		PlayMotion(mMotions[mInd_SideAttack2]);
		break;

	//���U���C���p�N�g2
	case MotionType::SideAttackImpact2:
		PlayMotion(mMotions[mInd_SideAttackImpact2]);
		mAggressors[agInd_LeftArm].mAggression =true;
		break;

	//���U��2
	case MotionType::SideAttack3:
		PlayMotion(mMotions[mInd_SideAttack3]);
		break;

	//��U��
	case MotionType::UpperAttack:
		PlayMotion(mMotions[mInd_UpperAttack]);
		if(math::NearlyEqual(mMotions[mInd_UpperAttack].GetNowFrame(), 2.5f)) //1��ڂ̏㏸
		{
			mMoveSpeedY =3.0f;
			mSoundEff[seInd_UpperKick].Start(false, 0, 0);
		}
		else if(math::NearlyEqual(mMotions[mInd_UpperAttack].GetNowFrame(), 5.5f)) //2��ڂ̏㏸
		{
			mMoveSpeedY =5.5f;
			mSoundEff[seInd_UpperKick].Start(false, 0, 0);
		}
		else
		{
			if(mMoveSpeedY <= 0.0f)
			{
				mMoveSpeedY =0.0f;
			}
		}
		if(3.5f <= mMotions[mInd_UpperAttack].GetNowFrame() && mMotions[mInd_UpperAttack].GetNowFrame() <= 4.5f) //1��ڂ̃C���p�N�g
		{
			mAggressors[agInd_LeftLeg].mAggression =true;
		}
		else if(6.0f <= mMotions[mInd_UpperAttack].GetNowFrame() && mMotions[mInd_UpperAttack].GetNowFrame() <= 7.0f) //2��ڂ̃C���p�N�g
		{
			mAggressors[agInd_RightLeg].mAggression =true;
		}
		break;

	//�󒆉��U��
	case MotionType::AerialLowerAttack:
		PlayMotion(mMotions[mInd_AerialLowerAttack]);
		if(mMotions[mInd_AerialLowerAttack].GetNowFrame() <= 2.0f)
		{
			mMoveSpeedY =1.0f;
		}
		else
		{
			 mMoveSpeedY =-5.0f;
			 mAggressors[agInd_RightLeg].mAggression =true;
		}
		break;

	//�󒆉��U���o�b�N�X�e�b�v
	case MotionType::LowerAttackBackStep:
		PlayMotion(mMotions[mInd_LowerAttackBackStep]);
		if(2.0f <= mMotions[mInd_LowerAttackBackStep].GetNowFrame())
		{
			mMoveSpeedX =-0.7f;
			motionMoveX =true;
		}
		break;

	//�K�[�h
	case MotionType::Guard:
		PlayMotion(mMotions[mInd_Guard]);
		break;

	//�_���[�W
	case MotionType::Damaged:
		PlayMotion(mMotions[mInd_Damaged]);
		break;

	//��_���[�W
	case MotionType::GreatDamaged:
		PlayMotion(mMotions[mInd_GreatDamaged]);
		break;

	//��Ԍn��
	case MotionType::Inter:
	case MotionType::InterNoIncpt:
		PlayInter(mMotions[GetMotionIndex(mInterFrom)], mMotions[GetMotionIndex(mInterTo)]);
		break;
	}
}


#define INTER_FRAME 5

//���[�V�����̈ڍs
void Gumi::MotionChange(RequestMotion requested, PassiveRequest psRequested)
{
#define D_LOW_IMPACT 0.5f
#define D_HIGH_IMPACT 4.0f

	//�ύX�v���̎��s
	switch(psRequested)
	{
	//���n
	case PassiveRequest::Landing:
		if(mNowMotionType == MotionType::AerialLowerAttack)
		{
			mNowMotionType =MotionType::LowerAttackBackStep;
			mSoundEff[seInd_LegDrop].Start(false, 0, 0);
			mMotionEnded =false;
		}
		break;

	//�_���[�W
	case PassiveRequest::Damaged:
		if(mNowMotionType != MotionType::Damaged &&
			mNowMotionType != MotionType::GreatDamaged &&
			mNowMotionType != MotionType::Guard)
		{
			mNowMotionType =MotionType::Damaged;
			mSoundEff[seInd_Damage].Start(false, 0, 0);
			if(mPhnomenon.mDamagedFromRight ? mIsRight?false:true : mIsRight?true:false)
			{
				mIsRight =!mIsRight;
			}
			mMoveSpeedX =-D_LOW_IMPACT;
			if(mMoveSpeedX <= 0.0f) {mMoveBack =true;}
		}
		break;

	//��_���[�W
	case PassiveRequest::GreatDamaged:
		if(mNowMotionType != MotionType::Damaged &&
			mNowMotionType != MotionType::GreatDamaged &&
			mNowMotionType != MotionType::Guard)
		{
			mNowMotionType =MotionType::GreatDamaged;
			mSoundEff[seInd_GreatDamage].Start(false, 0, 0);
			if(mPhnomenon.mDamagedFromRight ? mIsRight?false:true : mIsRight?true:false)
			{
				mIsRight =!mIsRight;
			}
			mMoveSpeedX =-D_HIGH_IMPACT;
			if(mMoveSpeedX <= 0.0f) {mMoveBack =true;}
		}
		break;
	}

	if(0 <= static_cast<int>(mNowMotionType)) //�ύX�\�ȃ��[�V�����ł����
	{
		if(mPos.y() <= 0.0f) //�n��ɂ�����
		{
			//�󓮓I���[�V�������N�G�X�g�ւ̈ڍs
			switch(psRequested)
			{
			//���n
			case PassiveRequest::Landing:
				if(mNowMotionType == MotionType::AerialLowerAttack)
				{
					mNowMotionType =MotionType::LowerAttackBackStep;
				}
				else
				{
					mNowMotionType =MotionType::Landing;
					mSoundEff[seInd_Landing].Start(false, 0, 0);
				}
				break;

			default:
				//���̓��N�G�X�g����
				GroundMotionRespond(requested, psRequested);
				break;
			}
		}
		else
		{
			//�󓮓I���[�V�������N�G�X�g�ւ̈ڍs
			switch(psRequested)
			{
			default:
				//���̓��N�G�X�g����
				AerialMotionRespond(requested);
				break;
			}
		}
	}

	if(mMotionEnded)
	{
		//���[�V�������I���������̎����ڍs
		switch(mNowMotionType)
		{
		//���荞�݋֎~�̕��
		case MotionType::InterNoIncpt:
			mNowMotionType =mInterTo; //��Ԑ�ւ��̂܂܈ڍs
			mInterFrom =mInterTo;
			break;

		//���
		case MotionType::Inter:
			mNowMotionType =mInterTo; //��Ԑ�ւ��̂܂܈ڍs
			mInterFrom =mInterTo;
			break;

		//�U�����
		case MotionType::TurnAround:
			mNowMotionType =MotionType::Idling;
			break;

		//���s�O�̏R��o��1
		case MotionType::ToDash1:
			mNowMotionType =MotionType::Dash;
			break;

		//���̏�u���[�L
		case MotionType::Brake1:
			SetInterMotion(mPrevMotionType, MotionType::Idling, 5, MotionType::Inter);
			break;

		//�U������u���[�L
		case MotionType::Brake2:
			if(requested == RequestMotion::TurnAround) //�t�����v�����p���I�ɂ���Ă�����
			{
				mNowMotionType =MotionType::ToDash2;
			}
			else
			{
				mNowMotionType =MotionType::Brake2ToIdling;
			}
			mIsRight =!mIsRight;
			break;

		//�U������u���[�L����A�C�h�����O
		case MotionType::Brake2ToIdling:
			mNowMotionType =MotionType::Idling;
			break;

		//���s�O�̏R��o��2
		case MotionType::ToDash2:
			mNowMotionType =MotionType::Dash;
			break;

		//�W�����v�̗\������
		case MotionType::JumpPre:
			mMoveSpeedY =smJumpingAbility1 *(smWeight /WEIGHT_REF_VAL);
			mNowMotionType =MotionType::Jump1;
			mSoundEff[seInd_Jump].Start(false, 0, 0);
			break;

		//�W�����v1�i��
		case MotionType::Jump1:
			mNowMotionType =MotionType::Air;
			break;

		//�W�����v2�i��
		case MotionType::Jump2:
			mNowMotionType =MotionType::Air;
			break;

		//���n
		case MotionType::Landing:
			SetInterMotion(MotionType::Landing, MotionType::Idling, 5, MotionType::Inter);
			break;

		//����U��1
		case MotionType::SideWeakPunch1:
			mNowMotionType =MotionType::SideWeakPunch1ToIdling;
			mSoundEff[seInd_Attack].Start(false, 0, 0);
			break;

		//����U��1���
		case MotionType::SideWeakPunch1ToIdling:
			mNowMotionType =MotionType::Idling;
			break;

		//����U��2
		case MotionType::SideWeakPunch2:
			mNowMotionType =MotionType::SideWeakPunch2ToIdling;
			mSoundEff[seInd_Attack].Start(false, 0, 0);
			break;

		//����U��2���
		case MotionType::SideWeakPunch2ToIdling:
			mNowMotionType =MotionType::Idling;
			break;

		//�_�b�V���U��1
		case MotionType::DashAttack1:
			mNowMotionType =MotionType::DashAttackImpact;
			mSoundEff[seInd_Attack].Start(false, 0, 0);
			break;

		//�_�b�V���U���C���p�N�g
		case MotionType::DashAttackImpact:
			mNowMotionType =MotionType::DashAttack2;
			break;

		//�_�b�V���U��2
		case MotionType::DashAttack2:
			mNowMotionType =MotionType::DashAttackToIdling;
			break;

		//�_�b�V���U�����
		case MotionType::DashAttackToIdling:
			mNowMotionType =MotionType::Idling;
			break;

		//���U��1
		case MotionType::SideAttack1:
			mNowMotionType =MotionType::SideAttackImpact1;
			mSoundEff[seInd_Attack].Start(false, 0, 0);
			break;

		//���U���C���p�N�g1
		case MotionType::SideAttackImpact1:
			mNowMotionType =MotionType::SideAttack2;
			break;

		//���U��2
		case MotionType::SideAttack2:
			mNowMotionType =MotionType::SideAttackImpact2;
			mSoundEff[seInd_Attack].Start(false, 0, 0);
			break;

		//���U���C���p�N�g2
		case MotionType::SideAttackImpact2:
			mNowMotionType =MotionType::SideAttack3;
			break;

		//���U��2
		case MotionType::SideAttack3:
			SetInterMotion(MotionType::SideAttack3, MotionType::Idling, 5, MotionType::Inter);
			break;

		//��U��
		case MotionType::UpperAttack:
			SetInterMotion(MotionType::UpperAttack, MotionType::Air, 10, MotionType::Inter);
			mJumpedTwice =true;
			mAerialAttacked =true;
			break;

		//�󒆉��U��
		case MotionType::AerialLowerAttack:
			//���n��ҋ@
			break;

		//�󒆉��U���o�b�N�X�e�b�v
		case MotionType::LowerAttackBackStep:
			SetInterMotion(MotionType::LowerAttackBackStep, MotionType::Idling, 10, MotionType::Inter);
			break;

		//�K�[�h
		case MotionType::Guard:
			SetInterMotion(mNowMotionType, MotionType::Idling, 20, MotionType::Inter);
			break;

		//�_���[�W
		case MotionType::Damaged:
			SetInterMotion(mNowMotionType, MotionType::Idling, 10, MotionType::Inter);
			break;

		//��_���[�W
		case MotionType::GreatDamaged:
			SetInterMotion(mNowMotionType, MotionType::Idling, 10, MotionType::Inter);
			break;

		//���Ɏw�肪�Ȃ���Ή������Ȃ�
		default:
			break;
		}
	}

	mMotionEnded =false;
}


// �n��ł̃��[�V�����̈ڍs�v������
void Gumi::GroundMotionRespond(RequestMotion requested, PassiveRequest psReqested)
{
	//�ύX���ɍs������
	switch(requested)
	{
	//��U��
	case RequestMotion::UpperAttack:
		if(!mUpperAttacked)
		{
			SetInterMotion(mNowMotionType, MotionType::UpperAttack, 2, MotionType::InterNoIncpt);
		}
		break;

	//���U��
	case RequestMotion::SideAttack:
		mNowMotionType =MotionType::SideAttack1;
		break;

	//����U��
	case RequestMotion::SideWeakAttack:
		if(mNowMotionType == MotionType::Dash) //�_�b�V���U��
		{
			mNowMotionType =MotionType::DashAttack1;
		}
		else if(mNowMotionType == MotionType::SideWeakPunch1ToIdling) //2����
		{
			mNowMotionType =MotionType::SideWeakPunch2;
		}
		else
		{
			mNowMotionType =MotionType::SideWeakPunch1;
		}
		break;

	//�K�[�h
	case RequestMotion::Guard:
		if(!(mNowMotionType == MotionType::Inter && mInterFrom == MotionType::Guard))
		{
			mNowMotionType =MotionType::Guard;
		}
		break;

	//���s
	case RequestMotion::Walk:
		if(mNowMotionType == MotionType::Idling)
		{
			mNowMotionType =MotionType::Walk;
		}
		break;

	//�U�����
	case RequestMotion::TurnAround:
		if(WALK_SPEED < mMoveSpeedX) //�����Ă�����
		{
			//�U������u���[�L��
			SetInterMotion(mPrevMotionType, MotionType::Brake2, 3, MotionType::InterNoIncpt);
			mSoundEff[seInd_Brake].Start(false, 0, 0);
		}
		else
		{
			mNowMotionType =MotionType::TurnAround;
			mIsRight =!mIsRight;
		}
		break;

	//�_�b�V��
	case RequestMotion::Dash:
		if(mNowMotionType != MotionType::Dash &&
			mNowMotionType != MotionType::ToDash1)
		{
			mNowMotionType =MotionType::ToDash1;
		}
		else if(mNowMotionType == MotionType::Brake2ToIdling)
		{
			mNowMotionType =MotionType::ToDash2;
		}
		else //psReqested == PassiveRequest::Landing
		{
			mNowMotionType =MotionType::Dash;
		}
		break;

	//�W�����v
	case RequestMotion::Jump:
		if(mNowMotionType != MotionType::JumpPre &&
			(mNowMotionType != MotionType::Inter) ? mInterTo != MotionType::JumpPre : true)
		{
			SetInterMotion(mPrevMotionType, MotionType::JumpPre, 1, MotionType::InterNoIncpt);
		}
		break;

	//���̃��[�V������
	case RequestMotion::ToNext:
		//���̃��[�V�����̑I��
		switch(mPrevMotionType)
		{
		//���s����Ȃ�
		case MotionType::Walk:
			SetInterMotion(mPrevMotionType, MotionType::Idling, 5, MotionType::Inter);
			break;

		//���s����Ȃ�
		case MotionType::Dash:
			SetInterMotion(mPrevMotionType, MotionType::Brake1, 4, MotionType::Inter);
			mSoundEff[seInd_Brake].Start(false, 0, 0);
			break;

		//�����͂̎��ɑJ�ڂ��郂�[�V�������Ȃ��ꍇ
		default:
			mNowMotionType =mPrevMotionType;
			break;
		}
		break;

	//�����Ȃ������牽�����Ȃ�
	default:
		break;
	}
}


// �󒆂ł̃��[�V�����̈ڍs�v������
void Gumi::AerialMotionRespond(RequestMotion requested)
{
	//�ύX���ɍs������
	switch(requested)
	{
	//���U��
	case RequestMotion::LowerAttack:
		mNowMotionType =MotionType::AerialLowerAttack;
		break;

	//�W�����v
	case RequestMotion::Jump:
		if(!mJumpedTwice)
		{
			mNowMotionType =MotionType::Jump2;
			mMoveSpeedY =smJumpingAbility2 *(smWeight /WEIGHT_REF_VAL);
			mSoundEff[seInd_Jump].Start(false, 0, 0);
		}
		mJumpedTwice =true;
		break;

	//���̃��[�V������
	case RequestMotion::ToNext:
		switch(mPrevMotionType)
		{
		//�����͂̎��ɑJ�ڂ��郂�[�V�������Ȃ��ꍇ
		default:
			mNowMotionType =mPrevMotionType;
			break;
		}
		break;

	//�����Ȃ������牽�����Ȃ�
	default:
		break;
	}
}


// X���ړ�
void Gumi::MoveX(bool motionMove, float friction)
{
	//�����x�̎Z�o
	if(mPos.y() <= 0.0f) //�n��ɂ�����
	{
		if(!motionMove)
		{
			if(mMoveBack)
			{
				mMoveSpeedX +=(Stage::smGroundFriction *friction) *(smWeight /WEIGHT_REF_VAL);

				if(0.0f <= mMoveSpeedX)
				{
					mMoveSpeedX =0.0f;

					mMoveBack =false;
				}
			}
			else
			{
				mMoveSpeedX -=(Stage::smGroundFriction *friction) *(smWeight /WEIGHT_REF_VAL);

				if(mMoveSpeedX <= 0.0f)
				{
					mMoveSpeedX =0.0f;
				}
			}
		}
	}
	else
	{
		if(GetController()->Dash(mIsRight))
		{
			mMoveSpeedX =DASH_SPEED /1.5f;
		}
		else if(GetController()->Walk(mIsRight))
		{
			mMoveSpeedX =WALK_SPEED /1.5f;
		}
		else if(GetController()->Back(mIsRight))
		{
			mMoveSpeedX =-DASH_SPEED /1.5f;
		}
		else if(GetController()->SlightlyBack(mIsRight))
		{
			mMoveSpeedX =-WALK_SPEED /1.5f;
		}
		else if(mMoveSpeedX <= 0.0f)
		{
			mMoveSpeedX +=(Stage::smGroundFriction *0.5f) *(smWeight /WEIGHT_REF_VAL);
		}
		else
		{
			mMoveSpeedX -=(Stage::smGroundFriction *0.5f) *(smWeight /WEIGHT_REF_VAL);
		}
	}

	//���x�̓K�p
	if(mIsRight)
	{
		mPos.x() +=mMoveSpeedX;
	}
	else
	{
		mPos.x() -=mMoveSpeedX;
	}
}


#define Y_MOVESPEED_LIMIT 4.0f

// Y���ړ�
void Gumi::MoveY()
{
	mMoveSpeedY -=Stage::smGravityAcceleration;
	if(GetController()->Down())
	{
		mPos.y() -=1.0f;
	}

	if(Y_MOVESPEED_LIMIT <= mMoveSpeedY)
	{
		mPos.y() +=Y_MOVESPEED_LIMIT;
	}
	else if(mMoveSpeedY <= -Y_MOVESPEED_LIMIT)
	{
		mPos.y() -=Y_MOVESPEED_LIMIT;
	}
	else
	{
		mPos.y() +=mMoveSpeedY;
	}

	if(mPos.y() < 0.0f)
	{
		mPos.y() =0.0f;
	}
}

// ���[�h�̊�����ʒm
bool Gumi::LoadSuccess() const
{
	//���s����
	if(mModel.LoadResult() == Task::Failed)
	{
		FatalError::GetInstance().Outbreak(smFailedModelMs);
		return false;
	}
	for(const auto& motion : mMotions)
	{
		if(motion.LoadResult() == Task::Failed)
		{
			FatalError::GetInstance().Outbreak(smFailedMotionMs);
			return false;
		}
	}

	//��������
	if(mModel.LoadResult() != Task::Success)
	{
		return false;
	}

	for(const auto& motion : mMotions)
	{
		if(motion.LoadResult() != Task::Success)
		{
			return false;
		}
	}

	for(const auto& se : mSoundEff)
	{
		if(se.LoadResult() != Task::Success)
		{
			return false;
		}
	}

	return true;
}


// ���݂̃��[�V���������U�����ǂ���
bool Gumi::IsHighImpact() const
{
	bool result =false;

	switch(mNowMotionType)
	{
	case MotionType::DashAttackImpact:
		result =true;
		break;

	case MotionType::AerialLowerAttack:
		result =true;
		break;

	case MotionType::SideAttackImpact2:
		result =true;
		break;

	case MotionType::UpperAttack:
		result =true;
		break;
	}

	return result;
}


// �X�V
void Gumi::Update()
{
	CWE_ASSERT(GetController());

	//������
	RequestMotion request;
	PassiveRequest psRequest;
	bool motionMoveX =false,
		 motionMoveY =false;
	float friction =1.0f;
	if(mPos.y()<=0.0f)
	{
		mJumpedTwice =false; //2�i�W�����v�t���O�̃��Z�b�g
		mAerialAttacked =false; //�󒆂ōU���t���O�̃��Z�b�g
		mUpperAttacked =false; //��U���t���O�̃��Z�b�g
	}
	for(auto& aggressor : mAggressors)
	{
		aggressor.mAggression =false;
	}

	//�Փ˂ւ̔���
	CollideReaction();

	//���͂ɑ΂��郂�[�V�����̕ύX
	InputReaction(request);

	//�󓮓I�ɋN���郂�[�V�����̕ύX
	PassiveReaction(psRequest);

	//���[�V�����̈ڍs
	MotionChange(request, psRequest);

	//���[�V�����̍X�V
	MotionUpdate(motionMoveX, motionMoveY, friction);

	//�ړ�����
	MoveY();
	MoveX(motionMoveX, friction);

	//�㏈��
	mPrevMotionType =mNowMotionType;

	//�`��p�v�Z
	using namespace math;
	QuaternionA qPose;
	MatrixIdentity(mLocal);

	if(!mIsRight)
	{
		QuaternionRotationRollPitchYaw(&qPose, 0.0f, ConvertToRadians(180.0f), 0.0f);
	}
	MatrixTranslationR(&mLocal, mPos, qPose);

	//�����蔻��̍X�V
	CollisionUpdate();
}