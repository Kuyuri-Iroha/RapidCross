#pragma once

#include "CWEIncludedAPI.h"
#include "CWEError.h"


namespace CWE
{
// �O���錾
struct Eye;

namespace math
{

// ��
#define CWE_PI 3.14159265358979323846
#define CWE_PI_F XM_PI

// �덷�l
constexpr static const float EPSILON =0.00000001f;


struct Int4;
struct Int4A;
struct Float2;
struct Float2A;
struct Float3;
struct Float3A;
struct Float4;
struct Float4A;
struct Float3x3A;
struct Float4x3;
struct Float4x3A;
struct Float4x4;
struct Float4x4A;
struct Quaternion;
struct QuaternionA;
typedef XMVECTOR Vector;
typedef XMMATRIX Matrix;


// ================ ����int�^ ==========================

struct Int4
{
private:
	int mI4[4];

public:
	Int4() {mI4[0] =0; mI4[1] =0; mI4[2] =0; mI4[3] =0;}
	Int4(const Int4& int4) {mI4[0]=int4.mI4[0]; mI4[1]=int4.mI4[1]; mI4[2]=int4.mI4[2]; mI4[3]=int4.mI4[3];}
	Int4(int x, int y, int z, int w) {mI4[0] =x; mI4[1] =y; mI4[2] =z; mI4[3] =w;}

	int x()const {return mI4[0];}
	int y()const {return mI4[1];}
	int z()const {return mI4[2];}
	int w()const {return mI4[3];}
	int& x() {return mI4[0];}
	int& y() {return mI4[1];}
	int& z() {return mI4[2];}
	int& w() {return mI4[3];}
};

__declspec (align(16))
struct Int4A : public Int4
{
public:
	Int4A(): Int4() {}
	Int4A(const Int4A& int4): Int4(int4) {}
	Int4A(int x, int y, int z, int w): Int4(x, y, z, w) {}

	int x()const {return Int4::x();}
	int y()const {return Int4::y();}
	int z()const {return Int4::z();}
	int w()const {return Int4::w();}
	int& x() {return Int4::x();}
	int& y() {return Int4::y();}
	int& z() {return Int4::z();}
	int& w() {return Int4::w();}
};


// ================ ����float�^ ==========================

struct Float2
{
	friend Vector LoadVector(const math::Float2& float2);
	friend void StoreVector(Float2* float2, const Vector& v);

private:
	XMFLOAT2 mF2;

	const XMFLOAT2& Get()const {return mF2;}
	XMFLOAT2& Get() {return mF2;}

public:
	Float2(): mF2(0.0f, 0.0f) {}
	Float2(const Float2& float2): mF2(float2.mF2) {}
	Float2(float x, float y): mF2(x, y) {}
	Float2& operator= (const Float2& float2)
	{
		mF2 =float2.mF2;
		return *this;
	}
	bool operator== (const Float2& float2)
	{
		return (mF2.x==float2.mF2.x) && (mF2.y==float2.mF2.y);
	}
	bool operator!= (const Float2& float2)
	{
		return !(*this==float2);
	}

	float x()const {return mF2.x;}
	float& x() {return mF2.x;}
	float y()const {return mF2.y;}
	float& y() {return mF2.y;}
};

__declspec (align(16))
struct Float2A : public Float2
{
private:

public:
	Float2A(): Float2() {}
	Float2A(const Float2A& float2A): Float2(float2A) {}
	Float2A(const Float2& float2): Float2(float2) {}
	Float2A(float x, float y): Float2(x, y) {}
};

struct Float3
{
	friend Vector LoadVector(const math::Float3& float3);
	friend void StoreVector(Float3* float3, const Vector& v);

private:
	XMFLOAT3 mF3;

public:
	Float3(): mF3(0.0f, 0.0f, 0.0f) {}
	Float3(const Float3& float3): mF3(float3.mF3) {}
	Float3(float x, float y, float z): mF3(x, y, z) {}
	Float3& operator= (const Float3& float3)
	{
		mF3 =float3.mF3;
		return *this;
	}
	bool operator== (const Float3& float3)
	{
		return (mF3.x==float3.mF3.x) && (mF3.y==float3.mF3.y) && (mF3.z==float3.mF3.z);
	}
	bool operator!= (const Float3& float3)
	{
		return !(*this==float3);
	}

	float x()const {return mF3.x;}
	float& x() {return mF3.x;}
	float y()const {return mF3.y;}
	float& y() {return mF3.y;}
	float z()const {return mF3.z;}
	float& z() {return mF3.z;}
};

__declspec (align(16))
struct Float3A : public Float3
{
private:

public:
	Float3A(): Float3() {}
	Float3A(const Float3A& float3A): Float3(float3A) {}
	Float3A(const Float3& float3): Float3(float3) {}
	Float3A(float x, float y, float z): Float3(x, y, z) {}

	float x()const {return Float3::x();}
	float& x() {return Float3::x();}
	float y()const {return Float3::y();}
	float& y() {return Float3::y();}
	float z()const {return Float3::z();}
	float& z() {return Float3::z();}
};

struct Float4
{
	friend Vector LoadVector(const math::Float4& float4);
	friend void StoreVector(Float4* float4, const Vector& v);

private:
	XMFLOAT4 mF4;

public:
	Float4(): mF4(0.0f, 0.0f, 0.0f, 0.0f) {}
	Float4(const Float4& float4): mF4(float4.mF4) {}
	Float4(float x, float y, float z, float w): mF4(x, y, z, w) {}
	Float4& operator= (const Float4& float4)
	{
		mF4 =float4.mF4;
		return *this;
	}
	bool operator== (const Float4& float4)
	{
		return (mF4.x==float4.mF4.x) && (mF4.y==float4.mF4.y) && (mF4.z==float4.mF4.z) && (mF4.w==float4.mF4.w);
	}
	bool operator!= (const Float4& float4)
	{
		return !(*this==float4);
	}

	float x()const {return mF4.x;}
	float& x() {return mF4.x;}
	float y()const {return mF4.y;}
	float& y() {return mF4.y;}
	float z()const {return mF4.z;}
	float& z() {return mF4.z;}
	float w()const {return mF4.w;}
	float& w() {return mF4.w;}
};

__declspec(align(16))
struct Float4A : public Float4
{
private:

public:
	Float4A(): Float4() {}
	Float4A(const Float4A& float4A): Float4(float4A) {}
	Float4A(const Float4& float4): Float4(float4) {}
	Float4A(float x, float y, float z, float w): Float4(x, y, z, w) {}

	float x()const {return Float4::x();}
	float& x() {return Float4::x();}
	float y()const {return Float4::y();}
	float& y() {return Float4::y();}
	float z()const {return Float4::z();}
	float& z() {return Float4::z();}
	float w()const {return Float4::w();}
	float& w() {return Float4::w();}
};

struct Quaternion
{
	friend Vector LoadQuaternion(const math::Quaternion& quaternion);
	friend void StoreQuaternion(Quaternion* quaternion, const Vector& v);

private:
	Float4 mF4;

public:
	Quaternion(): mF4(0.0f, 0.0f, 0.0f, 1.0f) {}
	Quaternion(const Quaternion& quaternion): mF4(quaternion.mF4) {}
	Quaternion& operator=(const Quaternion& quaternion)
	{
		mF4 =quaternion.mF4;
		return *this;
	}
	~Quaternion() {}
};

_declspec(align(16))
struct QuaternionA : public Quaternion
{
private:

public:
	QuaternionA(): Quaternion() {}
	QuaternionA(const QuaternionA& quaternion): Quaternion(quaternion) {}
	QuaternionA(const Quaternion& quaternion): Quaternion(quaternion) {}
	~QuaternionA() {}
};

struct Float3x3
{
	friend Matrix LoadMatrix(const Float3x3& float3x3);
	friend void StoreMatrix(Float3x3* float3x3, const Matrix& m);

private:
	union
	{
		XMFLOAT3X3 mF3x3;
		struct{Float3 mRow[3];};
	};

public:
	Float3x3(): mF3x3(1.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 1.0f) {}
	Float3x3(const Float3x3& float3x3): mF3x3(float3x3.mF3x3) {}
	Float3x3(float m00, float m01, float m02, 
			 float m10, float m11, float m12,
			 float m20, float m21, float m22): mF3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22) {}
	Float3x3& operator= (const Float3x3& float3x3)
	{
		mF3x3 =float3x3.mF3x3;
		return *this;
	}
	float operator() (unsigned row, unsigned column)const {return mF3x3(row, column);} 
	float& operator() (unsigned row, unsigned column) {return mF3x3(row, column);}

	const Float3& operator()(unsigned row)const
	{
		CWE_ASSERT(row < 3);
		return mRow[row];
	}
	Float3& operator()(unsigned row)
	{
		CWE_ASSERT(row < 3);
		return mRow[row];
	}
};

__declspec(align(16))
struct Float3x3A : public Float3x3
{
private:

public:
	Float3x3A(): Float3x3() {}
	Float3x3A(const Float3x3A& float3x3): Float3x3(float3x3) {}
	Float3x3A(const Float3x3& float3x3): Float3x3(float3x3) {}
	Float3x3A(float m00, float m01, float m02, 
			  float m10, float m11, float m12,
			  float m20, float m21, float m22): Float3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22) {}
};

struct Float4x3
{
	friend Matrix LoadMatrix(const Float4x3& float4x3);
	friend void StoreMatrix(Float4x3* float4x3, const Matrix& m);

private:
	union
	{
		XMFLOAT4X3 mF4x3;
		struct{Float3 mRow[4];};
	};

public:
	Float4x3(): mF4x3(1.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 1.0f,
					   0.0f, 0.0f, 0.0f) {}
	Float4x3(const Float4x3& float4x3): mF4x3(float4x3.mF4x3) {}
	Float4x3(float m00, float m01, float m02, 
			 float m10, float m11, float m12,
			 float m20, float m21, float m22,
			 float m30, float m31, float m32): mF4x3(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
	Float4x3& operator= (const Float4x3& float4x3)
	{
		mF4x3 =float4x3.mF4x3;
		return *this;
	}
	float operator() (unsigned row, unsigned column)const {return mF4x3(row, column);} 
	float& operator() (unsigned row, unsigned column) {return mF4x3(row, column);}

	const Float3& operator()(unsigned row)const
	{
		CWE_ASSERT(row < 4);
		return mRow[row];
	}
	Float3& operator()(unsigned row)
	{
		CWE_ASSERT(row < 4);
		return mRow[row];
	}
};

__declspec(align(16))
struct Float4x3A : public Float4x3
{
private:

public:
	Float4x3A(): Float4x3() {}
	Float4x3A(const Float4x3A& float4x3): Float4x3(float4x3) {}
	Float4x3A(const Float4x3& float3x3): Float4x3(float3x3) {}
	Float4x3A(float m00, float m01, float m02, 
			  float m10, float m11, float m12,
			  float m20, float m21, float m22,
			  float m30, float m31, float m32): Float4x3(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
};

struct Float4x4
{
	friend Matrix LoadMatrix(const Float4x4& float4x4);
	friend void StoreMatrix(Float4x4* float4x4, const Matrix& m);

private:
	union
	{
		XMFLOAT4X4 mF4x4;
		struct{Float4 mRow[4];};
	};

public:
	Float4x4(): mF4x4(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f) {}
	Float4x4(const Float4x4& float4x4): mF4x4(float4x4.mF4x4) {}
	Float4x4(float m00, float m01, float m02, float m03, 
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33): mF4x4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
	Float4x4& operator= (const Float4x4& float4x4)
	{
		mF4x4 =float4x4.mF4x4;
		return *this;
	}
	float operator() (unsigned row, unsigned column)const {return mF4x4(row, column);} 
	float& operator() (unsigned row, unsigned column) {return mF4x4(row, column);}
	bool operator== (const Float4x4& float4x4);
	bool operator!= (const Float4x4& float4x4);

	const Float4& operator()(unsigned row)const
	{
		CWE_ASSERT(row < 4);
		return mRow[row];
	}
	Float4& operator()(unsigned row)
	{
		CWE_ASSERT(row < 4);
		return mRow[row];
	}
};

__declspec(align(16))
struct Float4x4A : public Float4x4
{
private:

public:
	Float4x4A(): Float4x4() {}
	Float4x4A(const Float4x4A& float4x4): Float4x4(float4x4) {}
	Float4x4A(const Float4x4& float4x4): Float4x4(float4x4) {}
	Float4x4A(float m00, float m01, float m02, float m03, 
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33): Float4x4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
};





//**********************************************************************************
// ================= Vector & Matrix �ŎZ�p�֐� ====================================
//**********************************************************************************


// ============== �R���o�[�g�n�֐� ======================

inline Vector LoadVector(const Float2& float2) {return XMLoadFloat2(&float2.mF2);}
inline Vector LoadVector(const Float3& float3) {return XMLoadFloat3(&float3.mF3);}
inline Vector LoadVector(const Float4& float4) {return XMLoadFloat4(&float4.mF4);}
inline void StoreVector(Float2* float2, const Vector& v)
{
	CWE_ASSERT(float2);
	XMStoreFloat2(&float2->mF2, v);
}
inline void StoreVector(Float3* float3, const Vector& v)
{
	CWE_ASSERT(float3);
	XMStoreFloat3(&float3->mF3, v);
}
inline void StoreVector(Float4* float4, const Vector& v)
{
	CWE_ASSERT(float4);
	XMStoreFloat4(&float4->mF4, v);
}
inline Vector LoadQuaternion(const Quaternion& quaternion) {return LoadVector(quaternion.mF4);}
inline void StoreQuaternion(Quaternion* quaternion, const Vector& v)
{
	CWE_ASSERT(quaternion);
	StoreVector(&quaternion->mF4, v);
}
inline Matrix LoadMatrix(const Float3x3& float3x3) {return XMLoadFloat3x3(&float3x3.mF3x3);}
inline Matrix LoadMatrix(const Float4x3& float4x3) {return XMLoadFloat4x3(&float4x3.mF4x3);}
inline Matrix LoadMatrix(const Float4x4& float4x4) {return XMLoadFloat4x4(&float4x4.mF4x4);}
inline void StoreMatrix(Float3x3* float3x3, const Matrix& m)
{
	CWE_ASSERT(float3x3);
	XMStoreFloat3x3(&float3x3->mF3x3, m);
}
inline void StoreMatrix(Float4x3* float4x3, const Matrix& m)
{
	CWE_ASSERT(float4x3);
	XMStoreFloat4x3(&float4x3->mF4x3, m);
}
inline void StoreMatrix(Float4x4* float4x4, const Matrix& m)
{
	CWE_ASSERT(float4x4);
	XMStoreFloat4x4(&float4x4->mF4x4, m);
}


// ============== �x�N�g���n�֐� ========================

// �Z�b�g
inline Vector VectorSet(float x, float y, float z)
{
	return XMVectorSet(x, y, z, 0.0f);
}
inline Vector VectorSet(float x, float y, float z, float w)
{
	return XMVectorSet(x, y, z, w);
}
// �e�v�f�̒��o
inline float VectorGetX(const Vector v)
{
	return XMVectorGetX(v);
}
inline float VectorGetY(const Vector v)
{
	return XMVectorGetY(v);
}
inline float VectorGetZ(const Vector v)
{
	return XMVectorGetZ(v);
}
inline float VectorGetW(const Vector v)
{
	return XMVectorGetW(v);
}
// �덷���e��������
inline bool Vector3NearlyEqual(const Vector v1, const Vector v2, const Vector vEpsilon)
{
	return XMVector3NearEqual(v1, v2, vEpsilon) != 0;
}
// ���Z
inline Vector VectorAdd(const Vector v1, const Vector v2)
{
	return XMVectorAdd(v1, v2);
}
// ���Z
inline Vector VectorSubtract(const Vector v1, const Vector v2)
{
	return XMVectorSubtract(v1, v2);
}
// �X�J���[�{
Vector VectorScale(const Vector v, float scale);
// ����
inline Vector Vector3Dot(const Vector v1, const Vector v2)
{
	return XMVector3Dot(v1, v2);
}
// �O��
inline Vector Vector3Cross(const Vector v1, const Vector v2)
{
	return XMVector3Cross(v1, v2);
}
// ���K��
inline Vector Vector3Normalize(const Vector v)
{
	return XMVector3Normalize(v);
}
// ����
inline Vector Vector3Length(const Vector v)
{
	return XMVector3Length(v);
}
// ������2��
inline Vector Vector3LengthSq(const Vector v)
{
	return XMVector3LengthSq(v);
}
// 2�̃x�N�g���Ԃ̃��W�A���p(�Ȃ��p)
inline Vector Vector3AngleBetween(const Vector v1, const Vector v2)
{
	return XMVector3AngleBetweenVectors(v1, v2);
}
// 2�̐��K�����ꂽ�x�N�g���Ԃ̃��W�A���p(�Ȃ��p)
inline Vector Vector3AngleBetweenNormal(const Vector normalizedV1, const Vector normalizedV2)
{
	return XMVector3AngleBetweenNormals(normalizedV1, normalizedV2);
}
// �x�N�g�����s��ŕϊ�
inline Vector Vector3Transform(const Vector v, const Matrix& mTrans)
{
	return XMVector3Transform(v, mTrans);
}
// ���K�����ꂽ�x�N�g�����s��ŕϊ�
inline Vector Vector3TransformNormal(const Vector v, const Matrix& mTrans)
{
	return XMVector3TransformNormal(v, mTrans);
}
// �x�N�g������]�N�H�[�^�j�I���ŉ�]
inline Vector Vector3Rotate(const Vector v, const Vector qRotation)
{
	return XMVector3Rotate(v, qRotation);
}
// �x�N�g������]�N�H�[�^�j�I���ŉ�]
inline Vector Vector3InverseRotate(const Vector v, const Vector qRotation)
{
	return XMVector3InverseRotate(v, qRotation);
}
// �@���x�N�g���ɂ���ē��˃x�N�g���𔽎�
inline Vector Vector3Reflect(const Vector& incident, const Vector& normal)
{
	return XMVector3Reflect(incident, normal);
}


// ============== �N�H�[�^�j�I���n�֐� ==================

// ���K��
inline Vector QuaternionNormalize(const Vector quaterinon)
{
	return XMQuaternionNormalize(quaterinon);
}
// �N�H�[�^�j�I���̋t�s��
inline Vector QuaternionInverse(const Vector quaterinon)
{
	return XMQuaternionInverse(quaterinon);
}
// �C�ӎ���]�N�H�[�^�j�I�����쐬
inline Vector QuaternionRotationAxis(const Vector axis, float angle)
{
	return XMQuaternionRotationAxis(axis, angle);
}
// �I�C���[�p�����]�N�H�[�^�j�I�����쐬
inline Vector QuaternionRotationRollPitchYaw(float pitch, float yaw, float roll)
{
	return XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}
// �N�H�[�^�j�I���̏�Z
inline Vector QuaternionMultiply(const Vector q1, const Vector q2)
{
	return XMQuaternionMultiply(q1, q2);
}


// ============== �s��n�֐� ============================

// �P�ʍs��
inline Matrix MatrixIdentity()
{
	return XMMatrixIdentity();
}
// �t�s��
inline Matrix MatrixInverse(const Matrix& m)
{
	return XMMatrixInverse(&Vector(), m);
}
// �]�u�s��̍쐬
inline Matrix MatrixTranspose(const Matrix& m)
{
	return XMMatrixTranspose(m);
}
// �s��̏�Z
inline Matrix MatrixMultiply(const Matrix& m1, const Matrix& m2)
{
	return XMMatrixMultiply(m1, m2);
}
// �I�C���[�p�����]�s����쐬
inline Matrix MatrixRotationRollPitchYaw(float pitch, float yaw, float roll)
{
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}
// �N�H�[�^�j�I�������]�s����쐬
inline Matrix MatrixRotationQuaternion(const Vector qRotation)
{
	return XMMatrixRotationQuaternion(qRotation);
}
// ���ˉe�ϊ��s��쐬
inline Matrix OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	return XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}
// �r���[�ϊ��s��̍쐬
inline Matrix LookAtLH(const Vector eyePosition, const Vector focusPosition, const Vector upDirection)
{
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}


// ================= ���̑��̎Z�p�֐� ===================

// ��]�N�H�[�^�j�I������I�C���[�p���Z�o
void RollPitchYawFromQuaternion(float& pitch, float& yaw, float& roll, Vector qRotation);
// ��]�s�񂩂�I�C���[�p���Z�o
void RollPitchYawFromMatrix(float& pitch, float& yaw, float& roll, const Matrix& mRotation);





//**********************************************************************************
// ================= �Z�p�֐� ======================================================
//**********************************************************************************


// ============== �R���o�[�g�n�֐� ======================

// �x���@���ʓx�@
inline const float ConvertToRadians(float degrees) {return degrees *(CWE_PI_F / 180.0f);}
// �ʓx�@���x���@
inline const float ConvertToDegrees(float radians) {return radians *(180.0f / CWE_PI_F);}


// ============== �X�J���[�n�֐� ========================


// �덷���e��������
inline bool NearlyEqual(float a, float b, float epsilon=EPSILON)
{
	return (fabsf(a-b) <= epsilon);
}
// �R�T�C��
inline float Cos(float rad)
{
	return cosf(rad);
}
// �A�[�N�R�T�C��
inline float ACos(float value)
{
	return acosf(value);
}
// �T�C��
inline float Sin(float rad)
{
	return sinf(rad);
}
// �A�[�N�T�C��
inline float ASin(float value)
{
	return asinf(value);
}
// �^���W�F���g
inline float Tan(float rad)
{
	return tanf(rad);
}
// �A�[�N�^���W�F���g
inline float ATan(float x, float y)
{
	return atan2f(x, y);
}
// -CWE_PI_F�ȏ�CWE_PI_F�����Ɋp�x���ۂ߂�
inline float ModAngle(float rad)
{
	return XMScalarModAngle(rad);
}
// �p�x��-CWE_PI_F�ȏ�CWE_PI_F����������
inline bool IsRegularAngle(float rad)
{
	return (-CWE_PI_F <= rad && rad < CWE_PI_F);
}




// ============== �x�N�g���n�֐� ========================

// ���Z
inline void VectorAdd(Float2* result, const Float2& v1, const Float2& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()+v2.x();
	result->y() =v1.y()+v2.y();
}
inline void VectorAdd(Float3* result, const Float3& v1, const Float3& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()+v2.x();
	result->y() =v1.y()+v2.y();
	result->z() =v1.z()+v2.z();
}
inline void VectorAdd(Float4* result, const Float4& v1, const Float4& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()+v2.x();
	result->y() =v1.y()+v2.y();
	result->z() =v1.z()+v2.z();
	result->w() =v1.w()+v2.w();
}
// ���Z
inline void VectorSubtract(Float2* result, const Float2& v1, const Float2& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()-v2.x();
	result->y() =v1.y()-v2.y();
}
inline void VectorSubtract(Float3* result, const Float3& v1, const Float3& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()-v2.x();
	result->y() =v1.y()-v2.y();
	result->z() =v1.z()-v2.z();
}
inline void VectorSubtract(Float4* result, const Float4& v1, const Float4& v2)
{
	CWE_ASSERT(result);
	result->x() =v1.x()-v2.x();
	result->y() =v1.y()-v2.y();
	result->z() =v1.z()-v2.z();
	result->w() =v1.w()-v2.w();
}
// �X�J���[�{
void VectorScale(Float3* result, const Float3& v, float scale);
// �S�l���Z�b�g
inline void VectorFill(Float2& result, float value)
{
	result.x() =value;
	result.y() =value;
}
inline void VectorFill(Float3& result, float value)
{
	result.x() =value;
	result.y() =value;
	result.z() =value;
}
inline void VectorFill(Float4& result, float value)
{
	result.x() =value;
	result.y() =value;
	result.z() =value;
	result.w() =value;
}
// �[���x�N�g��������
inline bool VectorIsZero(const Float2& v)
{
	return (v.x() == 0.0f && v.y() == 0.0f);
}
inline bool VectorIsZero(const Float3& v)
{
	return (v.x() == 0.0f && v.y() == 0.0f && v.z() == 0.0f);
}
inline bool VectorIsZero(const Float4& v)
{
	return (v.x() == 0.0f && v.y() == 0.0f && v.z() == 0.0f && v.w() == 0.0f);
}
// 2�_�Ԃ̋���
float VectorDistance(const Float3& v1, const Float3& v2);
// 2�_�Ԃ̋�����2��
float VectorDistanceSq(const Float3& v1, const Float3& v2);
// 2�̐��K���ς݃x�N�g���Ԃ̃��W�A���p(�Ȃ��p)
float VectorAngleBetweenNormals(const Float3& v1, const Float3& v2);
// �O��
void VectorCross(Float3* result, const Float3& v1, const Float3& v2);
// ���K��
void VectorNormalize(Float3* result, const Float3& source);
// ���K������Ă��邩�𔻒�
template<typename V>
inline bool VectorIsNormalized(const V& v)
{
	return (VectorDistanceSq(v, V()) == 1.0f);
}
// �x�N�g������]�N�H�[�^�j�I���ŉ�]
inline void Vector3Rotate(Float3* result, const Float3& v, const Quaternion& qRotation)
{
	CWE_ASSERT(result);
	return StoreVector(result, XMVector3Rotate(LoadVector(v), LoadQuaternion(qRotation)));
}
// �x�N�g�����`���
void VectorLerp(Float3* result, const Float3& v1, const Float3& v2, float tx, float ty, float tz);
// �َ�x�N�g���ϊ�
inline void VectorConvert(Float4* result, const Float3& source)
{
	CWE_ASSERT(result);
	result->x() =source.x();
	result->y() =source.y();
	result->z() =source.z();
}
inline void VectorConvert(Float3* result, const Float4& source)
{
	CWE_ASSERT(result);
	result->x() =source.x();
	result->y() =source.y();
	result->z() =source.z();
}


// ============== �N�H�[�^�j�I���n�֐� ==================

// �C�ӎ���]�N�H�[�^�j�I�����쐬
inline void QuaternionRotationAxis(Quaternion* pQuaternion, const Float3& axis, float angle)
{
	StoreQuaternion(pQuaternion, XMQuaternionRotationAxis(LoadVector(axis), angle));
}
// �I�C���[�p��]�N�H�[�^�j�I�����쐬
inline void QuaternionRotationRollPitchYaw(Quaternion* pQuaternion, float pitch, float yaw, float roll)
{
	StoreQuaternion(pQuaternion, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}
// ���K�����ꂽ�x�N�g����Y����](�N�H�[�^�j�I��)
void QuaternionRotationY(Float3* pNormalizedPos, float angle);
// ���K�����ꂽ�x�N�g����X����](�N�H�[�^�j�I��)
void QuaternionRotationX(Float3* pNormalizedPos, float angle);
// ���K��
inline void QuaternionNormalize(Quaternion* result, const Quaternion& source)
{
	StoreQuaternion(result, XMQuaternionNormalize(LoadQuaternion(source)));
}
// �P�ʃN�H�[�^�j�I�����쐬
inline void QuaternionIdentity(Quaternion& quaternion)
{
	StoreQuaternion(&quaternion, XMQuaternionIdentity());
}
// �P�ʃN�H�[�^�j�I�����𔻒�
inline bool QuaternionIsIdentity(const Quaternion& quaternion)
{
	return XMQuaternionIsIdentity(LoadQuaternion(quaternion)) != 0;
}
// ������`���
inline void QuaternionSlerp(Quaternion* result, const Quaternion& q0, const Quaternion& q1, float t)
{
	StoreQuaternion(result, XMQuaternionSlerp(LoadQuaternion(q0), LoadQuaternion(q1), t));
}



// ============== �s��n�֐� ============================

// �S�l���Z�b�g
inline void MatrixFill(Float3x3& result, float value)
{
	VectorFill(result(0), value);
	VectorFill(result(1), value);
	VectorFill(result(2), value);
}
inline void MatrixFill(Float4x3& result, float value)
{
	VectorFill(result(0), value);
	VectorFill(result(1), value);
	VectorFill(result(2), value);
	VectorFill(result(3), value);
}
inline void MatrixFill(Float4x4& result, float value)
{
	VectorFill(result(0), value);
	VectorFill(result(1), value);
	VectorFill(result(2), value);
	VectorFill(result(3), value);
}
// ���ˉe�ϊ��s��쐬
void OrthographicLH(Float4x4* projection, float viewWidth, float viewHeight, float nearZ, float farZ);
// ���ˉe�ϊ��s��̓]�u�s��̍쐬
void OrthographicLHTp(Float4x4* projection, float viewWidth, float viewHeight, float nearZ, float farZ);
// �p�[�X�y�N�e�B�u�ˉe�ϊ��s��쐬
void PerspectiveFovLHTp(Float4x4* projection, float fovAngleY, float aspectRatio, float nearZ, float farZ);
// �r���[�ϊ��s��̍쐬
void LookAtLH(Float4x4* view, const Float3& eyePosition, const Float3& focusPosition, const Float3& upDirection);
// �r���[�ϊ��s��̓]�u�s��̍쐬
void LookAtLHTp(Float4x4* view, const Float3& eyePosition, const Float3& focusPosition, const Float3& upDirection);
//�]�u���[���h�ϊ��s��쐬
void WorldTp(Float4x4* world, const Float3& offset, const Float3& scale);
void WorldTp(Float4x4* world, const Float3& offset, const Quaternion& rotation);
void WorldTp(Float4x4* world, const Float3& offset, const Quaternion& rotation, float scale);
void WorldTp(Float4x4* world, const Float3& offset, float scale);
void WorldTp(Float4x4* world, const Float3& offset);
//�]�u2D���[���h�ϊ��s��쐬
void World2DTp(Float4x4* world, const Float3& offset, const Float2& scale);
void World2DTp(Float4x4* world, const Float3& offset, float scale);
void World2DTp(Float4x4* world, const Float3& offset);
// �p�[�e�B�N���r���{�[�h�p�̓]�u���[���h�ϊ��s��쐬
void WorldBillboardTp(Float4x4* world, const Float3& offset, const Float2& scale, const Eye& eye);
void WorldBillboardTp(Float4x4* world, const Float3& offset, float scale, const Eye& eye);
void WorldBillboardTp(Float4x4* world, const Float3& offset, const Eye& eye);
// �s��̏�Z
template<typename MResult, typename M1, typename M2>
void MatrixMultiply(MResult* result, const M1& m1, const M2& m2)
{
	StoreMatrix(result, XMMatrixMultiply(LoadMatrix(m1), LoadMatrix(m2)));
}
// �s��̏�Z�̓]�u
template<typename MResult, typename M1, typename M2>
void MatrixMultiplyTp(MResult* result, const M1& m1, const M2& m2)
{
	StoreMatrix(result, XMMatrixMultiplyTranspose(LoadMatrix(m1), LoadMatrix(m2)));
}
// ���s�ړ��s��̍쐬
template<typename M>
void MatrixTranslation(M* result, const Float3& offset)
{
	StoreMatrix(result, XMMatrixTranslation(offset.x(), offset.y(), offset.z()));
}
// ���s�ړ�*��]�s��̍쐬
template<typename M>
void MatrixTranslationR(M* result, const Float3& offset, const Quaternion& rotation)
{
	StoreMatrix(result, XMMatrixRotationQuaternion(LoadQuaternion(rotation)) *XMMatrixTranslation(offset.x(), offset.y(), offset.z()));
}
// �I�C���[�p�ɂ���]�s��̍쐬
template<typename M>
void MatrixRotationRollPitchYaw(M* result, float pitch, float yaw, float roll)
{
	StoreMatrix(result, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
}
// �N�H�[�^�j�I�������]�s����쐬
template<typename M>
void MatrixRotationQuaternion(M* result, const Quaternion& quaternion)
{
	StoreMatrix(result, XMMatrixRotationQuaternion(LoadQuaternion(quaternion)));
}
// �P�ʍs��쐬
template<typename M>
void MatrixIdentity(M& matrix)
{
	StoreMatrix(&matrix, XMMatrixIdentity());
}
// �t�s��쐬
template<typename M>
void MatrixInverse(M* result, const M& source)
{
	StoreMatrix(result, XMMatrixInverse(&Vector(), LoadMatrix(source)));
}
// �]�u�s��쐬
void MatrixTranspose(Float4x4* result, const Float4x4& source);
// �P�ʍs�񂩂ǂ���
template<typename M>
inline bool MatrixIsIdentity(const M& matrix)
{
	return XMMatrixIsIdentity(LoadMatrix(matrix)) != 0;
}
// �َ�s��ϊ�
void MatrixConvert(Float4x4* result, const Float4x3& source);


// ================= ���̑��̎Z�p�֐� =============================

// �x�W�F�Ȑ��֐�
void Bezier(Float2* bezier, const Float2& p0, const Float2& p1, const Float2& p2, const Float2& p3, float t);
float CubicBezier(float x1, float y1, float x2, float y2, float t);
// �x�N�g������x�N�g���։�]����N�H�[�^�j�I�����Z�o
void VectorToVectorRotaion(Quaternion* result, const Float3& fromV, const Float3& toV);
// ���K���x�N�g�����琳�K���x�N�g���։�]����N�H�[�^�j�I�����Z�o
void VectorToVectorRotaionNormal(Quaternion* result, const Float3& fromV, const Float3& toV);
// ��]�N�H�[�^�j�I������I�C���[�p���Z�o
void RollPitchYawFromQuaternion(float& pitch, float& yaw, float& roll, const Quaternion& rotation);





}// math
}// CWE



