#include "CWEPolygon.h"
#include "CWEMotion.h"
#include "CWEMacroConstants.h"
#include <algorithm>




namespace CWE
{


// 2D�\���p�̎ˉe�ϊ��s��
math::Float4x4 Polygon::m2DProj ={
	2.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 2.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};


// �f�t�H���g�R���X�g���N�^
Polygon::Polygon():
	mResource(Resource::GetInstance()), mTexSlot(0)
{
}

// �R�s�[�p�R���X�g���N�^
Polygon::Polygon(const Polygon &original, bool copyAll):
	mResource(Resource::GetInstance()), mHandle(), mTexSlot(original.mTexSlot)
{
	mHandle =mResource.CopyPolygonHandle(original.mHandle);
	if(copyAll) {mPosition =original.mPosition;}
}

Polygon::Polygon(const Polygon &original):
	mResource(Resource::GetInstance()), mHandle(), mTexSlot(original.mTexSlot), mPosition(original.mPosition)
{
	mHandle =mResource.CopyPolygonHandle(original.mHandle);
}

Polygon& Polygon::operator=(const Polygon& original)
{
	mHandle =mResource.CopyPolygonHandle(original.mHandle);
	mTexSlot =original.mTexSlot;
	mPosition =original.mPosition;
	return *this;
}

// �f�X�g���N�^
Polygon::~Polygon()
{
	Release();
}


// �t�@�C���̃��[�h
void Polygon::Load(const wchar_t *filePath, const FileFormat &format)
{
	mHandle =mResource.Create(filePath, format);
}


// ���݂̃n���h���̎Q�Ɛ悪�g�p�\���ǂ���
const Task& Polygon::LoadResult()const
{
	const PolygonData& data =mResource.GetPolygonData(mHandle);
	return data.mEnabled;
}


// �f�[�^�̃R�s�[
void Polygon::Copy(const Polygon &original, bool copyAll)
{
	mHandle =mResource.CopyPolygonHandle(original.mHandle);
	mTexSlot =original.mTexSlot;
	if(copyAll) {mPosition =original.mPosition;}
}


// �f�[�^�̊J��
void Polygon::Release()
{
	mResource.PolygonRelease(mHandle);
}


// �`��
void Polygon::Draw(bool cull)const
{
	const PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return;} //���K�f�[�^�łȂ���Ε`�悵�Ȃ�

	switch(data.mFileFormat)
	{
	case FileFormat::PMX:
		Draw3D(data, cull, false);
		break;

	case FileFormat::Picture:
		Draw2D(data);
		break;

	case FileFormat::Unknown:
		return;
		break;
	}
}


// ���b�V�����Ƃɕ`��
void Polygon::DrawMesh(unsigned meshNumber, bool cull)const
{
	const PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return;} //���K�f�[�^�łȂ���Ε`�悵�Ȃ�

	switch(data.mFileFormat)
	{
	case FileFormat::PMX:
		Draw3D(data, cull, true, meshNumber);
		break;

	case FileFormat::Picture:
		Draw2D(data);
		break;

	case FileFormat::Unknown:
		return;
		break;
	}
}


// ���b�V���f�[�^�̎擾
const std::vector<Mesh>& Polygon::GetMeshData(bool* result) const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(result)
	{
		*result =(data.mEnabled == Task::Success);
	}
	return data.mMesh;
}


// ���b�V�����̎擾
unsigned Polygon::ReadMeshNum()const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return -1;}
	return data.mMesh.size();
}


// ���b�V�����̎擾
const Mesh& Polygon::ReadMesh(unsigned meshNumber)const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	CWE_ASSERT(meshNumber<data.mMesh.size());
	CWE_ASSERT(data.mEnabled == Task::Success);
	return data.mMesh[meshNumber];
}


// �{�[���f�[�^�̎擾
const std::vector<Bone>& Polygon::GetBoneData(bool* result) const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(result)
	{
		*result =(data.mEnabled == Task::Success);
	}
	return data.mBones;
}

std::vector<Bone>& Polygon::GetBoneData(bool* result)
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(result)
	{
		*result =(data.mEnabled == Task::Success);
	}
	return data.mBones;
}


// �{�[�������̎擾
unsigned Polygon::GetBoneNum()const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return -1;}
	return data.mBones.size();
}


// �{�[�����̎擾
const Bone& Polygon::GetBone(unsigned boneNumber)const
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	CWE_ASSERT(boneNumber<mResource.GetPolygonData(mHandle).mBones.size());
	CWE_ASSERT(data.mEnabled == Task::Success);
	return mResource.GetPolygonData(mHandle).mBones[boneNumber];
}


// ���[�V������K�p���čĐ�
bool Polygon::MotionUpdate(Motion& motion, float frame)
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return false;}
	const unsigned boneNum =data.mBones.size();
	unsigned notLastFrame =0;
	unsigned i =0;
	ResetPose(data);

	// ���[�V�������Đ�
	for(i=0; i<boneNum; ++i)
	{
		if(motion.Update(data.mBones[i], frame))
		{
			++notLastFrame; //�I����Ă��Ȃ���������Z
		}
	}

	// ���[�V�������FK�X�V
	FKUpdate(data);

	// IK�̓K�p
	IKApply(data);

	//���_�ϊ��s��쐬
	for(i=0; i<boneNum; ++i)
	{
		math::MatrixMultiply(&data.mpHierarchy[i]->mMatrix, data.mpHierarchy[i]->mInvOffsetPose, data.mpHierarchy[i]->mPose);
	}

	return notLastFrame != 0;
}


// ���[�V�������Ԃ��ēK�p
void Polygon::MotionLerp(Motion& from, Motion& to, float t)
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return;}
	const unsigned boneNum =data.mBones.size();
	math::Float3A pos1, pos2;
	math::QuaternionA rotation1, rotation2;
	ResetPose(data);

	//���[�V������K�p
	for(auto& bone : data.mBones)
	{
		//�|�[�Y�̎擾
		from.GetNowPose(bone, pos1, rotation1);
		to.GetNowPose(bone, pos2, rotation2);

		//���
		math::VectorLerp(&bone.mPosition, pos1, pos2, t, t, t);
		
		math::QuaternionSlerp(&bone.mRotation, rotation1, rotation2, t);

		//�{�[���ɓK�p
		math::MatrixTranslationR(&bone.mPose, bone.mPosition, bone.mRotation); //mPose���ꎞ�I�ɗ��p
		math::MatrixMultiply(&bone.mRelatPose, bone.mPose, bone.mRelatPose);
	}

	//���[�V�������FK�X�V
	FKUpdate(data);

	//IK�̓K�p
	IKApply(data);

	//���_�ϊ��s��쐬
	for(auto& hierarchy : data.mpHierarchy)
	{
		math::MatrixMultiply(&hierarchy->mMatrix, hierarchy->mInvOffsetPose, hierarchy->mPose);
	}
}


// FK���{�[���ɓK�p
bool Polygon::FKApply()
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return false;}

	FKUpdate(data);
	return true;
}


// ���_�ϊ��s��̍X�V
bool Polygon::UpdateSkinningMatrix()
{
	PolygonData& data =mResource.GetPolygonData(mHandle);
	if(data.mEnabled != Task::Success) {return false;}

	for(unsigned i=0; i<data.mpHierarchy.size(); ++i)
	{
		math::MatrixMultiply(&data.mpHierarchy[i]->mMatrix, data.mpHierarchy[i]->mInvOffsetPose, data.mpHierarchy[i]->mPose);
	}

	return true;
}


// =============== �v���C�x�[�g =================================


// ���Ύp�����Z�b�g
void Polygon::ResetPose(PolygonData& data)
{
	for(auto& bone:data.mBones)
	{
		bone.mRelatPose =bone.mOffsetRelatPose;
	}
}


// FK�ł̃{�[���̍X�V
void Polygon::FKUpdate(PolygonData& data)
{
	const unsigned boneNum =data.mpHierarchy.size();
	
	//�K�w�v�Z
	for(unsigned i=0; i<boneNum; ++i)
	{
		//FK�K�p
		if(data.mpHierarchy[i]->mFather != -1) //���[�g�{�[������Ȃ����
		{
			math::MatrixMultiply(&data.mpHierarchy[i]->mPose, data.mpHierarchy[i]->mRelatPose, data.mBones[data.mpHierarchy[i]->mFather].mPose);
		}
		else //���[�g�{�[���Ȃ�
		{
			data.mpHierarchy[i]->mPose =data.mpHierarchy[i]->mRelatPose;
		}
	}
}
void Polygon::FKUpdate(PolygonData& data, unsigned boneIndex)
{
	const unsigned boneNum =data.mpHierarchy.size();
	CWE_ASSERT(boneIndex<boneNum);
	bool start =false;
	
	//�K�w�v�Z
	for(unsigned i=0; i<boneNum; ++i)
	{
		if(data.mpHierarchy[i]->mSelf == boneIndex)
		{
			start =true;
		}

		if(start)
		{
			//FK�K�p
			if(data.mpHierarchy[i]->mFather != -1) //���[�g�{�[������Ȃ����
			{
				math::MatrixMultiply(&data.mpHierarchy[i]->mPose, data.mpHierarchy[i]->mRelatPose, data.mBones[data.mpHierarchy[i]->mFather].mPose);
			}
			else //���[�g�{�[���Ȃ�
			{
				data.mpHierarchy[i]->mPose =data.mpHierarchy[i]->mRelatPose;
			}
		}
	}//for()
}


// IK�̓K�p
void Polygon::IKApply(PolygonData& data)
{
	using namespace math;
	const unsigned ikBoneNum =data.mIKBone.size();
	unsigned j =0,
			 k =0;
	Vector epsil =VectorSet(0.0000001f, 0.0000001f, 0.0000001f, 0.0000001f); //���e�덷�l
	Matrix mInvLink_RelatRota;
	Vector vTarget,
		   vIK;
	float rotaAngle;
	Vector rotaAxis;
	Vector qRotation;
	float pitch, yaw, roll,
		  prevPitch, prevYaw, prevRoll;
	Matrix mRotation,
		   mInvRotation;

	for(unsigned i=0; i<ikBoneNum; ++i)
	{
		//�ߎ��Z�o���[�v
		for(j=0; j<data.mIKBone[i].mLoop+30; ++j) //���x�����̂��߃��[�v�񐔂𒲐�
		{
			//IK�����N�{�[�����[�v
			for(k=0; k<data.mIKBone[i].mLinks.size(); ++k)
			{
				IKBone::Link& ikLink =data.mIKBone[i].mLinks[k];

				//�����N�{�[���̃��[�J����Ԃɕϊ�
				mInvLink_RelatRota =MatrixInverse(LoadMatrix(ikLink.mpBone->mPose));
				vTarget =VectorSet(data.mIKBone[i].mpTarget->mPose(3).x(), data.mIKBone[i].mpTarget->mPose(3).y(), data.mIKBone[i].mpTarget->mPose(3).z(), 0.0f);
				vIK =VectorSet(data.mIKBone[i].mpSelf->mPose(3).x(), data.mIKBone[i].mpSelf->mPose(3).y(), data.mIKBone[i].mpSelf->mPose(3).z(), 0.0f);

				vTarget =Vector3Transform(vTarget, mInvLink_RelatRota);
				vTarget =Vector3Normalize(vTarget);
				vIK =Vector3Transform(vIK, mInvLink_RelatRota);
				vIK =Vector3Normalize(vIK);

				if(Vector3NearlyEqual(vTarget, vIK, epsil)) //�ߎ������ł����
				{
					continue;
				}

				//��]�p(�����p)
				rotaAngle =VectorGetX(
					Vector3AngleBetweenNormal(vTarget, vIK)
					);

				//��]�p�̒P�ʊp����
				rotaAngle =std::min<float>(rotaAngle, data.mIKBone[i].mUnitAngle);
				rotaAngle =std::max<float>(rotaAngle, -data.mIKBone[i].mUnitAngle);

				//��]��
				rotaAxis =Vector3Cross(vTarget, vIK);
				rotaAxis =Vector3Normalize(rotaAxis);

				if(Vector3NearlyEqual(rotaAxis, VectorSet(0.0f, 0.0f, 0.0f, 0.0f), epsil)) //2�x�N�g�������s�ł����
				{
					continue;
				}

				//��]�N�H�[�^�j�I��
				qRotation =QuaternionRotationAxis(rotaAxis, rotaAngle);
				if(ikLink.mExistAngleLimited) //�p�x��������
				{
					//�I�C���[�p�ɕϊ�
					RollPitchYawFromQuaternion(pitch, yaw, roll, qRotation);

					//���ɎZ�o����Ă����]����I�C���[�p���Z�o
					mInvLink_RelatRota =MatrixIdentity();
					mInvLink_RelatRota.r[0] =LoadVector(ikLink.mpBone->mRelatPose(0));
					mInvLink_RelatRota.r[1] =LoadVector(ikLink.mpBone->mRelatPose(1));
					mInvLink_RelatRota.r[2] =LoadVector(ikLink.mpBone->mRelatPose(2));
					RollPitchYawFromMatrix(prevPitch, prevYaw, prevRoll, mInvLink_RelatRota);

					//�p�x����
					if(!NearlyEqual(prevPitch, 0.0f))
					{
						if(NearlyEqual(ikLink.mLimitAngleMin.x()-ikLink.mLimitAngleMax.x(), 0.0f)) {prevPitch =0.0f;}
						pitch =std::max<float>(pitch, ikLink.mLimitAngleMin.x()-prevPitch);
						pitch =std::min<float>(pitch, ikLink.mLimitAngleMax.x()-prevPitch);
					}
					else
					{
						pitch =std::max<float>(pitch, ikLink.mLimitAngleMin.x());
						pitch =std::min<float>(pitch, ikLink.mLimitAngleMax.x());
					}
					if(!NearlyEqual(prevYaw, 0.0f))
					{
						if(NearlyEqual(ikLink.mLimitAngleMin.y()-ikLink.mLimitAngleMax.y(), 0.0f)) {prevYaw =0.0f;}
						yaw =std::max<float>(yaw, ikLink.mLimitAngleMin.y()-prevYaw);
						yaw =std::min<float>(yaw, ikLink.mLimitAngleMax.y()-prevYaw);
					}
					else
					{
						yaw =std::max<float>(yaw, ikLink.mLimitAngleMin.y());
						yaw =std::min<float>(yaw, ikLink.mLimitAngleMax.y());
					}
					if(!NearlyEqual(prevRoll, 0.0f))
					{
						if(NearlyEqual(ikLink.mLimitAngleMin.z()-ikLink.mLimitAngleMax.z(), 0.0f)) {prevRoll =0.0f;}
						roll =std::max<float>(roll, ikLink.mLimitAngleMin.z()-prevRoll);
						roll =std::min<float>(roll, ikLink.mLimitAngleMax.z()-prevRoll);
					}
					else
					{
						roll =std::max<float>(roll, ikLink.mLimitAngleMin.z());
						roll =std::min<float>(roll, ikLink.mLimitAngleMax.z());
					}
					
					//��]�s��
					mRotation =MatrixRotationRollPitchYaw(pitch, yaw, roll);
				}
				else //�p�x�����Ȃ�
				{
					mRotation =MatrixRotationQuaternion(qRotation);
				}

				//���΍s��ɓK�p
				StoreMatrix(&ikLink.mpBone->mRelatPose,
					MatrixMultiply(mRotation, LoadMatrix(ikLink.mpBone->mRelatPose)));

				//����I��FK�̍X�V
				FKUpdate(data, ikLink.mpBone->mSelf);
			}
		}
	}
}


// 3D�`��
void Polygon::Draw3D(const PolygonData& data, bool cull, bool eachMesh, unsigned meshNumber)const
{
	Graphic& manager =Graphic::GetInstance();

	// �J�����O�𖳌���
	manager.RSResetUseState(cull ? RSState::Normal : RSState::NonCull);
	// IA�ɒ��_�o�b�t�@��ݒ�
	const unsigned stride[] ={sizeof(math::Float3), sizeof(math::Float2), sizeof(math::Float3), sizeof(math::Int4), sizeof(math::Float4), sizeof(math::Float3), sizeof(math::Float3), sizeof(math::Float3)};
	const unsigned offset[] ={0, 0, 0, 0, 0, 0, 0, 0};
	manager.IASetVertexBuffers(0, data.VERTEX_BUFFER_NUM, data.mpVertexBuffers, stride, offset);
	// IA�ɃC���f�b�N�X�E�o�b�t�@��ݒ�
	manager.IASetIndexBuffer(data.mpIndexBuffer, 0);

	// ����܂łɕ`�悵�����_��
	unsigned drewVertexNum =0;
	unsigned i =0;

	// ���b�V�����̕`�悩�ǂ���
	if(eachMesh)
	{
		CWE_ASSERT(meshNumber<data.mMesh.size());
		i =meshNumber;
		++meshNumber;
		//�`�悷�郁�b�V���̍ŏ��̒��_�ԍ����Z�o
		for(unsigned j=0; j<i; ++j)
		{
			drewVertexNum +=data.mMesh[j].mVertexNum;
		}
	}
	else
	{
		meshNumber =data.mMesh.size();
	}

	// ���b�V����1��1�`��
	for(; i<meshNumber; ++i)
	{
		// �V�F�[�_���\�[�X�r���[���s�N�Z���V�F�[�_�ɃZ�b�g
		manager.PSSetShaderResources(mTexSlot, 1, &data.mMesh[i].mpTexture);

		// ���b�V����`��
		manager.DrawIndexed(data.mMesh[i].mVertexNum, drewVertexNum, 0);

		// ����܂łɕ`�悵�����_�����L�^
		drewVertexNum +=data.mMesh[i].mVertexNum;
	}
}


// 2D�`��
void Polygon::Draw2D(const PolygonData& data)const
{
	Graphic& manager =Graphic::GetInstance();

	// �J�����O��L����
	manager.RSResetUseState(RSState::Normal);
	// IA�ɒ��_�o�b�t�@��ݒ�
	const unsigned stride[] ={sizeof(math::Float3), sizeof(math::Float2)};
	const unsigned offset[] ={0, 0};
	manager.IASetVertexBuffers(0, 2, data.mpVertexBuffers, stride, offset);
	// IA�ɃC���f�b�N�X�E�o�b�t�@��ݒ�
	manager.IASetIndexBuffer(data.mpIndexBuffer, 0);

	// �V�F�[�_���\�[�X�r���[���s�N�Z���V�F�[�_�ɃZ�b�g
	manager.PSSetShaderResources(mTexSlot, 1, &data.mMesh[0].mpTexture);		

	// ���b�V����`��
	manager.DrawIndexed(data.mMesh[0].mVertexNum, 0, 0);
}




}// CWE