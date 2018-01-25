#include "Stage.h"

using namespace CWE;


// �d�͒l
const float Stage::smGravityAcceleration =(9.80665f /2) /15;
// ���C
const float Stage::smGroundFriction =0.05f;


// �R���X�g���N�^
Stage::Stage(BattleStage stage)
{
	switch(stage)
	{
	case BattleStage::StarryRuins:
		mModel.Load(L"Resource\\stage37\\����X�e�[�W_��Օ�(����).pmx", FileFormat::PMX);
		mSkyDome.Load(L"Resource\\stage37\\Dome_CC901(�S������).pmx", FileFormat::PMX);
		break;

	case BattleStage::Unknown:
		CWE_ASSERT(false); //�s���ȗ񋓎q
		break;

	default:
		CWE_ASSERT(false); //������
		break;
	}
}


// �f�X�g���N�^
Stage::~Stage()
{
	mModel.Release();
	mSkyDome.Release();
}


// �f�ޑS�̂̃��[�h���ʂ�\��
bool Stage::AllResourceLoadResult(Task equal) const
{
	return (mModel.LoadResult() == equal || 
		mSkyDome.LoadResult() == equal);
}


// ���[�h�̏󋵂�ʒm
bool Stage::LoadSuccess() const
{
	if(AllResourceLoadResult(Task::Failed))
	{
		FatalError::GetInstance().Outbreak(L"Failed to load model data.\nMay file is corrupted.");

		return false;
	}
	else if(AllResourceLoadResult(Task::Working))
	{
		return false;
	}

	return true;
}


// �X�V
void Stage::Update()
{

}


// �X�e�[�W���f���̕`��
void Stage::DrawStage(Information& info) const
{
	CWE_ASSERT(info.mModelShader.GetCBData());

	// ���[���h�ϊ�
	CWE::math::MatrixTranspose(&info.mModelShader.GetCBData()->mWorld, mLocal);

	info.mModelShader.GetCBData()->mSpecular =0.5f;

	// �R���X�^���g�o�b�t�@�f�[�^�̓K�p
	info.mModelShader.mCB.Map();
	CopyMemory(info.mModelShader.mCB.Access(), info.mModelShader.GetCBData(), sizeof(*info.mModelShader.GetCBData()));
	info.mModelShader.mCB.Unmap();

	// �`��
	mModel.Draw(true);
}


// �X�J�C�h�[�����f���̕`��
void Stage::DrawSkyDome(Information& info) const
{
	CWE_ASSERT(info.mModelShader.GetCBData());

	// ���[���h�ϊ�
	CWE::math::WorldTp(&info.mModelShader.GetCBData()->mWorld, {0.0f, 0.0f, 0.0f}, WORLD_SCALE);

	// �R���X�^���g�o�b�t�@�f�[�^�̓K�p
	info.mModelShader.mCB.Map();
	CopyMemory(info.mModelShader.mCB.Access(), info.mModelShader.GetCBData(), sizeof(*info.mModelShader.GetCBData()));
	info.mModelShader.mCB.Unmap();

	// �`��
	mSkyDome.Draw(false);
}