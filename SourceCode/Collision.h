#pragma once

#include "../CWE/CWEMath.h"



// �_
typedef CWE::math::Float3 Point;
typedef CWE::math::Float3A PointA;

// ����
struct Segment
{
	Point mP1,
		  mP2;
};

// �J�v�Z��
struct Capsule
{
	Segment mSegment;
	float mRadius;
	bool mAggression;
};



// �_�Ɛ��̍ŒZ�������W
CWE::math::Vector MinDistancePos(const CWE::math::Vector p, const CWE::math::Vector lineP1, const CWE::math::Vector lineP2);


// �_�Ɛ����̍ŒZ������2��(�ŒZ�������W)
CWE::math::Vector Segment_PointDistanceSqFromVector(const CWE::math::Vector segP1, const CWE::math::Vector segP2, const CWE::math::Vector p, CWE::math::Vector& minPos);
// �_�Ɛ����̍ŒZ������2��
inline CWE::math::Vector Segment_PointDistanceSqFromVector(const CWE::math::Vector segP1, const CWE::math::Vector segP2, const CWE::math::Vector p)
{
	using namespace CWE::math;

	Vector minPos;
	return Segment_PointDistanceSqFromVector(segP1, segP2, p, minPos);
}


// 2�����Ԃ̍ŒZ������2��(�ŒZ�������W)
float DistanceSq(const Segment& seg1, const Segment& seg2, Point& seg1Min, Point& seg2Min);
// 2�����Ԃ̍ŒZ������2��
inline float DistanceSq(const Segment& seg1, const Segment& seg2)
{
	using namespace CWE::math;

	PointA seg1Min{},
		   seg2Min{};
	return DistanceSq(seg1, seg2, seg1Min, seg2Min);
}


// �J�v�Z���ƃJ�v�Z���̏Փ˔���(�ŒZ�������W)
bool Collide(const Capsule& cap1, const Capsule& cap2, Point& cap1Min, Point& cap2Min);
// �J�v�Z���ƃJ�v�Z���̏Փ˔���
inline bool Collide(const Capsule& cap1, const Capsule& cap2)
{
	PointA cap1Min{},
		   cap2Min{};
	return Collide(cap1, cap2, cap1Min, cap2Min);
}