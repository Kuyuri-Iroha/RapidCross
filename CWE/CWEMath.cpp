#include "CWEMath.h"
#include "CWEViewPort.h"



namespace CWE
{
namespace math
{

// ================= �萔��` ============================

// �W���o�����b�N���pX������
//(�v�f�o�b�O)
static float AvoidGimbalLockByPalamX =ConvertToRadians(85.0f);



// ================= �Z�p�^�֐� ==========================

// Float4x4
bool Float4x4::operator== (const Float4x4& float4x4)
{
	return (mRow[0]==float4x4.mRow[0] &&
		mRow[1]==float4x4.mRow[1] &&
		mRow[2]==float4x4.mRow[2] &&
		mRow[3]==float4x4.mRow[3]);
}
bool Float4x4::operator!= (const Float4x4& float4x4)
{
	return !(*this == float4x4);
}




//**********************************************************************************
// ================= Vector & Matrix �ŎZ�p�֐� ====================================
//**********************************************************************************

// ================= �x�N�g���n�֐� =====================

// �X�J���[�{
Vector VectorScale(const Vector v, float scale)
{
	Vector result =VectorSet(VectorGetX(v) *scale, VectorGetY(v) *scale, VectorGetZ(v) *scale, VectorGetW(v) *scale);
	return result;
}


// ================= ���̑��̎Z�p�֐� ===================

// ��]�N�H�[�^�j�I������I�C���[�p���Z�o
void RollPitchYawFromQuaternion(float& pitch, float& yaw, float& roll, const Vector qRotation)
{
	Matrix mRotation =MatrixRotationQuaternion(qRotation);

	//�s�b�`(X����])
	pitch =ASin(-mRotation(2, 1));
	//�W���o�����b�N���
	if(AvoidGimbalLockByPalamX < fabsf(pitch))
	{
		pitch =(pitch<0)?-AvoidGimbalLockByPalamX : AvoidGimbalLockByPalamX;
	}
	float cosineX =Cos(pitch);
	//���[(Y����])
	yaw =ATan((mRotation(2, 0)/cosineX), (mRotation(2, 2)/cosineX));
	//���[��(Z����])
	roll =ATan((mRotation(0, 1)/cosineX), (mRotation(1, 1)/cosineX));
	//�g����p�x�Ɋۂ߂�
	pitch =ModAngle(pitch);
	yaw =ModAngle(yaw);
	roll =ModAngle(roll);
}


// ��]�s�񂩂�I�C���[�p���Z�o
void RollPitchYawFromMatrix(float& pitch, float& yaw, float& roll, const Matrix& mRotation)
{
	//�s�b�`(X����])
	pitch =ASin(-mRotation(2, 1));
	//�W���o�����b�N���
	if(AvoidGimbalLockByPalamX < fabsf(pitch))
	{
		pitch =(pitch<0)?-AvoidGimbalLockByPalamX : AvoidGimbalLockByPalamX;
	}
	float cosineX =Cos(pitch);
	//���[(Y����])
	yaw =ATan((mRotation(2, 0)/cosineX), (mRotation(2, 2)/cosineX));
	//���[��(Z����])
	roll =ATan((mRotation(0, 1)/cosineX), (mRotation(1, 1)/cosineX));
	//�g����p�x�Ɋۂ߂�
	pitch =ModAngle(pitch);
	yaw =ModAngle(yaw);
	roll =ModAngle(roll);
}



//**********************************************************************************
// ================= �Z�p�֐� ======================================================
//**********************************************************************************

// ============== �x�N�g���n�֐� ========================

// �X�J���[�{
void VectorScale(Float3* result, const Float3& v, float scale)
{
	CWE_ASSERT(result);
	result->x() =v.x() *scale;
	result->y() =v.y() *scale;
	result->z() =v.z() *scale;
}


// 2�_�Ԃ̋���
float VectorDistance(const Float3& v1, const Float3& v2)
{
	return XMVectorGetX(
				XMVector3Length(
					LoadVector(v2)-LoadVector(v1)
					)
			);
}


// 2�_�Ԃ̋�����2��
float VectorDistanceSq(const Float3& v1, const Float3& v2)
{
	return XMVectorGetX(
				XMVector3LengthSq(
					LoadVector(v2)-LoadVector(v1)
					)
			);
}


// 2�̐��K���ς݃x�N�g���Ԃ̃��W�A���p(�Ȃ��p)
float VectorAngleBetweenNormals(const Float3& v1, const Float3& v2)
{
	return XMVectorGetX(
				XMVector3AngleBetweenNormals(
					LoadVector(v1), LoadVector(v2)
					)
			);
}


// �O��
void VectorCross(Float3* result, const Float3& v1, const Float3& v2)
{
	StoreVector(result, XMVector3Cross(LoadVector(v1), LoadVector(v2)));
}


// ���K��
void VectorNormalize(Float3* result, const Float3& source)
{
	StoreVector(result, XMVector3Normalize(LoadVector(source)));
}


// ���K������Ă��邩�𔻒�
//bool IsNormalized(const Float3& v);


// �x�N�g�����`���
void VectorLerp(Float3* result, const Float3& v1, const Float3& v2, float tx, float ty, float tz)
{
	StoreVector(result, 
		XMVectorLerpV(LoadVector(v1), LoadVector(v2), XMVectorSet(tx, ty, tz, 0.0f))
		);
}



// ============== �N�H�[�^�j�I���n�֐� ==================

// ���K�����ꂽ�x�N�g����Y����](�N�H�[�^�j�I��)
void QuaternionRotationY(Float3* pNormalizedPos, float angle)
{
	StoreVector(
		pNormalizedPos,
		XMVector3Rotate(LoadVector(*pNormalizedPos),
			XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), angle)
			)
		);
}

// ���K�����ꂽ�x�N�g����Y����](�N�H�[�^�j�I��)
void QuaternionRotationX(Float3* pNormalizedPos, float angle)
{
	StoreVector(
		pNormalizedPos,
		XMVector3Rotate(LoadVector(*pNormalizedPos),
			XMQuaternionRotationAxis(XMVectorSet(-pNormalizedPos->z(), 0.0f, pNormalizedPos->x(), 0.0f), angle)
			)
		);
}


// �P�ʃN�H�[�^�j�I�����쐬
//void SetIdentity(Float4& quaternion);


// �P�ʃN�H�[�^�j�I�����𔻒�
//bool IsIdentity(const Float4& quaternion)



// ============== �s��n�֐� ============================

// ���ˉe�ϊ��s��쐬
void OrthographicLH(Float4x4* projection, float viewWidth, float viewHeight, float nearZ, float farZ)
{
	StoreMatrix(projection, XMMatrixOrthographicLH(viewHeight, viewWidth, nearZ, farZ));
}

// ���ˉe�ϊ��s��̓]�u�s��̍쐬
void OrthographicLHTp(Float4x4* projection, float viewWidth, float viewHeight, float nearZ, float farZ)
{
	StoreMatrix(projection, XMMatrixTranspose(XMMatrixOrthographicLH(viewHeight, viewWidth, nearZ, farZ)));
}

// �p�[�X�y�N�e�B�u�ˉe�ϊ��s��쐬
void PerspectiveFovLHTp(Float4x4* projection, float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	StoreMatrix(projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ)));
}

// �r���[�ϊ��s��̍쐬
void LookAtLH(Float4x4* view, const Float3& eyePosition, const Float3& focusPosition, const Float3& upDirection)
{
	StoreMatrix(view, XMMatrixLookAtLH(LoadVector(eyePosition), LoadVector(focusPosition), LoadVector(upDirection)));
}

// �r���[�ϊ��s��̓]�u�s��̍쐬
void LookAtLHTp(Float4x4* view, const Float3& eyePosition, const Float3& focusPosition, const Float3& upDirection)
{
	StoreMatrix(view, XMMatrixTranspose(XMMatrixLookAtLH(LoadVector(eyePosition), LoadVector(focusPosition), LoadVector(upDirection))));
}


// ���[���h�ϊ��s��쐬
void WorldTp(Float4x4* world, const Float3& offset, const Float3& scale)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale.x(), scale.y(), scale.z()) *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldTp(Float4x4* world, const Float3& offset, const Quaternion& rotation)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixRotationQuaternion(LoadQuaternion(rotation)) *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldTp(Float4x4* world, const Float3& offset, const Quaternion& rotation, float scale)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale, scale, scale) *XMMatrixRotationQuaternion(LoadQuaternion(rotation)) *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldTp(Float4x4* world, const Float3& offset, float scale)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale, scale, scale) *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldTp(Float4x4* world, const Float3& offset)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}


//2D���[���h�ϊ��s��쐬
void World2DTp(Float4x4* world, const Float3& offset, const Float2& scale)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale.x(), scale.y(), 1.0f) *XMMatrixTranslation(offset.x(), offset.y(), 0.0f)));
}

void World2DTp(Float4x4* world, const Float3& offset, float scale)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale, scale, 1.0f) *XMMatrixTranslation(offset.x(), offset.y(), 0.0f)));
}

void World2DTp(Float4x4* world, const Float3& offset)
{
	StoreMatrix(world, XMMatrixTranspose(XMMatrixTranslation(offset.x(), offset.y(), 0.0f)));
}


// �p�[�e�B�N���r���{�[�h�p�̃��[���h�ϊ��s��쐬
void WorldBillboardTp(Float4x4* world, const Float3& offset, const Float2& scale, const Eye& eye)
{
	// �t�s��ŉ�]�s����쐬
	Matrix rotation =XMMatrixIdentity();
	rotation =XMMatrixLookAtLH(XMVECTOR(), XMVectorSet(eye.GetFocusPos().x()-eye.GetPos().x(), eye.GetFocusPos().y()-eye.GetPos().y(), eye.GetFocusPos().z()-eye.GetPos().z(), 0.0f), LoadVector(eye.GetUpDir()));
	Vector determ;
	rotation =XMMatrixInverse(&determ, rotation);

	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale.x(), scale.y(), 1.0f) *rotation *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldBillboardTp(Float4x4* world, const Float3& offset, float scale, const Eye& eye)
{
	// �t�s��ŉ�]�s����쐬
	Matrix rotation =XMMatrixIdentity();
	rotation =XMMatrixLookAtLH(XMVECTOR(), XMVectorSet(eye.GetFocusPos().x()-eye.GetPos().x(), eye.GetFocusPos().y()-eye.GetPos().y(), eye.GetFocusPos().z()-eye.GetPos().z(), 0.0f), LoadVector(eye.GetUpDir()));
	Vector determ;
	rotation =XMMatrixInverse(&determ, rotation);

	StoreMatrix(world, XMMatrixTranspose(XMMatrixScaling(scale, scale, 1.0f) *rotation *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}

void WorldBillboardTp(Float4x4* world, const Float3& offset, const Eye& eye)
{
	// �t�s��ŉ�]�s����쐬
	Matrix rotation =XMMatrixIdentity();
	rotation =XMMatrixLookAtLH(XMVECTOR(), XMVectorSet(eye.GetFocusPos().x()-eye.GetPos().x(), eye.GetFocusPos().y()-eye.GetPos().y(), eye.GetFocusPos().z()-eye.GetPos().z(), 0.0f), LoadVector(eye.GetUpDir()));
	Vector determ;
	rotation =XMMatrixInverse(&determ, rotation);

	StoreMatrix(world, XMMatrixTranspose(rotation *XMMatrixTranslation(offset.x(), offset.y(), offset.z())));
}


// �]�u�s��쐬
void MatrixTranspose(Float4x4* result, const Float4x4& source)
{
	(*result)(0, 0) =source(0, 0);
	(*result)(1, 0) =source(0, 1);
	(*result)(2, 0) =source(0, 2);
	(*result)(3, 0) =source(0, 3);
	(*result)(0, 1) =source(1, 0);
	(*result)(1, 1) =source(1, 1);
	(*result)(2, 1) =source(1, 2);
	(*result)(3, 1) =source(1, 3);
	(*result)(0, 2) =source(2, 0);
	(*result)(1, 2) =source(2, 1);
	(*result)(2, 2) =source(2, 2);
	(*result)(3, 2) =source(2, 3);
	(*result)(0, 3) =source(3, 0);
	(*result)(1, 3) =source(3, 1);
	(*result)(2, 3) =source(3, 2);
	(*result)(3, 3) =source(3, 3);
}


// �P�ʍs�񂩂ǂ���
//bool IsIdentity(const Float3x3& matrix);

//bool IsIdentity(const Float4x4& matrix);


// �َ�s��ϊ�
void MatrixConvert(Float4x4* result, const Float4x3& source)
{
	CWE_ASSERT(result);

	VectorConvert(&(*result)(0), source(0));
	VectorConvert(&(*result)(1), source(1));
	VectorConvert(&(*result)(2), source(2));
	VectorConvert(&(*result)(3), source(3));
	(*result)(3).w() =1.0f;
}




// ================= ���̑��̎Z�p�֐� =============================

// �x�W�F�Ȑ��֐�
void Bezier(Float2* bezier, const Float2& p0, const Float2& p1, const Float2& p2, const Float2& p3, float t)
{
	float t01 =1-t;
	bezier->x() =t01*t01*t01*p0.x() + 3*t01*t01*t*p1.x() + 3*t01*t*t*p2.x() + t*t*t*p3.x();
	bezier->y() =t01*t01*t01*p0.y() + 3*t01*t01*t*p1.y() + 3*t01*t*t*p2.y() + t*t*t*p3.y();
}

float CubicBezier(float x1, float y1, float x2, float y2, float t)
{
	float param = 0.5f;
	float approx =0.0f;

	//X function of the bezier curve.
	auto fx =[x1, x2](float tx){
		float s =1.0f -tx;
		return (3 *s*s *tx *x1) + (3 *s *tx*tx *x2) + (tx*tx*tx);
	};
	//Y function of the bezier curve.
	auto fy =[y1, y2](float ty){
		float s =1.0f -ty;
		return (3 *s*s *ty *y1) + (3 *s *ty*ty *y2) + (ty*ty*ty);
	};

	//Calculate the parameter from direction points using Newton's method.
	for (int i = 0; i < 15; i++)
	{
		approx =fx(param);

		if(math::NearlyEqual(approx, t)) {break;}

		if(approx > t)
		{
			param -= 1.0f / float(4 << i);
		}
		else //approx < t
		{
			param += 1.0f / float(4 << i);
		}
	}

	return fy(param);
}


// �x�N�g������x�N�g���։�]����N�H�[�^�j�I�����Z�o
void VectorToVectorRotaion(Quaternion* result, const Float3& fromV, const Float3& toV)
{
	Vector aFromV =LoadVector(fromV),
		   aToV =LoadVector(toV);
	Vector angle =XMVector3AngleBetweenVectors(aFromV, aToV),
		   axis =XMVector3Cross(aFromV, aToV);

	StoreQuaternion(result, XMQuaternionRotationAxis(axis, XMVectorGetX(angle)));
}


// ��]�N�H�[�^�j�I������I�C���[�p���Z�o
void RollPitchYawFromQuaternion(float& pitch, float& yaw, float& roll, const Quaternion& rotation)
{
	Matrix mRotation =XMMatrixRotationQuaternion(LoadQuaternion(rotation));

	//�s�b�`(X����])
	pitch =ASin(-mRotation(2, 1));
	//�W���o�����b�N���
	if(AvoidGimbalLockByPalamX < fabsf(pitch))
	{
		pitch =(pitch<0)?-AvoidGimbalLockByPalamX : AvoidGimbalLockByPalamX;
	}
	float cosineX =Cos(pitch);
	//���[(Y����])
	yaw =ATan((mRotation(2, 0)/cosineX), (mRotation(2, 2)/cosineX));
	//���[��(Z����])
	roll =ATan((mRotation(0, 1)/cosineX), (mRotation(1, 1)/cosineX));
}



}// math
}// CWE