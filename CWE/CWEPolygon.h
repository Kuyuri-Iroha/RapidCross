#pragma once

#include "CWEGraphicResource.h"
#include "CWEMath.h"
#include "CWETypeDefinition.h"
#include "CWEError.h"



namespace CWE
{
class Graphic;


class Motion;


// �|���S����ʃN���X
class Polygon
{
private:
	// 2D�\���p�̎ˉe�ϊ��s��
	static math::Float4x4 m2DProj;

	// ���\�[�X�N���X�ɃA�N�Z�X����ׂ̎Q��
	Resource &mResource;

	// ���\�[�X�ւ̃n���h��
	ResourceHandle mHandle;

	// �e�N�X�`���g�p�X���b�g
	unsigned mTexSlot;

	//���Ύp����񃊃Z�b�g
	void ResetPose(PolygonData& data);
	// FK�ł̃{�[���̍X�V
	void FKUpdate(PolygonData& data);
	// �w��{�[�������̊K�w�̃{�[���݂̂�FK�X�V
	void FKUpdate(PolygonData& data, unsigned boneIndex);
	// IK�̓K�p
	// (FK�X�V�ς݂ł��鎖���O��)
	void IKApply(PolygonData& data);
	// 3D�`��
	void Draw3D(const PolygonData& data, bool cull, bool eachMesh, unsigned numMeshes=0)const;
	// 2D�`��
	void Draw2D(const PolygonData& data)const;

public:
	// 2D�\���p�̎ˉe�ϊ��s����擾����
	static const math::Float4x4& Get2DProjection() {return m2DProj;}

	Polygon();
	Polygon(const Polygon &original, bool copyAll);
	Polygon(const Polygon& original);
	Polygon& operator=(const Polygon& original);
	~Polygon();

	// �ʒu���W
	math::Float3 mPosition;


	// �t�@�C���̃��[�h(�V�K���\�[�X�̐���)
	void Load(const wchar_t *filePath, const FileFormat &format);

	// �n���h���̎Q�Ɛ悪���݂��邩
	bool Is() const {mResource.IsPolygon(mHandle);}

	// ���݂̃n���h���̎Q�Ɛ悪�g�p�\���ǂ���
	const Task& LoadResult()const;

	// �f�[�^�̃R�s�[
	void Copy(const Polygon &original, bool copyAll);

	// �f�[�^�̊J��
	void Release();

	// �`��
	void Draw(bool cull)const;

	// ���b�V�����Ƃɕ`��
	void DrawMesh(unsigned meshNumber, bool cull)const;

	// hlsl�ł̃e�N�X�`���X���b�g�̃Z�b�g
	void SetTexSlot(unsigned slot) {CWE_ASSERT(slot<D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT); mTexSlot =slot;}

	// ���b�V���f�[�^�̎擾
	const std::vector<Mesh>& GetMeshData(bool* result) const;

	// ���b�V�������̎擾
	unsigned ReadMeshNum()const;

	// ���b�V�����̎擾
	const Mesh& ReadMesh(unsigned meshNumber)const;

	// �{�[���f�[�^�̎擾
	const std::vector<Bone>& GetBoneData(bool* result) const;
	std::vector<Bone>& GetBoneData(bool* result);

	// �{�[�������̎擾
	unsigned GetBoneNum()const;

	// �{�[�����̎擾
	const Bone& GetBone(unsigned boneNumber)const;

	// ���[�V������K�p���čĐ�
	// �߂�l�Ftrue=�Đ��� false=�Đ��I��
	bool MotionUpdate(Motion& motion, float frame);

	// ���[�V�������Ԃ��ēK�p
	void MotionLerp(Motion& from, Motion& to, float t);

	// FK�ł̃{�[���̍X�V
	bool FKApply();

	// IK�̓K�p
//	bool IKApply(); //���[�V�����t�@�C���ȊO�Ń{�[���𓮂��������Ȃ��������

	// ���_�ϊ��s��̍X�V
	bool UpdateSkinningMatrix();

};




}//CWE