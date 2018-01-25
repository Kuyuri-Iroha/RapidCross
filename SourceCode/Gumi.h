#pragma once

#include "Player.h"


// GUMI
class Gumi final  : public PlayerBase
{
public:
	// ���[�V�����̃��N�G�X�g
	enum class RequestMotion : int
	{
		Walk,					//���s
		TurnAround,				//�U�����
		Dash,					//���s
		Jump,					//�W�����v
		Guard,					//�K�[�h

		SideWeakAttack,			//����U��
		SideAttack,				//���U��
		UpperAttack,			//��U��
		LowerAttack,			//���U��

		ToNext,					//����
	};

	// ���[�V�����̎󓮓I�ȃ��N�G�X�g
	enum class PassiveRequest : int
	{
		Landing,				//���n
		Damaged,				//�_���[�W
		GreatDamaged,			//��_���[�W

	};

	// ���[�V�����̎��
	enum class MotionType : int
	{
		InterNoIncpt =-INT_MIN,	//���荞�݋֎~�̕��
		JumpPre,				//�W�����v�̗\������
		Brake1,					//�u���[�L
		Brake2,					//�U������u���[�L
		ToDash2,				//���s�O�̏R��o��2
		TurnAround,				//�U�����
		SideWeakPunch1,			//����U��1
		SideWeakPunch2,			//����U��2
		DashAttack1,			//�_�b�V���U��1
		DashAttackImpact,		//�_�b�V���U���C���p�N�g
		DashAttack2,			//�_�b�V���U��2
		SideAttack1,			//���U��1
		SideAttackImpact1,		//���U���C���p�N�g1
		SideAttack2,			//���U��2
		SideAttackImpact2,		//���U���C���p�N�g2
		SideAttack3,			//���U��3
		UpperAttack,			//��U��
		AerialLowerAttack,		//�󒆉��U��
		LowerAttackBackStep,	//���U���o�b�N�X�e�b�v
		Guard,					//�K�[�h
		Damaged,				//�_���[�W
		GreatDamaged,			//��_���[�W

		NoMotion =0,			//���[�V�����Ȃ�

		Idling,					//�A�C�h�����O
		Walk,					//���s
		Dash,					//���s
		ToDash1,				//���s�O�̏R��o��1
		Brake2ToIdling,			//�U������u���[�L����A�C�h�����O
		Jump1,					//�W�����v1
		Jump2,					//�W�����v2
		Air,					//��
		Landing,				//���n

		SideWeakPunch1ToIdling,	//����U��1����A�C�h�����O��
		SideWeakPunch2ToIdling,	//����U��2����A�C�h�����O��
		DashAttackToIdling,		//�_�b�V���U������A�C�h�����O��
		Inter,					//���
	};

	// ���[�V�����z��̃C���f�b�N�X
	enum MotionIndex : unsigned
	{
		mInd_Idling,
		mInd_Walk,
		mInd_TurnAround,
		mInd_Dash,
		mInd_ToDash1,
		mInd_Brake1,
		mInd_Brake2,
		mInd_Brake2ToIdling,
		mInd_ToDash2,
		mInd_JumpPre,
		mInd_Jump1,
		mInd_Jump2,
		mInd_Air,
		mInd_Landing,

		mInd_SideWeakPunch1,
		mInd_SideWeakPunch1ToIdling,
		mInd_SideWeakPunch2,
		mInd_SideWeakPunch2ToIdling,
		mInd_DashAttack1,
		mInd_DashAttackImpact,
		mInd_DashAttack2,
		mInd_DashAttackToIdling,
		mInd_SideAttack1,
		mInd_SideAttackImpact1,
		mInd_SideAttack2,
		mInd_SideAttackImpact2,
		mInd_SideAttack3,
		mInd_UpperAttack,
		mInd_AerialSideWeakAttack,
		mInd_AerialLowerAttack,
		mInd_LowerAttackBackStep,
		mInd_Guard,
		mInd_Damaged,
		mInd_GreatDamaged,

		mInd_TheNumber,
		mInd_Unknown
	};

	// �^�_���[�W�R���W�����z��C���f�b�N�X
	enum AggressorIndex : unsigned
	{
		agInd_RightArm,
		agInd_LeftArm,
		agInd_RightLeg,
		agInd_LeftLeg,
		agInd_TheNumber,
		agInd_Unknown
	};

	// ���ʉ��z��C���f�b�N�X
	enum SoundEffIndex : unsigned
	{
		seInd_Landing,
		seInd_Attack,
		seInd_Damage,
		seInd_GreatDamage,
		seInd_Jump,
		seInd_UpperKick,
		seInd_LegDrop,
		seInd_Guard,
		seInd_Footsteps,
		seInd_Brake,
		seInd_TheNumber,
		seInd_Unknown
	};

private:
	// �L�����N�^�[�̕������
	static const float smWeight,
					   smJumpingAbility1,
					   smJumpingAbility2;
	static const int smDamage =5;

	// ���[�V�����f�[�^
	std::array<CWE::Motion, mInd_TheNumber> mMotions;
	// ���[�V������ԃI�u�W�F�N�g
	MotionType mInterFrom, //��Ԍ����[�V����
			   mInterTo; //��Ԑ惂�[�V����

	// �T�E���h�f�[�^
	std::array<CWE::Sound, seInd_TheNumber> mSoundEff;

	float mNowMotionFrame; //���݂̃��[�V�����t���[��
	MotionType mNowMotionType, //���t���[���̃��[�V����
			   mPrevMotionType; //�O�t���[���̃��[�V����
	bool mMotionEnded; //�Đ����̃��[�V�������I��������
	float mMoveSpeedX, //�ړ����x(X���W)
		  mMoveSpeedY; //�ړ����x(Y���W)
	bool mMoveBack;	//����ւ̈ړ���
	bool mJumpedTwice; //2�i�W�����v������
	bool mAerialAttacked; //�󒆂ōU��������
	bool mUpperAttacked; //��U����������

	// �����蔻��̍X�V
	void CollisionUpdate();
	// �Փ˂ւ̔���
	void CollideReaction();

	// ���[�V�����^�C�v����Ή����郂�[�V�����C���f�b�N�X���擾
	const MotionIndex GetMotionIndex(const MotionType mType) const;
	// ��ԃ��[�V�����̐ݒ�
	void SetInterMotion(MotionType from, MotionType to, unsigned frame, MotionType interType);
	// ���[�V�����̍Đ�
	void PlayMotion(CWE::Motion& motion);
	// ��Ԃ̍Đ�
	void PlayInter(CWE::Motion& from, CWE::Motion& to);

	// ���͂ɑ΂��郂�[�V�����̕ύX
	void InputReaction(RequestMotion& request);
	// �󓮓I�ɋN���郂�[�V�����̕ύX
	void PassiveReaction(PassiveRequest& request);
	// ���[�V�����̍X�V
	void MotionUpdate(bool& motionMoveX, bool& motionMoveY, float& friction);
	// ���[�V�����̈ڍs
	void MotionChange(RequestMotion requested, PassiveRequest psReqested);
	// �n��ł̃��[�V�����̈ڍs�v������
	void GroundMotionRespond(RequestMotion requested, PassiveRequest psReqested);
	// �󒆂ł̃��[�V�����̈ڍs�v������
	void AerialMotionRespond(RequestMotion requested);
	// result: (0: �ړ��Ȃ�, 1: ���͂���ړ�, 2: ���͂Ȃ��ړ�)
	void MoveX(bool motionMove, float friction);
	void MoveY();

public:
	Gumi(bool left, Controller* controller, unsigned colorNumber);
	~Gumi();

	bool LoadSuccess() const override;
	bool IsHighImpact() const override;
	void Update() override;

};