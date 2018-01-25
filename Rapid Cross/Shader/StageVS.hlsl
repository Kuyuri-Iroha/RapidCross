


// �萔�o�b�t�@
cbuffer g_cb : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 EyePos;

	float Ambient;
	float3 LDir;
	float4 LColor;
	float LStrength;
	float Specular;
	float2 Dammy;
}

cbuffer g_shadow : register(b1)
{
	matrix SView;
	matrix SProjection;
}


struct VSInput
{
	float3 mPos : POSITION; //���_���W(���f�����W�n)
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
	float3 mNorm : NORMAL; //�@���x�N�g��
};


struct VSOutput
{
	float4 mPos : SV_POSITION; //���_���W(�����ϊ����W)
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
	float3 mNorm : TEXCOORD1; //�@���x�N�g��
	float3 mViewDir : TEXCOORD2; //�����x�N�g��
	float3 mShadowCoord : TEXCOORD3; //�V���h�E�}�b�v���W
};


// �V���h�E�}�b�v�`��p���_�V�F�[�_�֐�
float4 StageShadowVS(VSInput ip) : SV_POSITION
{
	// �X�L�j���O
	float4 pos =float4(ip.mPos, 1.0f);

	// ���_���W�ϊ�
	pos =mul(pos, World);
	pos =mul(pos, View);
	pos =mul(pos, Projection);

	return pos;
}


// ���_�V�F�[�_�֐�
VSOutput StageVS(VSInput ip)
{
	VSOutput op;

	// �X�L�j���O
	float4 pos =float4(ip.mPos, 1.0f);

	// ���_���W�ϊ�
	op.mPos =mul(pos, World);
	op.mViewDir =op.mPos.xyz -EyePos.xyz; //���_�x�N�g���̍쐬
    op.mPos =mul(op.mPos, View);
	op.mPos =mul(op.mPos, Projection);

	// �e�N�X�`�����W�̎󂯓n��
	op.mUV =ip.mUV;

	// �@���x�N�g���ϊ�
	op.mNorm =ip.mNorm;

	// �V���h�E�}�b�s���O�p�̎Z�o
	pos =mul(pos, World);
	pos =mul(pos, SView);
	pos =mul(pos, SProjection);
	pos.xyz =pos.xyz /pos.w;
	op.mShadowCoord.x =(pos.x +1.0f) /2.0f;
	op.mShadowCoord.y =(-pos.y +1.0f) /2.0f;
	op.mShadowCoord.z =pos.z;

	return op;
}