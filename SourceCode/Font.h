#pragma once

#include "../CWE/CWELib.h"
#include <unordered_map>



// �t�H���g�쐬�N���X
class Font
{
public:
	// �����Z�b�g
	enum class Charset : unsigned
	{
		ANSI =ANSI_CHARSET,
		ShiftJIS =SHIFTJIS_CHARSET
	};
	// �A���`�G�C���A�V���O�̃N�I���e�B
	enum class Quality : unsigned
	{
		Low =GGO_GRAY2_BITMAP,
		Midium =GGO_GRAY4_BITMAP,
		High =GGO_GRAY8_BITMAP
	};

	// �����e�N�X�`���f�[�^
	struct TextureData
	{
		ID3D11ShaderResourceView* mpSRV;
		float mWidth,
			  mHeight;
		TextureData() : mpSRV(nullptr), mWidth(0.0f), mHeight(0.0f) {}
		TextureData(const TextureData& texData) : mpSRV(texData.mpSRV), mWidth(texData.mWidth), mHeight(texData.mHeight) {}
		~TextureData() {SAFE_RELEASE(mpSRV);}
		TextureData& operator=(const TextureData& texData) {mpSRV=texData.mpSRV; mWidth=texData.mWidth; mHeight=texData.mHeight;}
	};

private:
	std::unordered_map<wchar_t, TextureData> mTexture;
	bool mEnabled;
	CWE::Task mStatus;

	bool CreateTextureChar(wchar_t aChar, TextureData& data, HDC& hdc, Quality quality, CWE::Graphic& manager);
	bool CreateTextureANSI(HDC& hdc, Quality quality);
	bool CreateTextureShiftJIS();

public:
	Font();
	~Font();

	// �t�H���g�̍쐬
	bool Create(int size, int weight, Quality quality, bool italic, bool underline, bool strikeOut, Charset charset, const wchar_t* name);
	// �t�H���g�̍쐬(�񓯊������p)
	static void Create(Font& thisData, int size, int weight, Quality quality, bool italic, bool underline, bool strikeOut, Charset charset, const wchar_t* name);
	// �e�N�X�`���f�[�^�̎擾
	const TextureData* const Get(wchar_t wChar) const;
	// �g�p�\�����擾
	inline bool Enabled() const {return mEnabled;}
	// ��Ԃ��擾
	inline const CWE::Task& Status() const {return mStatus;}
};