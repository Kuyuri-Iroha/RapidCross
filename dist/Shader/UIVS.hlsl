


// �萔�o�b�t�@
cbuffer g_cb : register(b0)
{
	matrix World;
	float Param;
	float3 dammy;
}

cbuffer g_StaticCB : register(b1)
{
	matrix Projection;
};


// ���_�V�F�[�_�̓��̓f�[�^
struct VSInput
{
	float3 mPos : POSITION; //���_���W
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
};


// ���_�V�F�[�_�̏o�̓f�[�^
struct VSOutput
{
	float4 mPos : SV_POSITION; //���_���W
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
};



// ���_�V�F�[�_�֐�
VSOutput UIVS(VSInput input)
{
	VSOutput output;
	// ���_���W
    output.mPos =float4(input.mPos.xy, 0.0f, 1.0f);
    output.mPos =mul(output.mPos, World);
	output.mPos =mul(output.mPos, Projection);

	// �e�N�X�`�����W�̎󂯓n��
	output.mUV =input.mUV;
	
	// �o��
	return output;
}