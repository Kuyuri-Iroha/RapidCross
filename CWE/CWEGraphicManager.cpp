#include "CWEGraphicManager.h"
#include "CWEError.h"
#include "CWEPolygon.h"
#include <vector>


#define RAW_STRING_W(var) L#var


namespace CWE
{

//==================================================================================================================================================================================================
//
// �����_�����O�p�C�v���C��
//
//==================================================================================================================================================================================================


static float CLEAR_COLOR[4] ={0.0f, 0.0f, 0.0f, 1.0f};


// �V�F�[�_�[�}�N����`
ShaderMacro::ShaderMacro(const char* name, const char* definition)
{
	Set(name, definition);
}

void ShaderMacro::Set(const char* name, const char* definition)
{
	mShaderMacro.Name =name;
	mShaderMacro.Definition =definition;
}


// ========== �����_�����O�p�C�v���C�� ==================

// ���̓G�������g
IAInputElement::IAInputElement(const char* semanticName, unsigned semanticIndex, const Format& format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate)
{
	Set(semanticName, semanticIndex, format, inputSlot, alignedByteOffset, inputSlotClass, instanceDataStepRate);
}

void IAInputElement::Set(const char* semanticName, unsigned semanticIndex, const Format& format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate)
{
	mIE.SemanticName =semanticName;
	mIE.SemanticIndex =semanticIndex;
	mIE.Format =format;
	mIE.InputSlot =inputSlot;
	mIE.AlignedByteOffset =alignedByteOffset;
	mIE.InputSlotClass =inputSlotClass==InputClassification::VertexData ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
	mIE.InstanceDataStepRate =instanceDataStepRate;
}


// ���̓��C�A�E�g&���_�V�F�[�_�I�u�W�F�N�g
bool IAAndVS::Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName, const IAInputElement* pInputElements, unsigned numElements)
{
	Graphic& master =Graphic::GetInstance();

	// �f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�V�F�[�_�R�[�h�̃R���p�C���I�v�V����
#if defined(DEBUG) || defined(_DEBUG)
	UINT compileOption =D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
	//�R���p�C���ς݃V�F�[�_�̎g�p���O��
	UINT compileOption =D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#endif

	//�V�F�[�_�̃o�C�g�R�[�h���i�[����u���u
	ID3DBlob* pBlob =nullptr;

	//���_�V�F�[�_�R�[�h�̃R���p�C��
	HRESULT hr;
	if(pShaderMacros) {D3DX11CompileFromFile(shaderFile, &pShaderMacros[0].mShaderMacro, NULL, functionName, "vs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	else {D3DX11CompileFromFile(shaderFile, NULL, NULL, functionName, "vs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	if(FAILED(hr))
	{
		FatalError::GetInstance().Outbreak(L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}
	//���_�V�F�[�_�̍쐬
	if(FAILED(master.mpDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &mpVertexShader)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}

	//���̓��C�A�E�g�I�u�W�F�N�g�̍쐬
	if(!pInputElements) {return false;}
	if(FAILED(master.mpDevice->CreateInputLayout(&pInputElements[0].mIE, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &mpInputLayout)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"���̓��C�A�E�g�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���\n�������p���ł��܂���");
		return false;
	}
	//���_�V�F�[�_�I�u�W�F�N�g�̍쐬�I��
	SAFE_RELEASE(pBlob);

	return true;
}


// �W�I���g���V�F�[�_�I�u�W�F�N�g�̍쐬
bool GS::Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName)
{
	Graphic& master =Graphic::GetInstance();

	// �f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�V�F�[�_�R�[�h�̃R���p�C���I�v�V����
#if defined(DEBUG) || defined(_DEBUG)
	UINT compileOption =D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
	UINT compileOption =D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#endif

	//�V�F�[�_�̃o�C�g�R�[�h���i�[����u���u
	ID3DBlob* pBlob =nullptr;

	//�W�I���g���V�F�[�_�R�[�h�̃R���p�C��
	HRESULT hr;
	if(pShaderMacros) {D3DX11CompileFromFile(shaderFile, &pShaderMacros[0].mShaderMacro, NULL, functionName, "gs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	else {D3DX11CompileFromFile(shaderFile, NULL, NULL, functionName, "gs_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	if(FAILED(hr))
	{
		FatalError::GetInstance().Outbreak(L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}

	//�W�I���g���V�F�[�_�̍쐬
	if(FAILED(master.mpDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &mpGeometryShader)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}
	//�W�I���g���V�F�[�_�I�u�W�F�N�g�̍쐬�I��
	SAFE_RELEASE(pBlob);

	return true;
}


// �s�N�Z���V�F�[�_�I�u�W�F�N�g�̍쐬
bool PS::Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName)
{
	Graphic& master =Graphic::GetInstance();

	// �f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�V�F�[�_�R�[�h�̃R���p�C���I�v�V����
#if defined(DEBUG) || defined(_DEBUG)
	UINT compileOption =D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
	//�R���p�C���ς݃V�F�[�_�̎g�p���O��
	UINT compileOption =D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#endif

	//�V�F�[�_�̃o�C�g�R�[�h���i�[����u���u
	ID3DBlob* pBlob =nullptr;

	//�s�N�Z���V�F�[�_�R�[�h�̃R���p�C��
	HRESULT hr;
	if(pShaderMacros) {D3DX11CompileFromFile(shaderFile, &pShaderMacros[0].mShaderMacro, NULL, functionName, "ps_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	else {D3DX11CompileFromFile(shaderFile, NULL, NULL, functionName, "ps_5_0", compileOption, NULL, NULL, &pBlob, NULL, &hr);}
	if(FAILED(hr))
	{
		FatalError::GetInstance().Outbreak(L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}

	//�s�N�Z���V�F�[�_�̍쐬
	if(FAILED(master.mpDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &mpPixelShader)))
	{
		//�J�����Ă���I��
		SAFE_RELEASE(pBlob);
		FatalError::GetInstance().Outbreak(L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���\n�t�@�C�����j�����Ă���\��������܂�");
		return false;
	}
	//�s�N�Z���V�F�[�_�I�u�W�F�N�g�̍쐬�I��
	SAFE_RELEASE(pBlob);

	return true;
}


// �v���Z�b�g����쐬
bool SamplerState::Create(const SamplerStatus& preset)
{
	Graphic& master =Graphic::GetInstance();
	// �f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�T���v���[�X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU =D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV =D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW =D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias =0.0f;
	samplerDesc.MaxAnisotropy =0;
	samplerDesc.ComparisonFunc =D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] =0.0f;
	samplerDesc.BorderColor[1] =0.0f;
	samplerDesc.BorderColor[2] =0.0f;
	samplerDesc.BorderColor[3] =0.0f;
	samplerDesc.MaxLOD =FLT_MAX;
	samplerDesc.MinLOD =-FLT_MAX;

	switch(preset)
	{
	case SamplerStatus::Liner:
		samplerDesc.Filter =D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;

	case SamplerStatus::Point:
		samplerDesc.Filter =D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	}


	//�T���v���[�X�e�[�g�̍쐬
	if(FAILED(master.mpDevice->CreateSamplerState(&samplerDesc, &mpSamplerState)))
	{
		FatalError::GetInstance().Outbreak(L"�T���v���[�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}

// �V���h�E�}�b�v�p�̍쐬
bool SamplerState::CreateForShadow()
{
	Graphic& master =Graphic::GetInstance();
	// �f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�T���v���[�X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU =D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV =D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW =D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias =0.0f;
	samplerDesc.MaxAnisotropy =1;
	samplerDesc.ComparisonFunc =D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc.Filter =D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.BorderColor[0] =1.0f;
	samplerDesc.BorderColor[1] =1.0f;
	samplerDesc.BorderColor[2] =1.0f;
	samplerDesc.BorderColor[3] =1.0f;
	samplerDesc.MaxLOD =FLT_MAX;
	samplerDesc.MinLOD =-FLT_MAX;

	//�T���v���[�X�e�[�g�̍쐬
	if(FAILED(master.mpDevice->CreateSamplerState(&samplerDesc, &mpSamplerState)))
	{
		FatalError::GetInstance().Outbreak(L"�T���v���[�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}


// �R���X�^���g�o�b�t�@�̍쐬
bool ConstantBuffer::Create(size_t bufferSize)
{
	Graphic& master =Graphic::GetInstance();
	//�f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpDevice);

	//�ݒ�
	D3D11_BUFFER_DESC constBafferDesc;
	constBafferDesc.Usage =D3D11_USAGE_DYNAMIC;
	constBafferDesc.BindFlags =D3D11_BIND_CONSTANT_BUFFER;
	constBafferDesc.CPUAccessFlags =D3D11_CPU_ACCESS_WRITE;
	constBafferDesc.MiscFlags =0;
	constBafferDesc.StructureByteStride =0;
	constBafferDesc.ByteWidth =bufferSize;

	//�萔�o�b�t�@�̍쐬
	if(FAILED(master.mpDevice->CreateBuffer(&constBafferDesc, NULL, &mpConstantBuffer)))
	{
		FatalError::GetInstance().Outbreak(L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}

// �R���X�^���g�o�b�t�@�̏������݊J�n
bool ConstantBuffer::Map()
{
	Graphic& master =Graphic::GetInstance();
	//�f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpImmediateContext);

	if(FAILED(master.mpImmediateContext->Map(mpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource)))
	{
		return false;
	}

	mMapped =true;
	return true;
}

// �R���X�^���g�o�b�t�@�̏������ݏI��
void ConstantBuffer::Unmap()
{
	Graphic& master =Graphic::GetInstance();
	//�f�o�C�X�������ς݂ł��邱�Ƃ�ۏ؂���
	CWE_ASSERT(master.mpImmediateContext);

	master.mpImmediateContext->Unmap(mpConstantBuffer, 0);

	mMapped =false;
}

// �T�u���\�[�X�ւ̏�������
void* const ConstantBuffer::Access()
{
	if(!mMapped)
	{
		return nullptr;
	}
	return mResource.pData;
}


// ============ �����_�[�^�[�Q�b�g�r���[�I�u�W�F�N�g ==========

// �N���A
void RenderTargetView::Clear()
{
	Graphic::GetInstance().DeviceContext().ClearRenderTargetView(mpRenderTargetView, CLEAR_COLOR);
}


// ============ �[�x/�X�e���V���r���[�I�u�W�F�N�g =============

// �N���A
void DepthStencilView::Clear()
{
	//���͌Œ�l
	Graphic::GetInstance().DeviceContext().ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



//==================================================================================================================================================================================================
//
// �e�N�X�`��
//
//==================================================================================================================================================================================================


// ======== 2D�e�N�X�`�� =================

// �e�N�X�`���̍쐬
bool Texture2D::Create(const Texture2DDesc& desc)
{
	if(FAILED( Graphic::GetInstance().Device().CreateTexture2D(&desc, nullptr, &mpTexture) )) //�T�u���\�[�X�͖�����
	{
		return false;
	}
	
	return true;
}


//==================================================================================================================================================================================================
//
// �`��n�Ǘ��N���X
//
//==================================================================================================================================================================================================

//�R���X�g���N�^
Graphic::Graphic():
mWindows(),
mpFactory(nullptr), mpDevice(nullptr), mpImmediateContext(nullptr), mpSwapChain(nullptr),
mRS(), mOM(), mSyncInterval(1)
{
	mFormat[0] =DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //�o�b�N�o�b�t�@
	mFormat[1] =DXGI_FORMAT_D24_UNORM_S8_UINT; //�[�x/�X�e���V���o�b�t�@

	mMSAAQuality.Count =1;
	mMSAAQuality.Quality =0;
}

//�f�X�g���N�^
Graphic::~Graphic()
{
}

//�C���X�^���X�̎擾
Graphic& Graphic::GetInstance()
{
	static Graphic instance;
	return instance;
}


//������
bool Graphic::Init()
{
	//IDXGIFactory�C���^�[�t�F�C�X�̎擾
	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&mpFactory)))
	{
		FatalError::GetInstance().Outbreak(FatalError::smDeviceCreationErrorMS);
		return false;
	}
	//�f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	if(!CreateDeviceAndSwapChain()) {return false;}
	//�`��p�C�v���C���n�I�u�W�F�N�g�̍쐬
	if(!CreateBlendState()) {return false;}
	if(!CreateRasterizerState()) {return false;}
	if(!CreateDepthStencilState()) {return false;}
	
	//�o�b�N�o�b�t�@�[�̃��Z�b�g
	if(!ResetBackBuffer())
	{
		FatalError::GetInstance().Outbreak(FatalError::smDeviceCreationErrorMS);
		return false;
	}

	//[Alt]+[Enter]�L�[�ɂ���ʃ��[�h�؂�ւ��@�\�𖳌��ɂ���(���ʂȂ�)
	mpFactory->MakeWindowAssociation(mWindows.mMainWindow, DXGI_MWA_NO_ALT_ENTER);

	//�`��p�C�v���C���̏�����
	InitRenderingPipeline();
	RenderingPipelineRestoration();

	return true;
}


//�I������
void Graphic::End()
{
	//�f�o�C�X�X�e�[�g�̃N���A
	if(mpImmediateContext) {mpImmediateContext->ClearState();}

	//�X���b�v�`�F�C�����E�C���h�E���[�h�ɂ���
	if(mpSwapChain) {mpSwapChain->SetFullscreenState(FALSE, NULL);}

	//�擾�����C���^�[�t�F�C�X�̊J��
	SAFE_RELEASE(mOM.mpDepthStencilState);
	for(int i=0; i<_countof(mRS.mpRasterizerState); ++i) {SAFE_RELEASE(mRS.mpRasterizerState[i]);}
	SAFE_RELEASE(mOM.mpBlendState);
	SAFE_RELEASE(mOM.mpBlendState);
	SAFE_RELEASE(mOM.mpRenderTargetView);
	SAFE_RELEASE(mpSwapChain);
	SAFE_RELEASE(mpImmediateContext);
	SAFE_RELEASE(mpDevice);
	SAFE_RELEASE(mpFactory);
}


//�X�V����
void Graphic::Update()
{
	//�o�b�N�o�b�t�@�̕\��
	mpSwapChain->Present(mSyncInterval, 0);

	//�`��^�[�Q�b�g�̃N���A
	mpImmediateContext->ClearRenderTargetView(mOM.mpRenderTargetView, CLEAR_COLOR);
	//�[�x/�X�e���V���̃N���A
	mpImmediateContext->ClearDepthStencilView(mOM.mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


//Device and swap chain creation.
bool Graphic::CreateDeviceAndSwapChain()
{
	D3D_FEATURE_LEVEL featureLevels =D3D_FEATURE_LEVEL_11_0,
					  featureLevelsSupported;

	//Create device.
	if(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevels, 1, D3D11_SDK_VERSION, &mpDevice, &featureLevelsSupported, &mpImmediateContext))
	{
		FatalError::GetInstance().Outbreak(FatalError::smDeviceCreationErrorMS);
		return false;
	}

	//Description of swap chain
	DXGI_SWAP_CHAIN_DESC swapStatus;
	ZeroMemory(&swapStatus, sizeof(swapStatus));
	swapStatus.BufferCount =1;
	swapStatus.BufferDesc.Width =mWindows.mInitialWindowSizeX;
	swapStatus.BufferDesc.Height =mWindows.mInitialWindowSizeY;
	swapStatus.BufferDesc.Format =mFormat[0];
	swapStatus.BufferDesc.RefreshRate.Numerator =60;
	swapStatus.BufferDesc.RefreshRate.Denominator =1;
//	swapStatus.BufferDesc.Scaling =DXGI_MODE_SCALING_CENTERED;
//	swapStatus.BufferDesc.ScanlineOrdering =DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapStatus.BufferUsage =DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapStatus.OutputWindow =mWindows.mMainWindow;
	swapStatus.SampleDesc =mMSAAQuality;
	swapStatus.Windowed =mWindows.mIsWindowMode;
	swapStatus.Flags =DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//Create swap chain.
	if(FAILED(mpFactory->CreateSwapChain(mpDevice, &swapStatus, &mpSwapChain)))
	{
		FatalError::GetInstance().Outbreak(FatalError::smDeviceCreationErrorMS);
		return false;
	}

	return true;
}


//�u�����h�X�e�[�g�̍쐬
bool Graphic::CreateBlendState()
{
	//�u�����h�X�e�[�g�̐ݒ�
	D3D11_BLEND_DESC blendState;
	ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
	blendState.AlphaToCoverageEnable =FALSE;
	blendState.IndependentBlendEnable =FALSE;
	blendState.RenderTarget[0].BlendEnable =TRUE;
	blendState.RenderTarget[0].SrcBlend =D3D11_BLEND_SRC_ALPHA;;
	blendState.RenderTarget[0].DestBlend =D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].BlendOp =D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ONE;
	blendState.RenderTarget[0].DestBlendAlpha =D3D11_BLEND_ZERO;
	blendState.RenderTarget[0].BlendOpAlpha =D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].RenderTargetWriteMask =D3D11_COLOR_WRITE_ENABLE_ALL;

	//�u�����h�X�e�[�g�I�u�W�F�N�g�̍쐬
	if(FAILED(mpDevice->CreateBlendState(&blendState, &mOM.mpBlendState)))
	{
		FatalError::GetInstance().Outbreak(L"�u�����h�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}


//���X�^���C�U�X�e�[�g�̍쐬
bool Graphic::CreateRasterizerState()
{
	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode =D3D11_FILL_SOLID;
	rasterizerState.CullMode =D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise =FALSE;
	rasterizerState.DepthBias =0;
	rasterizerState.DepthBiasClamp =0;
	rasterizerState.SlopeScaledDepthBias =0;
	rasterizerState.DepthClipEnable =TRUE; //�[�x�l�N���b�s���O
	rasterizerState.ScissorEnable =FALSE; //�V�U�[��`�N���b�s���O
	rasterizerState.MultisampleEnable =FALSE; //�}���`�T���v�����O
	rasterizerState.AntialiasedLineEnable =FALSE; //���̃}���`�T���v�����O

	//�W�����X�^���C�U�X�e�[�g�I�u�W�F�N�g�̍쐬
	if(FAILED(mpDevice->CreateRasterizerState(&rasterizerState, &mRS.mpRasterizerState[0])))
	{
		FatalError::GetInstance().Outbreak(L"���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}
	// �J�����O�Ȃ����X�^���C�U�X�e�[�g�I�u�W�F�N�g
	rasterizerState.CullMode =D3D11_CULL_NONE;
	if(FAILED(mpDevice->CreateRasterizerState(&rasterizerState, &mRS.mpRasterizerState[1])))
	{
		FatalError::GetInstance().Outbreak(L"���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}
	// ���C���[�t���[�����X�^���C�U�X�e�[�g�I�u�W�F�N�g
	rasterizerState.FillMode =D3D11_FILL_WIREFRAME;
	rasterizerState.CullMode =D3D11_CULL_NONE;
	if(FAILED(mpDevice->CreateRasterizerState(&rasterizerState, &mRS.mpRasterizerState[2])))
	{
		FatalError::GetInstance().Outbreak(L"���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}


//�[�x/�X�e���V���X�e�[�g�̍쐬
bool Graphic::CreateDepthStencilState()
{
	//�[�x/�X�e���V���X�e�[�g�̐ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilState;
	depthStencilState.DepthEnable =TRUE;
	depthStencilState.DepthWriteMask =D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilState.DepthFunc =D3D11_COMPARISON_LESS;
	depthStencilState.StencilEnable =FALSE;
	depthStencilState.StencilReadMask =0;
	depthStencilState.StencilWriteMask =0;
		//�ʂ��\�������Ă���Ƃ��̃X�e���V���e�X�g�̐ݒ�
	depthStencilState.FrontFace.StencilFailOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.FrontFace.StencilDepthFailOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.FrontFace.StencilPassOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.FrontFace.StencilFunc =D3D11_COMPARISON_NEVER;
		//�ʂ����������Ă���Ƃ��̃X�e���V���e�X�g�̐ݒ�
	depthStencilState.BackFace.StencilFailOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.BackFace.StencilDepthFailOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.BackFace.StencilPassOp =D3D11_STENCIL_OP_KEEP;
	depthStencilState.BackFace.StencilFunc =D3D11_COMPARISON_ALWAYS;

	//�[�x/�X�e���V���X�e�[�g�I�u�W�F�N�g�̍쐬
	if(FAILED(mpDevice->CreateDepthStencilState(&depthStencilState, &mOM.mpDepthStencilState)))
	{
		FatalError::GetInstance().Outbreak(L"�[�x/�X�e���V���X�e�[�g�̍쐬�Ɏ��s���܂���\n�v���O�������I�����܂�");
		return false;
	}

	return true;
}


//�`��p�C�v���C���̏�����
void Graphic::InitRenderingPipeline()
{
	// IA�Ƀv���~�e�B�u���O�p�`���X�g�ŃZ�b�g
	mpImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// RS�Ƀ��X�^���C�U�X�e�[�g�I�u�W�F�N�g��ݒ�
	mpImmediateContext->RSSetState(mRS.mpRasterizerState[0]);
}


//�f�o�C�X�̏�������
void Graphic::ChackDeviceLost()
{
	if(mpDevice->GetDeviceRemovedReason() != S_OK)
	{
		FatalError::GetInstance().Outbreak(L"DirectX�f�o�C�X�����X�g���܂���\n�������p���ł��܂���");
	}
}


//�o�b�N�o�b�t�@�̏�����
bool Graphic::ResetBackBuffer()
{
	//�X���b�v�`�F�C������o�b�N�o�b�t�@���擾����
	ID3D11Texture2D* pBackBuffer =0;
	if(FAILED(mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
	{
		return false;
	}

	//�擾�����o�b�N�o�b�t�@�̏���ۑ�
	D3D11_TEXTURE2D_DESC backBufferStatus;
	pBackBuffer->GetDesc(&backBufferStatus);

	//�X���b�v�`�F�C������擾�����o�b�N�o�b�t�@�����ɕ`��^�[�Q�b�g�r���[�����
	if(FAILED(mpDevice->CreateRenderTargetView(pBackBuffer, NULL, &mOM.mpRenderTargetView)))
	{
		SAFE_RELEASE(pBackBuffer);
		return false;
	}
	//�ȍ~�o�b�N�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
	SAFE_RELEASE(pBackBuffer);


	//�[�x/�X�e���V���e�N�X�`���̐ݒ�
	D3D11_TEXTURE2D_DESC depthStencilStatus =backBufferStatus;
//	depthStencilStatus.Width =backBufferStatus.Width;
//	depthStencilStatus.Height =backBufferStatus.Height;
	depthStencilStatus.MipLevels =1;
	depthStencilStatus.ArraySize =1;
	depthStencilStatus.Format =mFormat[1];
	depthStencilStatus.SampleDesc =backBufferStatus.SampleDesc;
	depthStencilStatus.Usage =D3D11_USAGE_DEFAULT;
	depthStencilStatus.BindFlags =D3D11_BIND_DEPTH_STENCIL;
	depthStencilStatus.CPUAccessFlags =NULL;
	depthStencilStatus.MiscFlags =NULL;
	//�[�x/�X�e���V���e�N�X�`���̍쐬
	ID3D11Texture2D* pDepthStencil =0;
	if(FAILED(mpDevice->CreateTexture2D(&depthStencilStatus, NULL, &pDepthStencil)))
	{
		SAFE_RELEASE(pDepthStencil);
		return false;
	}

	//�[�x/�X�e���V���r���[�̐ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewStatus;
	depthStencilViewStatus.Format =depthStencilStatus.Format;
	depthStencilViewStatus.ViewDimension =D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewStatus.Flags =0;
	depthStencilViewStatus.Texture2D.MipSlice =0;
	HRESULT hr;
	//�[�x/�X�e���V���r���[�̍쐬
	if(FAILED(hr =mpDevice->CreateDepthStencilView(pDepthStencil, NULL, &mOM.mpDepthStencilView)))
	{
		SAFE_RELEASE(pDepthStencil);
		return false;
	}
	SAFE_RELEASE(pDepthStencil);

	//�`��p�C�v���C���̕���
	RenderingPipelineRestoration();

	return true;
}


//��ʃT�C�Y�̕ύX�ɔ����`��p�C�v���C���̕���
void Graphic::RenderingPipelineRestoration()
{
	//OM�ɕ`��^�[�Q�b�g�r���[�Ɛ[�x/�X�e���V���r���[��ݒ�
	mpImmediateContext->OMSetRenderTargets(1, &mOM.mpRenderTargetView, mOM.mpDepthStencilView);
	//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
	const FLOAT blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	mpImmediateContext->OMSetBlendState(mOM.mpBlendState, blendFactor, 0xffffffff);
	//OM�ɐ[�x/�X�e���V���X�e�[�g�I�u�W�F�N�g��ݒ�
	mpImmediateContext->OMSetDepthStencilState(mOM.mpDepthStencilState, 0);
}




//Windows�R�[���o�b�N�֐��p�̕`��̈�T�C�Y�ύX�֐�
void Graphic::InnerResetWindowSize()
{
	//�`��^�[�Q�b�g�̃����[�X
	mpImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	SAFE_RELEASE(mOM.mpRenderTargetView);

	//�X���b�v�`�F�C����ݒ�ς݂̃o�b�t�@�ɕύX����
	mpSwapChain->ResizeBuffers(1, 0, 0, mFormat[0], DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
}


//�t���X�N���[�����[�h�𐳂�����Ԃɖ߂�
void Graphic::InnerWindowModeStateRestoration()
{
	mpSwapChain->SetFullscreenState(mWindows.mIsWindowMode ? FALSE:TRUE, NULL);
	MessageBox(mWindows.mMainWindow, L"Alt+Enter�ɂ���ʃ��[�h�ؑւɂ͑Ή����Ă��܂���B", L"��Ή��̓���", MB_OK);
}


// ���_�V�F�[�_�̐؂�ւ�
void Graphic::VSSetShader(const IAAndVS& shader)
{
	CWE_ASSERT(shader.mpInputLayout&&shader.mpVertexShader);
	mpImmediateContext->IASetInputLayout(shader.mpInputLayout);
	mpImmediateContext->VSSetShader(shader.mpVertexShader, nullptr, 0);
}


// �W�I���g���V�F�[�_�̐؂�ւ�
void Graphic::GSSetShader(const GS& shader)
{
	CWE_ASSERT(shader.mpGeometryShader);
	mpImmediateContext->GSSetShader(shader.mpGeometryShader, nullptr, 0);
}


// �s�N�Z���V�F�[�_�̐؂�ւ�
void Graphic::PSSetShader(const PS& shader)
{
	CWE_ASSERT(shader.mpPixelShader);
	mpImmediateContext->PSSetShader(shader.mpPixelShader, nullptr, 0);
}


// �e�V�F�[�_�̃����[�u
void Graphic::VSRemoveShader()
{
	mpImmediateContext->IASetInputLayout(nullptr);
	mpImmediateContext->VSSetShader(nullptr, nullptr, 0);
}

void Graphic::GSRemoveShader()
{
	mpImmediateContext->GSSetShader(nullptr, nullptr, 0);
}

void Graphic::PSRemoveShader()
{
	mpImmediateContext->PSSetShader(nullptr, nullptr, 0);
}


//�E�B���h�E���[�h�̃��Z�b�g
void Graphic::ResetWindowMode(bool windowMode)
{
	BOOL currentWindowMode;

	mpSwapChain->GetFullscreenState(&currentWindowMode, NULL);
	if(windowMode == (currentWindowMode?false:true)) {return;}

	mpSwapChain->SetFullscreenState(!currentWindowMode, NULL);

	//�R�A�ɕύX��ۑ�
	mWindows.mIsWindowMode =(currentWindowMode?true:false);
}


//�f�B�X�v���C���[�h�̃��Z�b�g
bool Graphic::ResetDisplayMode(unsigned width, unsigned height/*, const unsigned& refreshRate*/)
{
	//�t���X�N���[�����[�h���͕ύX���󂯕t���Ȃ�
	if(!mWindows.mIsWindowMode)
	{
		return false;
	}

	//�ύX���e���Z�b�g
	DXGI_MODE_DESC modeState;

	modeState.Width =width;
	modeState.Height =height;
	modeState.RefreshRate.Numerator =60;
	modeState.RefreshRate.Denominator =1;
	modeState.Format =mFormat[0];
	modeState.ScanlineOrdering =DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeState.Scaling =DXGI_MODE_SCALING_CENTERED;

	//�R�A�̑O��Ԃ�ۑ�
	int beforeWidth =mWindows.mWindowSizeX,
		beforeHeight =mWindows.mWindowSizeY;
	double beforeExRateX =mWindows.mWindowSizeExRateX,
		   beforeExRateY =mWindows.mWindowSizeExRateY;
	//�R�A�ɕύX��ۑ�
	mWindows.mWindowSizeX =width;
	mWindows.mWindowSizeY =height;
	mWindows.mWindowSizeExRateX =mWindows.mWindowSizeX/mWindows.mInitialWindowSizeX;
	mWindows.mWindowSizeExRateY =mWindows.mWindowSizeY/mWindows.mInitialWindowSizeY;

	//�X���b�v�`�F�C���ɕύX���e��ۑ�
	if(FAILED(mpSwapChain->ResizeTarget(&modeState)))
	{
		//���s���ɃR�A�̕ύX��O��Ԃɖ߂�
		mWindows.mWindowSizeX =beforeHeight;
		mWindows.mWindowSizeY =beforeWidth;
		mWindows.mWindowSizeExRateX =beforeExRateX;
		mWindows.mWindowSizeExRateY =beforeExRateY;
		return false;
	}
	
	return true;
}


//Set the sync interval.
void Graphic::SetSyncInterval(unsigned inter)
{
	mSyncInterval =inter;
}


// �g�p���郉�X�^���C�U�X�e�[�g�̃��Z�b�g
void Graphic::RSResetUseState(const RSState& useState)
{
	switch(useState)
	{
	case RSState::Normal:
		mpImmediateContext->RSSetState(mRS.mpRasterizerState[0]);
		break;

	case RSState::NonCull:
		mpImmediateContext->RSSetState(mRS.mpRasterizerState[1]);
		break;

	case RSState::WireFrame:
		mpImmediateContext->RSSetState(mRS.mpRasterizerState[2]);
		break;
	}
}


// �T���v���[�X�e�[�g�̃Z�b�g
void Graphic::VSSetSampler(unsigned slot, const SamplerState& samplerState)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	mpImmediateContext->VSSetSamplers(slot, 1, &samplerState.mpSamplerState);
}
void Graphic::GSSetSampler(unsigned slot, const SamplerState& samplerState)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	mpImmediateContext->GSSetSamplers(slot, 1, &samplerState.mpSamplerState);
}
void Graphic::PSSetSampler(unsigned slot, const SamplerState& samplerState)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);
	mpImmediateContext->PSSetSamplers(slot, 1, &samplerState.mpSamplerState);
}


// �R���X�^���g�o�b�t�@�̃Z�b�g
void Graphic::VSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	mpImmediateContext->VSSetConstantBuffers(slot, 1, &constantBuffer.mpConstantBuffer);
}
void Graphic::GSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	mpImmediateContext->GSSetConstantBuffers(slot, 1, &constantBuffer.mpConstantBuffer);
}
void Graphic::PSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer)
{
	CWE_ASSERT(slot<D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	mpImmediateContext->PSSetConstantBuffers(slot, 1, &constantBuffer.mpConstantBuffer);
}


// �[�x�o�b�t�@�̗L�����E������
void Graphic::OMSetDepthBuffer(bool enable)
{
	mpImmediateContext->OMSetRenderTargets(1, &mOM.mpRenderTargetView, enable?mOM.mpDepthStencilView:nullptr);
}


// �`��^�[�Q�b�g���Z�b�g
void Graphic::OMSetRenderTarget(const RenderTargetView& renderTargetView)
{
	mpImmediateContext->OMSetRenderTargets(1, &renderTargetView.mpRenderTargetView, mOM.mpDepthStencilView);
}
void Graphic::OMSetRenderTarget(const DepthStencilView& depthStencilView)
{
	mpImmediateContext->OMSetRenderTargets(1, nullptr, depthStencilView.mpDepthStencilView);
}
void Graphic::OMSetRenderTarget(const RenderTargetView& renderTargetView, const DepthStencilView& depthStencilView)
{
	mpImmediateContext->OMSetRenderTargets(1, &renderTargetView.mpRenderTargetView, depthStencilView.mpDepthStencilView);
}

// �`��^�[�Q�b�g���f�t�H���g�̂��̂ɃZ�b�g
void Graphic::OMSetRenderTargetDefalut()
{
	mpImmediateContext->OMSetRenderTargets(1, &mOM.mpRenderTargetView, mOM.mpDepthStencilView);
}


// IA�ɒ��_�o�b�t�@��ݒ�
void Graphic::IASetVertexBuffers(unsigned startSlot, unsigned numBuffers, ID3D11Buffer *const *ppVertexBuffers, const unsigned* pStrides, const unsigned *pOffsets)
{
	mpImmediateContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets);
}


// IA�ɃC���f�b�N�X�o�b�t�@��ݒ�
void Graphic::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, unsigned offset)
{
	mpImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
}


// �V�F�[�_���\�[�X�r���[���s�N�Z���V�F�[�_�ɃZ�b�g
void Graphic::PSSetShaderResources(unsigned startSlot, unsigned numViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
	mpImmediateContext->PSSetShaderResources(startSlot, numViews, ppShaderResourceViews);
}


// �C���f�b�N�X�t���`��
void Graphic::DrawIndexed(unsigned indexCount, unsigned startIndexLocation, int baseVertexLocation)
{
	mpImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}



}//CWE