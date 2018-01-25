#pragma once

#include "../CWE/CWEPolygon.h"
#include "BattleEnums.h"
#include "Controller.h"
#include "Collision.h"
#include "Motion.h"



// �v���C���[���N���X
class PlayerBase
{
private:
	Controller* const mpController;

protected:
	static const wchar_t* const smFailedModelMs;
	static const wchar_t* const	smFailedMotionMs;

	CWE::Polygon mModel;

	MotionInter mInter;
	CWE::math::Float4x4 mLocal;
	bool mIsRight; //�E������

	// �R���g���[���[�̎擾
	const Controller* const GetController() const {return mpController;}

public:
	PlayerBase(bool left, Controller* pController, const wchar_t* const filePath);
	virtual ~PlayerBase();

	CWE::math::Float3 mPos;
	//��_���[�W�R���W����
	Capsule mUpperBody,
			mLowerBody;
	//�^�_���[�W�R���W����
	std::vector<Capsule> mAggressors;
	bool mGreatDamage;
	//HP
	static const int smMaxHP =700;
	int mHP;

	//�O������
	struct Phenomenon
	{
		bool mGreatDamaged;
		bool mDamaged;
		bool mDamagedFromRight;
	};
	Phenomenon mPhnomenon;

	virtual bool LoadSuccess() const =0;
	virtual void CollisionUpdate() =0;
	virtual bool IsHighImpact() const =0;
	virtual void Update() =0;
	void Draw(const CWE::math::Float4x4& stage) const;

};