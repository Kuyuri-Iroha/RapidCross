#include "Information.h"
#include "../CWE/CWEUsefullyMacro.h"



using namespace CWE;

const wchar_t VERTEX_SHADER_ERROR_MS[] =L"Failed to create the vertex shader.\nThere is a possibility that the file is broken.";
const wchar_t PIXEL_SHADER_ERROR_MS[] =L"Failed to create the pixel shader.\nThere is a possibility that the file is broken.";
const wchar_t CONSTANT_BUFFER_ERROR_MS[] =L"Failed to create the constant buffer.\nThere is a possibility that the file is broken.";
const wchar_t SAMPLER_STATE_ERROR_MS[] =L"Failed to create the sampler state.\nThere is a possibility that the file is broken.";


// ======================== ModelShader ===================================


// �R���X�g���N�^
ModelShader::ModelShader():
	mpCBData(nullptr), mpBoneCBData(nullptr)
{

}


// �f�X�g���N�^
ModelShader::~ModelShader()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpBoneCBData);
}


// �V�F�[�_�̍쐬
void ModelShader::Create()
{
	IAInputElement ie[5];
	ie[0].Set("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, InputClassification::VertexData, 0);
	ie[1].Set("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, InputClassification::VertexData, 0);
	ie[2].Set("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, InputClassification::VertexData, 0);
	ie[3].Set("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 3, 0, InputClassification::VertexData, 0);
	ie[4].Set("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, InputClassification::VertexData, 0);
	if(!mVS.Create(L"Shader\\ModelVS.hlsl", nullptr, "ModelVS", ie, 5))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mShadowVS.Create(L"Shader\\ModelVS.hlsl", nullptr, "ModelShadowVS", ie, 5))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mStageVS.Create(L"Shader\\StageVS.hlsl", nullptr, "StageVS", ie, 3))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mStageShadowVS.Create(L"Shader\\StageVS.hlsl", nullptr, "StageShadowVS", ie, 3))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mPS.Create(L"Shader\\ModelPS.hlsl", nullptr, "ModelPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mDomePS.Create(L"Shader\\DomePS.hlsl", nullptr, "DomePS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mCB.Create(sizeof(CBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mBoneCB.Create(sizeof(BoneCBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mShadowCB.Create(sizeof(ShadowCBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mSampler.Create(SamplerStatus::Liner))
	{
		FatalError::GetInstance().Outbreak(SAMPLER_STATE_ERROR_MS);
	}
	if(!mShadowMapSampler.CreateForShadow())
	{
		FatalError::GetInstance().Outbreak(SAMPLER_STATE_ERROR_MS);
	}
}


// �V�F�[�_�p�X�̊J�n
void ModelShader::Begin(ID3D11ShaderResourceView* shadowMapTex) const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mBoneCB);
	manager.VSSetConstantBuffer(2, mShadowCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mPS);
	manager.PSSetConstantBuffer(0, mCB);
	manager.PSSetSampler(0, mSampler);
	manager.PSSetSampler(1, mShadowMapSampler);
	if(shadowMapTex)
	{
		manager.PSSetShaderResources(1, 1, &shadowMapTex);
	}
}


// �V�F�[�_�p�X�̏I��
void ModelShader::End() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSRemoveShader();
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// �V���h�E�}�b�v�`��p�V�F�[�_�p�X�̊J�n
void ModelShader::BeginShadow() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mShadowVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mBoneCB);
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// �X�e�[�W�V�F�[�_�p�X�̊J�n
void ModelShader::BeginStage(ID3D11ShaderResourceView* shadowMapTex) const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mStageVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mShadowCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mPS);
	manager.PSSetConstantBuffer(0, mCB);
	manager.PSSetSampler(0, mSampler);
	manager.PSSetSampler(1, mShadowMapSampler);
	manager.PSSetShaderResources(1, 1, &shadowMapTex);
}


// �X�e�[�W�̃V���h�E�}�b�v�`��p�V�F�[�_�p�X�̊J�n
void ModelShader::BeginStageShadow() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mStageShadowVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// �h�[���V�F�[�_�p�X�̊J�n
void ModelShader::BeginDome() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mStageVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mDomePS);
	manager.PSSetSampler(0, mSampler);
}


// �R���X�^���g�o�b�t�@�̗L����
void ModelShader::EnableConstantBuffer()
{
	if(!mpCBData) {mpCBData =new CBData;}
	if(!mpBoneCBData) {mpBoneCBData =new BoneCBData;}
	if(!mpShadowCBData) {mpShadowCBData =new ShadowCBData;}
}


// �R���X�^���g�o�b�t�@�̖�����
void ModelShader::DisableConstantBuffer()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpBoneCBData);
	SAFE_DELETE(mpShadowCBData);
}


// ======================== UIShader ===================================

// �R���X�g���N�^
UIShader::UIShader():
	mpCBData(nullptr), mpStaticCBData(nullptr)
{
	
}


// �f�X�g���N�^
UIShader::~UIShader()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}


// �V�F�[�_�̍쐬
void UIShader::Create()
{
	IAInputElement ie[2];
	ie[0].Set("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, InputClassification::VertexData, 0);
	ie[1].Set("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, InputClassification::VertexData, 0);
	if(!mVS.Create(L"Shader\\UIVS.hlsl", nullptr, "UIVS", ie, 2))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mPixShaders[PS_Picture].Create(L"Shader\\UIPS.hlsl", nullptr, "UIPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mPixShaders[PS_Loading].Create(L"Shader\\LoadingPS.hlsl", nullptr, "LoadingPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mPixShaders[PS_Title].Create(L"Shader\\TitlePS.hlsl", nullptr, "TitlePS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mPixShaders[PS_Result].Create(L"Shader\\ResultPS.hlsl", nullptr, "ResultPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mCB.Create(sizeof(CBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mStaticCB.Create(sizeof(StaticCBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mSampler.Create(SamplerStatus::Point))
	{
		FatalError::GetInstance().Outbreak(SAMPLER_STATE_ERROR_MS);
	}
}


// Begin the default shader path.
void UIShader::Begin() const
{
	Begin(PS_Picture);
}


// Begin the shader path.
void UIShader::Begin(PSIndex index) const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mStaticCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mPixShaders[index]);
	manager.PSSetConstantBuffer(0, mCB);
	manager.PSSetSampler(0, mSampler);
}


// �V�F�[�_�p�X�̏I��
void UIShader::End() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSRemoveShader();
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// �萔�o�b�t�@�̗L����
void UIShader::EnableConstantBuffer()
{
	if(!mpCBData) {mpCBData =new CBData;}
	if(!mpStaticCBData) {mpStaticCBData =new StaticCBData;}
}


// �萔�o�b�t�@�̖�����
void UIShader::DisableConstantBuffer()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}


// Update the projection matrix
void UIShader::UpdateProjection()
{
	CWE_ASSERT(mpStaticCBData);
	if(mpStaticCBData->mProjection != Polygon::Get2DProjection())
	{
		mpStaticCBData->mProjection =Polygon::Get2DProjection();
		mStaticCB.Map();
		CopyMemory(mStaticCB.Access(), &Polygon::Get2DProjection(), sizeof(math::Float4x4));
		mStaticCB.Unmap();
	}
}



// ======================== Ending Parameter Shader ===================================

EndParamShader::~EndParamShader()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}


void EndParamShader::Create()
{
	IAInputElement ie[2];
	ie[0].Set("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, InputClassification::VertexData, 0);
	ie[1].Set("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, InputClassification::VertexData, 0);
	if(!mVS.Create(L"Shader\\UIVS.hlsl", nullptr, "UIVS", ie, 2))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mPS.Create(L"Shader\\EndingParamPS.hlsl", nullptr, "ParamPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mCB.Create(sizeof(CBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mStaticCB.Create(sizeof(StaticCBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mSampler.Create(SamplerStatus::Liner))
	{
		FatalError::GetInstance().Outbreak(SAMPLER_STATE_ERROR_MS);
	}
}


// Begin of the shader path.
void EndParamShader::Begin() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mStaticCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mPS);
	manager.PSSetConstantBuffer(0, mCB);
	manager.PSSetSampler(0, mSampler);
}


// End of the shader path.
void EndParamShader::End() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSRemoveShader();
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// Enable of the constant buffer.
void EndParamShader::EnableConstantBuffer()
{
	if(!mpCBData) {
		mpCBData =new CBData;
	}
	if(!mpStaticCBData) {
		mpStaticCBData =new StaticCBData;
	}
}


// Disable of the constant buffer.
void EndParamShader::DisableConstantBuffer()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}


// ======================== HP Parameter Shader ===================================

HPParamShader::~HPParamShader()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}


void HPParamShader::Create()
{
	IAInputElement ie[2];
	ie[0].Set("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, InputClassification::VertexData, 0);
	ie[1].Set("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, InputClassification::VertexData, 0);
	if(!mVS.Create(L"Shader\\UIVS.hlsl", nullptr, "UIVS", ie, 2))
	{
		FatalError::GetInstance().Outbreak(VERTEX_SHADER_ERROR_MS);
	}
	if(!mPS.Create(L"Shader\\HPParamPS.hlsl", nullptr, "ParamPS"))
	{
		FatalError::GetInstance().Outbreak(PIXEL_SHADER_ERROR_MS);
	}
	if(!mCB.Create(sizeof(CBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mStaticCB.Create(sizeof(StaticCBData)))
	{
		FatalError::GetInstance().Outbreak(CONSTANT_BUFFER_ERROR_MS);
	}
	if(!mSampler.Create(SamplerStatus::Liner))
	{
		FatalError::GetInstance().Outbreak(SAMPLER_STATE_ERROR_MS);
	}
}


// Begin of the shader path.
void HPParamShader::Begin() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSSetShader(mVS);
	manager.VSSetConstantBuffer(0, mCB);
	manager.VSSetConstantBuffer(1, mStaticCB);
	manager.GSRemoveShader();
	manager.PSSetShader(mPS);
	manager.PSSetConstantBuffer(0, mCB);
	manager.PSSetSampler(0, mSampler);
}


// End of the shader path.
void HPParamShader::End() const
{
	Graphic& manager =Graphic::GetInstance();

	manager.VSRemoveShader();
	manager.GSRemoveShader();
	manager.PSRemoveShader();
}


// Enable of the constant buffer.
void HPParamShader::EnableConstantBuffer()
{
	if(!mpCBData) {
		mpCBData =new CBData;
	}
	if(!mpStaticCBData) {
		mpStaticCBData =new StaticCBData;
	}
}


// Disable of the constant buffer.
void HPParamShader::DisableConstantBuffer()
{
	SAFE_DELETE(mpCBData);
	SAFE_DELETE(mpStaticCBData);
}



// ======================== Information ===================================


// �R���X�g���N�^
Information::Information():
	mMagnifX(1.0f /float(CWE_DEFAULT_WINDOW_SIZE_X)), mMagnifY(1.0f /float(CWE_DEFAULT_WINDOW_SIZE_Y)), mEndRequest(false), mRuningScene(TaskName::Title), mWinner(PlayerUnknown), mEnded(false), mLoading(false)
{
	mDebugChars[0].Load(L"Resource\\DebugString\\0.png", FileFormat::Picture);
	mDebugChars[1].Load(L"Resource\\DebugString\\1.png", FileFormat::Picture);
	mDebugChars[2].Load(L"Resource\\DebugString\\2.png", FileFormat::Picture);
	mDebugChars[3].Load(L"Resource\\DebugString\\3.png", FileFormat::Picture);
	mDebugChars[4].Load(L"Resource\\DebugString\\4.png", FileFormat::Picture);
	mDebugChars[5].Load(L"Resource\\DebugString\\5.png", FileFormat::Picture);
	mDebugChars[6].Load(L"Resource\\DebugString\\6.png", FileFormat::Picture);
	mDebugChars[7].Load(L"Resource\\DebugString\\7.png", FileFormat::Picture);
	mDebugChars[8].Load(L"Resource\\DebugString\\8.png", FileFormat::Picture);
	mDebugChars[9].Load(L"Resource\\DebugString\\9.png", FileFormat::Picture);
	mDebugChars[10].Load(L"Resource\\DebugString\\a.png", FileFormat::Picture);
	mDebugChars[11].Load(L"Resource\\DebugString\\b.png", FileFormat::Picture);
	mDebugChars[12].Load(L"Resource\\DebugString\\c.png", FileFormat::Picture);
	mDebugChars[13].Load(L"Resource\\DebugString\\d.png", FileFormat::Picture);
	mDebugChars[14].Load(L"Resource\\DebugString\\e.png", FileFormat::Picture);
	mDebugChars[15].Load(L"Resource\\DebugString\\f.png", FileFormat::Picture);
	mDebugChars[16].Load(L"Resource\\DebugString\\g.png", FileFormat::Picture);
	mDebugChars[17].Load(L"Resource\\DebugString\\h.png", FileFormat::Picture);
	mDebugChars[18].Load(L"Resource\\DebugString\\i.png", FileFormat::Picture);
	mDebugChars[19].Load(L"Resource\\DebugString\\j.png", FileFormat::Picture);
	mDebugChars[20].Load(L"Resource\\DebugString\\k.png", FileFormat::Picture);
	mDebugChars[21].Load(L"Resource\\DebugString\\l.png", FileFormat::Picture);
	mDebugChars[22].Load(L"Resource\\DebugString\\m.png", FileFormat::Picture);
	mDebugChars[23].Load(L"Resource\\DebugString\\n.png", FileFormat::Picture);
	mDebugChars[24].Load(L"Resource\\DebugString\\o.png", FileFormat::Picture);
	mDebugChars[25].Load(L"Resource\\DebugString\\p.png", FileFormat::Picture);
	mDebugChars[26].Load(L"Resource\\DebugString\\q.png", FileFormat::Picture);
	mDebugChars[27].Load(L"Resource\\DebugString\\r.png", FileFormat::Picture);
	mDebugChars[28].Load(L"Resource\\DebugString\\s.png", FileFormat::Picture);
	mDebugChars[29].Load(L"Resource\\DebugString\\t.png", FileFormat::Picture);
	mDebugChars[30].Load(L"Resource\\DebugString\\u.png", FileFormat::Picture);
	mDebugChars[31].Load(L"Resource\\DebugString\\v.png", FileFormat::Picture);
	mDebugChars[32].Load(L"Resource\\DebugString\\w.png", FileFormat::Picture);
	mDebugChars[33].Load(L"Resource\\DebugString\\x.png", FileFormat::Picture);
	mDebugChars[34].Load(L"Resource\\DebugString\\y.png", FileFormat::Picture);
	mDebugChars[35].Load(L"Resource\\DebugString\\z.png", FileFormat::Picture);
	mDebugChars[36].Load(L"Resource\\DebugString\\[.].png", FileFormat::Picture);

	mModelShader.Create();
	mUIShader.Create();
	mEndParamShader.Create();
	mHPParamShader.Create();
}


// �f�X�g���N�^
Information::~Information()
{
	for(CWE::Polygon& debugChar : mDebugChars)
	{
		debugChar.Release();
	}
}


// �f�o�b�O�����S�Ă����[�h�ł��Ă��邩�ǂ���
bool Information::DebugCharsLoaded() const
{
	bool loaded =true;

	for(const CWE::Polygon& debugChar : mDebugChars)
	{
		if(debugChar.LoadResult() != Task::Success)
		{
			CWE_ASSERT(debugChar.LoadResult() != Task::Failed); //���[�h���s�͋����I��
			loaded =false;
			break;
		}
	}

	return loaded;
}


// �f�o�b�O������`��
void Information::DrawDebugChar(unsigned index) const
{
	CWE_ASSERT(index < DEBUG_CHAR_NUM);
	if(mDebugChars[index].LoadResult() == Task::Success)
	{
		mDebugChars[index].Draw(true);
	}
}