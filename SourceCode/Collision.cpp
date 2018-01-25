#include "Collision.h"




// �_�Ɛ��̍ŒZ�������W
CWE::math::Vector MinDistancePos(const CWE::math::Vector p, const CWE::math::Vector lineP1, const CWE::math::Vector lineP2)
{
	using namespace CWE::math;

	float length12, t;
	float dot12_1P;
	Vector line12, line1P;
	Vector result;

	line12 =VectorSubtract(lineP2, lineP1);
	line1P =VectorSubtract(p, lineP1);
	dot12_1P =VectorGetX(Vector3Dot(line12, line1P));

	if(dot12_1P <= 0.0f)
	{
		return lineP1;
	}

	length12 =VectorGetX(Vector3Dot(line12, line12));
	if(NearlyEqual(length12, 0.0f))
	{
		return lineP1;
	}

	if(length12 < dot12_1P)
	{
		return lineP2;
	}

	t =dot12_1P /length12;
	result =VectorSet(
		VectorGetX(lineP1) +VectorGetX(line12) *t,
		VectorGetY(lineP1) +VectorGetY(line12) *t,
		VectorGetZ(lineP1) +VectorGetZ(line12) *t
		);

	return result;
}


// �_�Ɛ����̍ŒZ������2��(�ŒZ�������W)
CWE::math::Vector Segment_PointDistanceSqFromVector(const CWE::math::Vector segP1, const CWE::math::Vector segP2, const CWE::math::Vector p, CWE::math::Vector& minPos)
{
	using namespace CWE::math;

	Vector sa;

	//���Ɠ_�̍ŋߓ_���擾����
	minPos =MinDistancePos(p, segP1, segP2);

	//�ŋߓ_�Ɠ_�̋�����Ԃ�
	sa =VectorSubtract(minPos, p);
	return Vector3LengthSq(sa);
}


// 2�����Ԃ̍ŒZ������2��(�ŒZ�������W)
float DistanceSq(const Segment& seg1, const Segment& seg2, Point& seg1Min, Point& seg2Min)
{
	using namespace CWE::math;

	Vector segA1 =LoadVector(seg1.mP1),
		   segA2 =LoadVector(seg1.mP2),
		   segB1 =LoadVector(seg2.mP1),
		   segB2 =LoadVector(seg2.mP2);	   
	Vector da, db, p, tp;
	Vector e1, e2;
	Vector tmpSegMin{};
	float a, b, c, d, e, f, w, s, t =0.0f;
	float tmpA, tmpB, tmpResult;

	//�e�����̕����x�N�g�����Z�o
	da =VectorSubtract(segA2, segA1);
	db =VectorSubtract(segB2, segB1);

	p =VectorSubtract(segA1, segB1);

	a =VectorGetX(Vector3LengthSq(da));
	b =-VectorGetX(Vector3Dot(da, db));
	c =-b;
	d =-VectorGetX(Vector3LengthSq(db));
	e =-VectorGetX(Vector3Dot(da, p));
	f =-VectorGetX(Vector3Dot(db, p));

	//(segA1 -segA2) or (segB1 - segB2) �̑傫��������Ȃ��[���ɋ߂����ǂ���
	tmpA =a < 0.0f ? -a : a;
	tmpB =d < 0.0f ? -d : d;
	if(tmpA < EPSILON)
	{
		if(tmpB < EPSILON)
		{
			StoreVector(&seg1Min, segA1);
			StoreVector(&seg2Min, segB1);
			return VectorGetX(Vector3LengthSq(VectorSubtract(segA1, segB1))); //2�����̎n�_���m�̋���
		}
		else
		{
			tmpResult =VectorGetX(Segment_PointDistanceSqFromVector(segB1, segB2, segA1, tmpSegMin));
			StoreVector(&seg1Min, segA1);
			StoreVector(&seg2Min, tmpSegMin);
			return tmpResult;
		}
	}
	else if(tmpB < EPSILON)
	{
		tmpResult =VectorGetX(Segment_PointDistanceSqFromVector(segA1, segA2, segB1, tmpSegMin));
		StoreVector(&seg1Min, tmpSegMin);
		StoreVector(&seg2Min, segB1);
		return tmpResult;
	}

	s =-0.1f;
	w =a *d -b *c;
	if(w <= -EPSILON)
	{
		//�N���[�����̌���
		s =(e *d -b *f) /w;
		t =(a *f -e *c) /w;
	}

	//2�������k�ނ��Ă�����_�ƌ��Ȃ��ē_���m�̋�����Ԃ�
	if(a <= 0.0f && -d <= 0.0f)
	{
		tp =VectorSubtract(segA1, segB1);
		StoreVector(&seg1Min, segA1);
		StoreVector(&seg2Min, segB1);
		return std::sqrt(VectorGetX(Vector3LengthSq(tp)));
	}

	if(a <= 0.0f)
	{
		s =0.0f;
		if(1.0f < t || t < 0.0f)
		{
			t =1.0f < t ? 1.0f : 0.0f;
		}
	}
	else if(-d <= 0.0f)
	{
		t =0.0f;
		if(1.0f < s || s < 0.0f)
		{
			s =1.0f < s ? 1.0f : 0.0f;
		}
	}
	else
	{
		if(1.0f < s || s < 0.0f)
		{
			s =1.0f < s ? 1.0f : 0.0f;
			t =(-b *s + -f) / -d; //�K�E�X�����@

			if(1.0f < t || t < 0.0f)
			{
				t =1.0f < t ? 1.0f : 0.0f;
				s =(-b *t - -e) /a; //�K�E�X�����@
				if(1.0f < s) {s =1.0f;}
				else if(s < 0.0f) {s =0.0f;}
			}
		}
		else if(1.0f < t || t < 0.0f)
		{
			t =1.0f < t ? 1.0f : 0.0f;
			s =(-b *t - -e) /a; //�K�E�X�����@

			if(1.0f < s || s < 0.0f)
			{
				s =1.0f < s ? 1.0f : 0.0f;
				t =(-b *s + -f) / -d; //�K�E�X�����@
				if(1.0f < t) {t =1.0f;}
				else if(t < 0.0f) {t =0.0f;}
			}
		}
	}

	e1 =VectorScale(da, s);
	e1 =VectorAdd(e1, segA1);

	e2 =VectorScale(db, t);
	e2 =VectorAdd(e2, segB1);

	tp =VectorSubtract(e1, e2);
	StoreVector(&seg1Min, e1);
	StoreVector(&seg2Min, e2);
	return VectorGetX(Vector3LengthSq(tp));
}


// �J�v�Z���ƃJ�v�Z���̏Փ˔���
bool Collide(const Capsule& cap1, const Capsule& cap2, Point& cap1Min, Point& cap2Min)
{
	float dist =DistanceSq(cap1.mSegment, cap2.mSegment, cap1Min, cap2Min);
	return dist < (cap1.mRadius+cap2.mRadius)*(cap1.mRadius+cap2.mRadius);
}