

// �萔�o�b�t�@
cbuffer g_cb : register(b0)
{
	matrix World;
	float Param;
	float Alpha;
	float2 dammy;
}

// �e�N�X�`��
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);


// �s�N�Z���V�F�[�_�̓��̓f�[�^
struct PSInput
{
	float4 mPos : SV_POSITION; //���_���W
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
};


// �s�N�Z���V�F�[�_�̏o�̓f�[�^
struct PSOutput
{
	float4 mColor : SV_TARGET;
};


// �s�N�Z���V�F�[�_�֐�
PSOutput UIPS(PSInput input)
{
	PSOutput op;
	op.mColor =Texture.Sample(Sampler, input.mUV);
	op.mColor.a =op.mColor.a *Alpha;
	return op;
}