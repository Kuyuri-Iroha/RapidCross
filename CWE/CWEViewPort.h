#pragma once

#include "CWEIncludedAPI.h"
#include "CWEMath.h"


namespace CWE
{


//�ڃI�u�W�F�N�g
struct Eye
{
private:
	math::Float3 mPos; //���_�̈ʒu
	math::Float3 mFocusPos; //�œ_���W
	math::Float3 mUpDir; //��������W
	math::Float3 mNormalizedPos; //���K�����ꂽ���_�̈ʒu
	float mDistance; //���_����œ_�܂ł̋���
	bool mNormalizedPosIs; //���K������Ă��邩

public:


	//�R���X�g���N�^&�f�X�g���N�^
	Eye();
	Eye(const math::Float3& pos, const math::Float3& focusPos, const math::Float3& upDir);
	~Eye();


	// ���_���W�ړ��֐�

	//���݈ʒu����̈ړ�
	void PosMove(float x, float y, float z);
	//�ʒu�̕ύX
	void PosReset(float x, float y, float z);
	//�p�x��p���Č��݈ʒu����œ_�𒆐S�ɉ�]�ړ�
	// (r�l�̂ݑ����0.0f�ɂ���ƃG���[)
	void PosMoveByAngle(float xAngle, float yAngle, float r);
	//�p�x��p���Ĉʒu�̕ύX
//	void PosResetByRad(float xRad, float yRad, float r);


	// �œ_���W�ړ��֐�

	//���݈ʒu����̈ړ�
	void FocusMove(float x, float y, float z);
	//�ʒu�̕ύX
	void FocusReset(float x, float y, float z);
	//���_�ʒu�Ƌ��Ɍ��݈ʒu����̈ړ�
	void FocusMoveWithPos(float x, float y, float z);
	//���_�ʒu�Ƌ��Ɉʒu�̕ύX
	void FocusResetWithPos(float x, float y, float z);


	// ������ݒ�֐�

	//���݂̊p�x����ɕύX
	// ������
//	void UpDirMove(float rad);

	//������̕ύX
	void UpDirReset(float x, float y);

	// �r���[�ϊ��s��쐬
	void LookAtLHTp(math::Float4x4* view);


	// �������ւ̎Q��
	const math::Float3& GetPos()const {return mPos;}
	const math::Float3& GetFocusPos()const {return mFocusPos;}
	const math::Float3& GetUpDir()const {return mUpDir;}
	bool GetNormalized()const {return mNormalizedPosIs;}
	// ���_�Əœ_�Ԃ̋������擾
	//(��]�֐���r�l��ݒ�ς݂��ƍ���)
	float GetDistance()const;
	// ���_�Əœ_�̋�����2����擾
	float GetSqrDistance()const;
};


}//CWE
