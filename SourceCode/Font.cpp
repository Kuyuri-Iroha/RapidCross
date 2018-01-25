#include "Font.h"




// �R���X�g���N�^
Font::Font():
	mEnabled(false), mStatus(CWE::Task::Working)
{

}

// �f�X�g���N�^
Font::~Font()
{

}


// �ꕶ���P�ʂ̃e�N�X�`���쐬
bool Font::CreateTextureChar(wchar_t aChar, TextureData& data, HDC& hdc, Quality quality, CWE::Graphic& manager)
{
	unsigned charCode =0;
	TEXTMETRIC tm;
	GLYPHMETRICS gm;
	const MAT2 m ={
		{0, 1}, {0, 0}, {0, 0}, {0, 1}
	};
	ID3D11Texture2D* pTexture =nullptr;

	//�t�H���g�r�b�g�}�b�v�쐬
	charCode =static_cast<unsigned>(aChar);
	GetTextMetricsW(hdc, &tm);
	unsigned memSize =GetGlyphOutlineW(hdc, charCode, static_cast<unsigned>(quality), &gm, NULL, NULL, &m);
	unsigned char* pBitmap =new unsigned char[memSize];
	GetGlyphOutlineW(hdc, charCode, static_cast<unsigned>(quality), &gm, memSize, pBitmap, &m);

	//�e�N�X�`���̍쐬
	CD3D11_TEXTURE2D_DESC desc(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
		gm.gmCellIncX, tm.tmHeight, 1, 1, 
		D3D11_BIND_SHADER_RESOURCE, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_CPU_ACCESS_WRITE
		);
	if(FAILED(manager.Device().CreateTexture2D(&desc, 0, &pTexture)))
	{
		return false;
	}

	data.mWidth =float(desc.Width);
	data.mHeight =float(desc.Height);

	//�e�N�X�`�����}�b�v
	D3D11_MAPPED_SUBRESOURCE mapped;
	if(FAILED(manager.DeviceContext().Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	unsigned char* pBits =(unsigned char*)mapped.pData;

	//�r�b�g�}�b�v���e�N�X�`���ɏ�������
	int offsetX =gm.gmptGlyphOrigin.x,
		offsetY =tm.tmAscent -gm.gmptGlyphOrigin.y,
		width =gm.gmBlackBoxX +(4 -(gm.gmBlackBoxX %4)) %4,
		height =gm.gmBlackBoxY;
	int	level =0;
	int x=0, y=0;
	unsigned alpha=0, color=0;

	switch(quality)
	{
	case Quality::Low:
		level =5;
		break;

	case Quality::Midium:
		level =17;
		break;

	case Quality::High:
		level =65;
		break;

	default:
		CWE_ASSERT(false);
		break;
	}
	memset(pBits, 0, mapped.RowPitch*tm.tmHeight);

	for(y=offsetY; y<offsetY+height; ++y)
	{
		for(x=offsetX; x<offsetX+width; ++x)
		{
			alpha =(255 *pBitmap[x -offsetX +width *(y -offsetY)]) /(level -1);
			color =0x00ffffff | (alpha <<24);
			CopyMemory((unsigned char*)pBits +mapped.RowPitch *y +4 *x, &color, sizeof(unsigned));
		}
	}

	// �e�N�X�`�����A���}�b�v
	manager.DeviceContext().Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));

	//�e�N�X�`���ւ̏������ݏI��
	SAFE_DELETE_ARRAY(pBitmap);

	//�����_�[�^�[�Q�b�g�r���[���쐬
	if(FAILED(manager.Device().CreateShaderResourceView(pTexture, NULL, &data.mpSRV)))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	//�e�N�X�`���������
	SAFE_RELEASE(pTexture);

	return true;
}


// ANSI(�p��)�̃t�H���g�e�N�X�`�����쐬
bool Font::CreateTextureANSI(HDC& hdc, Quality quality)
{
	CWE::Graphic& manager =CWE::Graphic::GetInstance();
	TextureData* data;
	wchar_t aChar =L'!';
	for(unsigned i=unsigned(aChar); i<=126; ++i)
	{
		data =&mTexture[aChar];
		if(!CreateTextureChar(aChar, *data, hdc, quality, manager))
		{
			return false;
		}
		++aChar;
	}

	return true;
}


// ShiftJIS(���{��)�̃t�H���g�e�N�X�`�����쐬
bool Font::CreateTextureShiftJIS()
{
	CWE_ASSERT(false); //��Ή�
	return false;
}


// �t�H���g�̍쐬
bool Font::Create(int size, int weight, Quality quality, bool italic, bool underline, bool strikeOut, Charset charset, const wchar_t* name)
{
	if(mEnabled) {return false;} //�쐬�ς݂ł���Ύ��s

	bool result =true;

	HFONT font =CreateFontW(
		size, 0, 0, 0, weight,
		italic ? TRUE : FALSE,
		underline ? TRUE : FALSE,
		strikeOut ? TRUE : FALSE,
		static_cast<unsigned>(charset),
		OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, (FIXED_PITCH|FF_MODERN),
		name
		);

	HDC context =GetDC(NULL);
	HFONT oldFont =HFONT(SelectObject(context, font));

	//�w�肳�ꂽ�����Z�b�g�ɂ��t�H���g�e�N�X�`���̍쐬
	switch(charset)
	{
	case Charset::ANSI:
		result =CreateTextureANSI(context, quality);
		break;

	case Charset::ShiftJIS:
		result =CreateTextureShiftJIS();
		break;

	default:
		CWE_ASSERT(false);
		break;
	}

	//�f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(context, oldFont);
	DeleteObject(font );
	ReleaseDC(NULL, context);

	mEnabled =result;
	mStatus =result ? CWE::Task::Success : CWE::Task::Failed;

	return result;
}

// �t�H���g�̍쐬(�񓯊������p)
void Font::Create(Font& thisData, int size, int weight, Quality quality, bool italic, bool underline, bool strikeOut, Charset charset, const wchar_t* name)
{
	thisData.mStatus =CWE::Task::Working;

	if(thisData.Create(size, weight, quality, italic, underline, strikeOut, charset, name))
	{
		thisData.mStatus =CWE::Task::Success;
	}
	else
	{
		thisData.mStatus =CWE::Task::Failed;
	}
}


// �e�N�X�`���f�[�^�̎擾
const Font::TextureData* const Font::Get(wchar_t aChar) const
{
	if(mTexture.count(aChar) == 0 || !mEnabled)
	{
		return nullptr;
	}

	return &mTexture.at(aChar);
}