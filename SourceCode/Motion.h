#pragma once

#include "../CWE/CWELib.h"



// ���[�V����
class Motion final
{
private:
	CWE::Motion mMotion;
	CWE::math::Float4x4 mOldPos;
	float mNowFrame;

public:
	Motion(): mMotion(), mNowFrame(0.0f) {}
	~Motion() {}

	inline const CWE::Motion& GetMotion() const {return mMotion;}
	inline CWE::Motion& GetMotion() {return mMotion;}
	inline float GetNowFrame() const {return mNowFrame;}

	bool Play(CWE::Polygon& applied, bool order, float speed, CWE::math::Matrix& movement);
	bool Apply(CWE::Polygon& applied, float frame, CWE::math::Matrix& movement);
	void Restart();

};


// ���[�V�������
class MotionInter final
{
private:
	unsigned mNowCount; //���݂̃J�E���g
	unsigned mInterCount; //��ԃJ�E���g

public:
	MotionInter();
	~MotionInter();

	void Reset(unsigned interCount);
	//result: true=��Ԓ�, false=��Ԋ���
	bool Exec(CWE::Polygon& model, CWE::Motion& from, CWE::Motion& to);

};