#include "Player.h"
#include "Information.h"
#include "DebugString.h"
#include "Controller.h"
#include "../CWE/CWEMath.h"




// �X�^�e�B�b�N�萔
const wchar_t* const PlayerBase::smFailedModelMs =L"Failed to load model data.\nMay file is corrupted.";
const wchar_t* const PlayerBase::smFailedMotionMs =L"Failed to load motion data.\nMay file is corrupted.";


// �R���X�g���N�^
PlayerBase::PlayerBase(bool left, Controller* pController, const wchar_t* const filePath):
	mpController(pController), mIsRight(left), mHP(smMaxHP)
{
	mModel.Load(filePath, CWE::FileFormat::PMX);

	mPos.x() =left ? -70.0f : 70.0f;
}


// �f�X�g���N�^
PlayerBase::~PlayerBase()
{
	mModel.Release();
}


// �`��
void PlayerBase::Draw(const CWE::math::Float4x4& stage) const
{
	Information& info =Information::GetInfo();
	CWE_ASSERT(info.mModelShader.GetCBData() && info.mModelShader.GetBoneCBData());

	//���[���h�ϊ�
	CWE::math::MatrixMultiplyTp(&info.mModelShader.GetCBData()->mWorld, mLocal, stage);

	//�{�[�����̓K�p
	const std::vector<CWE::Bone>& boneData =mModel.GetBoneData(nullptr);
	for(unsigned i=0; i<boneData.size(); ++i)
	{
		if(MODEL_BONE_MAX <= i) {break;}
		CWE::math::MatrixTranspose(&info.mModelShader.GetBoneCBData()->mBoneMat[i], boneData[i].mMatrix);
	}

	//�X�y�L�����Ȃ�
	info.mModelShader.GetCBData()->mSpecular =0.0f;

	//�R���X�^���g�o�b�t�@�f�[�^�̓K�p
	info.mModelShader.mCB.Map();
	CopyMemory(info.mModelShader.mCB.Access(), info.mModelShader.GetCBData(), sizeof(*info.mModelShader.GetCBData()));
	info.mModelShader.mCB.Unmap();
	info.mModelShader.mBoneCB.Map();
	CopyMemory(info.mModelShader.mBoneCB.Access(), info.mModelShader.GetBoneCBData(), sizeof(*(info.mModelShader.GetBoneCBData())));
	info.mModelShader.mBoneCB.Unmap();

	//�`��
	mModel.Draw(false);
}