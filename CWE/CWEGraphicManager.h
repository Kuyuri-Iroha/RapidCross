#pragma once

#include "CWEIncludedAPI.h"
#include "CWEMath.h"
#include "CWEMacroConstants.h"
#include "CWEUsefullyMacro.h"
#include "CWEWindows.h"
#include "CWEError.h"
#include <string>


namespace CWE
{
// �O���錾
class Graphic;

namespace callbacks//�g�p�֎~
{
LRESULT CALLBACK CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}


//�O���錾
class Viewport;
class Resource;



//�O���t�B�b�N�̕i��
enum class Quality
{
	Low =1,
	Middle =0,
	High =2,
	Maximum =3
};


// ���̓G�������g�p�̃f�[�^����
enum class InputClassification
{
	VertexData =0,
	InstanceData =1
};

// �g�p���郉�X�^���C�U�X�e�[�g�I��p
enum class RSState
{
	Normal =0,
	NonCull =1,
	WireFrame =2
};


// �g�p����T���v���X�e�[�g�̑I��p
enum class SamplerStatus
{
	Liner =0, //���`���
	Point =1, //��ԂȂ�
};





// �t�H�[�}�b�g
typedef DXGI_FORMAT Format;

// �O���錾
struct IAAndVS;
struct GS;
struct PS;

// �V�F�[�_�}�N����`
struct ShaderMacro
{
private:
	friend struct IAAndVS;
	friend struct GS;
	friend struct PS;
	D3D_SHADER_MACRO mShaderMacro;

public:
	ShaderMacro(): mShaderMacro() {}
	~ShaderMacro() {}
	ShaderMacro(const char* name, const char* definition);

	void Set(const char* name, const char* definition);
};


// ���̓G�������g
struct IAInputElement
{
private:
	friend struct IAAndVS;
	D3D11_INPUT_ELEMENT_DESC mIE;

public:
	IAInputElement():mIE() {}
	~IAInputElement() {}
	IAInputElement(const char* semanticName, unsigned semanticIndex, const Format& format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);

	void Set(const char* semanticName, unsigned semanticIndex, const Format& format, unsigned inputSlot, unsigned alignedByteOffset, InputClassification inputSlotClass, unsigned instanceDataStepRate);
};


// ���̓��C�A�E�g&���_�V�F�[�_�I�u�W�F�N�g
struct IAAndVS
{
private:
	friend class CWE::Graphic;
	ID3D11InputLayout* mpInputLayout;
	ID3D11VertexShader* mpVertexShader;

public:
	IAAndVS(): mpInputLayout(nullptr), mpVertexShader(nullptr) {}
	~IAAndVS() {SAFE_RELEASE(mpInputLayout); SAFE_RELEASE(mpVertexShader);}

	// ���̓��C�A�E�g&���_�V�F�[�_�I�u�W�F�N�g�̍쐬
	// (shaderMacroNum�̎w����~�X����ƕs���A�N�Z�X��O����������̂Œ���)
	bool Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName, const IAInputElement* pInputElements, unsigned numElements);

};


// �W�I���g���V�F�[�_�I�u�W�F�N�g
struct GS
{
private:
	friend class CWE::Graphic;
	ID3D11GeometryShader* mpGeometryShader;

public:
	GS(): mpGeometryShader(nullptr) {}
	~GS() {SAFE_RELEASE(mpGeometryShader);}

	// �W�I���g���V�F�[�_�I�u�W�F�N�g�̍쐬
	bool Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName);

};


// �s�N�Z���V�F�[�_�I�u�W�F�N�g
struct PS
{
private:
	friend class CWE::Graphic;
	ID3D11PixelShader* mpPixelShader;

public:
	PS(): mpPixelShader(nullptr) {}
	~PS() {SAFE_RELEASE(mpPixelShader);}

	// �s�N�Z���V�F�[�_�I�u�W�F�N�g�̍쐬
	bool Create(const wchar_t* shaderFile, const ShaderMacro* pShaderMacros, const char* functionName);

};



// �T���v���[�X�e�[�g�I�u�W�F�N�g
struct SamplerState
{
private:
	friend class CWE::Graphic;
	ID3D11SamplerState* mpSamplerState;

public:
	SamplerState(): mpSamplerState(nullptr) {}
	~SamplerState() {SAFE_RELEASE(mpSamplerState);}

	// �v���Z�b�g����쐬
	bool Create(const SamplerStatus& preset);
	// �V���h�E�}�b�v�p�̍쐬
	bool CreateForShadow();
};


// �R���X�^���g�o�b�t�@�I�u�W�F�N�g
struct ConstantBuffer
{
private:
	friend class CWE::Graphic;
	ID3D11Buffer* mpConstantBuffer;
	D3D11_MAPPED_SUBRESOURCE mResource;
	bool mMapped;

public:
	ConstantBuffer(): mpConstantBuffer(nullptr) {}
	~ConstantBuffer() {SAFE_RELEASE(mpConstantBuffer);}

	// �R���X�^���g�o�b�t�@�̍쐬
	bool Create(size_t bufferSize);

	// �������݊J�n
	bool Map();
	//�������ݏI��
	void Unmap();
	//�T�u���\�[�X�ւ̏�������
	void* const Access();

};


// �����_�[�^�[�Q�b�g�r���[�I�u�W�F�N�g
struct RenderTargetView
{
private:
	friend class CWE::Graphic;
	ID3D11RenderTargetView* mpRenderTargetView;

public:
	RenderTargetView(): mpRenderTargetView(nullptr) {}
	~RenderTargetView() {SAFE_RELEASE(mpRenderTargetView);}

	// �쐬
	template<typename T_Resource>
	bool Create(T_Resource& resource)
	{
		if(FAILED( Graphic::GetInstance().Device().CreateRenderTargetView(resource.GetResource(), nullptr, &mpRenderTargetView) ))
		{
			return false;
		}

		return true;
	}

	// �N���A
	void Clear();
};



// �[�x/�X�e���V���r���[�L�q�I�u�W�F�N�g
typedef D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;


// �[�x/�X�e���V���r���[�I�u�W�F�N�g
struct DepthStencilView
{
private:
	friend class CWE::Graphic;
	ID3D11DepthStencilView* mpDepthStencilView;

public:
	DepthStencilView(): mpDepthStencilView(nullptr) {}
	~DepthStencilView() {SAFE_RELEASE(mpDepthStencilView);}

	// �쐬
	template<typename T_Resource>
	bool Create(T_Resource& resource, const DepthStencilViewDesc& desc)
	{
		if(FAILED( Graphic::GetInstance().Device().CreateDepthStencilView(resource.GetResource(), &desc, &mpDepthStencilView) ))
		{
			return false;
		}

		return true;
	}
	// �N���A
	void Clear();

};


// �V�F�[�_���\�[�X�r���[
struct ShaderResourceView
{
private:
	ID3D11ShaderResourceView* mpShaderResourceView;

public:
	ShaderResourceView(): mpShaderResourceView(nullptr) {}
	~ShaderResourceView() {}

	//�쐬
	template<typename T_Resource>
	bool Create(T_Resource& resource)
	{
		if(FAILED( Graphic::GetInstance().Device().CreateShaderResourceView(resource.GetResource(), nullptr, &mpShaderResourceView) ))
		{
			return false;
		}
		return true;
	}
};


// 2D�e�N�X�`���L�q�I�u�W�F�N�g
typedef D3D11_TEXTURE2D_DESC Texture2DDesc;


// 2D�e�N�X�`���I�u�W�F�N�g
struct Texture2D
{
private:
	friend struct RenderTargetView;
	friend struct DepthStencilView;
	friend struct ShaderResourceView;
	ID3D11Texture2D* mpTexture;
	inline ID3D11Texture2D* GetResource() {return mpTexture;}

public:
	Texture2D(): mpTexture(nullptr) {}
	~Texture2D() {SAFE_RELEASE(mpTexture);}

	// �e�N�X�`���̍쐬
	bool Create(const Texture2DDesc& desc);
	inline const ID3D11Texture2D* const GetResource() const {return mpTexture;}
};





//�`��n�Ǘ��N���X
class Graphic
{
private:
	//�t�����h
	friend bool Init(unsigned width, unsigned height, bool windowMode, const wchar_t windowTitle[128]);
	friend bool Update();
	friend void End();
	friend LRESULT CALLBACK CWE::callbacks::CWE_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	friend bool GraphicInitialized();
	friend class CWE::Viewport;
	friend class CWE::Resource;
	friend struct CWE::IAAndVS;
	friend struct CWE::GS;
	friend struct CWE::PS;
	friend struct CWE::SamplerState;
	friend struct CWE::ConstantBuffer;



	//�r���[�|�[�g�������p�f�t�H���g�l
	const int NearZ =1,
			  FarZ  =1000;



	//�C���^�[�t�F�C�X
	IDXGIFactory* mpFactory; //DXGI
	ID3D11Device* mpDevice; //�f�o�C�X
	ID3D11DeviceContext* mpImmediateContext; //�f�o�C�X�E�R���e�L�X�g
	IDXGISwapChain* mpSwapChain; //�X���b�v�E�`�F�C��

	//======= �`��p�C�v���C�� ==========

	// ���X�^���C�U
	struct Rasterizer
	{
		//���X�^���C�U�X�e�[�g
		ID3D11RasterizerState* mpRasterizerState[3];
	} mRS;

	// �o�̓}�[�W���[
	struct OutputMerger
	{
		//�u�����h�X�e�[�g
		ID3D11BlendState* mpBlendState;
		//�[�x/�X�e���V���X�e�[�g
		ID3D11DepthStencilState* mpDepthStencilState;
		//�[�x/�X�e���V�� �r���[
		ID3D11DepthStencilView* mpDepthStencilView;
		//�ʏ�X�N���[���p�`��^�[�Q�b�g�r���[
		ID3D11RenderTargetView* mpRenderTargetView;
	}mOM;
	//�`�掞�̃t�H�[�}�b�g
	DXGI_FORMAT mFormat[2];
	DXGI_SAMPLE_DESC mMSAAQuality;
	unsigned mSyncInterval;


	//������
	bool Init();
	//�I������
	void End();
	//�X�V����
	void Update();

	//�f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	bool CreateDeviceAndSwapChain();
	//�u�����h�X�e�[�g�̍쐬
	bool CreateBlendState();
	//���X�^���C�U�X�e�[�g�̍쐬
	bool CreateRasterizerState();
	//�[�x/�X�e���V���X�e�[�g�̍쐬
	bool CreateDepthStencilState();
	//�`��p�C�v���C���̏�����
	void InitRenderingPipeline();

	//�f�o�C�X�̏�������
	void ChackDeviceLost();

	//�o�b�N�o�b�t�@�[�̃��Z�b�g
	//true=���� false=���s
	bool ResetBackBuffer();

	//��ʃT�C�Y�̕ύX�ɔ����`��p�C�v���C���̕���
	void RenderingPipelineRestoration();

	//Windows�R�[���o�b�N�֐��p�̕`��̈�ύX�֐�
	void InnerResetWindowSize();
	//�t���X�N���[�����[�h�𐳂�����Ԃɖ߂�
	void InnerWindowModeStateRestoration();


	Graphic();
	~Graphic();
	Graphic(const Graphic&) =delete;
	Graphic& operator=(const Graphic&) =delete;

public:
	//�V���O���g���̎���
	static Graphic& GetInstance();

	// Windows���
	Windows mWindows;

	// �f�o�C�X���L�����ǂ���
	inline bool CheckDeviceIs() {return mpDevice?true:false;}
	// �O������̃f�o�C�X�̎g�p
	inline ID3D11Device& Device() {CWE_ASSERT(CheckDeviceIs()); return *mpDevice;}
	// �O������̃f�o�C�X�R���e�L�X�g�̎g�p
	inline ID3D11DeviceContext& DeviceContext() {CWE_ASSERT(CheckDeviceIs()); return *mpImmediateContext;}

	//�e�V�F�[�_�̃t�@�C���p�X�̃Z�b�g
	void VSSetShader(const IAAndVS& shader);
	void GSSetShader(const GS& shader);
	void PSSetShader(const PS& shader);

	// �e�V�F�[�_�̃����[�u
	void VSRemoveShader();
	void GSRemoveShader();
	void PSRemoveShader();

	//�E�B���h�E���[�h�̃��Z�b�g
	//true=�E�B���h�E���[�h false=�t���X�N���[�����[�h
	void ResetWindowMode(bool windowMode);

	//�f�B�X�v���C���[�h�̃��Z�b�g
	//�Ԃ�l: true=���� false=���s
	bool ResetDisplayMode(unsigned width, unsigned height/*, const unsigned& refreshRate*/);

	//Set the sync interval.
	//inter : FPS = Refresh rate / inter
	void SetSyncInterval(unsigned inter);

	// �g�p���郉�X�^���C�U�X�e�[�g�̃��Z�b�g
	void RSResetUseState(const RSState& useState);

	// �T���v���[�X�e�[�g�̃Z�b�g
	void VSSetSampler(unsigned slot, const SamplerState& samplerState);
	void GSSetSampler(unsigned slot, const SamplerState& samplerState);
	void PSSetSampler(unsigned slot, const SamplerState& samplerState);

	// �萔�o�b�t�@�̃Z�b�g
	void VSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer);
	void GSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer);
	void PSSetConstantBuffer(unsigned slot, const ConstantBuffer& constantBuffer);

	// �[�x�o�b�t�@�̗L�����E������
	void OMSetDepthBuffer(bool enable);
	// �`��^�[�Q�b�g���Z�b�g
	void OMSetRenderTarget(const RenderTargetView& renderTargetView);
	void OMSetRenderTarget(const DepthStencilView& depthStencilView);
	void OMSetRenderTarget(const RenderTargetView& renderTargetView, const DepthStencilView& depthStencilView);
	// �`��^�[�Q�b�g���f�t�H���g�̂��̂ɃZ�b�g
	void OMSetRenderTargetDefalut();

	// IA�ɒ��_�o�b�t�@��ݒ�
	void IASetVertexBuffers(unsigned startSlot, unsigned numBuffers, ID3D11Buffer *const *ppVertexBuffers, const unsigned* pStrides, const unsigned *pOffsets);
	// IA�ɃC���f�b�N�X�o�b�t�@��ݒ�
	void IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, unsigned offset);

	// �V�F�[�_���\�[�X�r���[���s�N�Z���V�F�[�_�ɃZ�b�g
	void PSSetShaderResources(unsigned startSlot, unsigned numViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);

	// �C���f�b�N�X�t���`��
	void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, int baseVertexLocation);
};



}//CWE

