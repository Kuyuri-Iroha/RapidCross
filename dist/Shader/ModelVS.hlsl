


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

cbuffer g_bone : register(b1)
{
	float4x4 BoneMat[145];
}

cbuffer g_shadow : register(b2)
{
	matrix SView;
	matrix SProjection;
}


struct VSInput
{
	float3 mPos : POSITION; //���_���W(���f�����W�n)
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
	float3 mNorm : NORMAL; //�@���x�N�g��
	uint4 mBoneIndx : BLENDINDICES0;//�{�[���C���f�b�N�X
	float4 mWeight : BLENDWEIGHT0;//�{�[���E�F�C�g
};


struct VSOutput
{
	float4 mPos : SV_POSITION; //���_���W(�����ϊ����W)
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
	float3 mNorm : TEXCOORD1; //�@���x�N�g��
	float3 mViewDir : TEXCOORD2; //�����x�N�g��
	float3 mShadowCoord : TEXCOORD3; //�V���h�E�}�b�v���W
};



// �{�[���X�L�j���O
float4 BoneSkinning(float4 pos, uint4 bidx, float4 bwgt)
{
	float3 bPos =bwgt.x*mul( pos, BoneMat[bidx.x]);
	bPos +=bwgt.y*mul(pos, BoneMat[bidx.y]);
	bPos +=bwgt.z*mul(pos, BoneMat[bidx.z]);
	bPos +=bwgt.w*mul(pos, BoneMat[bidx.w]);
	return float4(bPos, 1.0f);
}


// �{�[���X�L�j���O�@���ϊ�
float3 BoneSkinningNorm(float3 nor, uint4 bidx, float4 bwgt)
{
	float3 bnor =bwgt.x*mul(nor, (float3x3)BoneMat[bidx.x]);
	bnor.xyz +=bwgt.y*mul(nor, (float3x3)BoneMat[bidx.y]);
	bnor.xyz +=bwgt.z*mul(nor, (float3x3)BoneMat[bidx.z]);
	bnor.xyz +=bwgt.w*mul(nor, (float3x3)BoneMat[bidx.w]);
	// �P���ȉ�]���ψ�X�P�[���̏ꍇ�̂ݗL��
	// ���m�Ȗ@���̕ϊ��͋t�]�u�s���
	return normalize(bnor);
}


// �V���h�E�}�b�v�`��p���_�V�F�[�_�֐�
float4 ModelShadowVS(VSInput ip) : SV_POSITION
{
	// �X�L�j���O
	float4 pos =BoneSkinning(float4(ip.mPos, 1.0f), ip.mBoneIndx, ip.mWeight);

	// ���_���W�ϊ�
	pos =mul(pos, World);
    pos =mul(pos, View);
	pos =mul(pos, Projection);

	return pos;
}


// ���_�V�F�[�_�֐�
VSOutput ModelVS(VSInput ip)
{
	VSOutput op;

	// �X�L�j���O
	float4 pos =BoneSkinning(float4(ip.mPos, 1.0f), ip.mBoneIndx, ip.mWeight);

	// ���_���W�ϊ�
	op.mPos =mul(pos, World);
	op.mViewDir =pos.xyz -EyePos.xyz; //���_�x�N�g���̍쐬
    op.mPos =mul(op.mPos, View);
	op.mPos =mul(op.mPos, Projection);

	// �e�N�X�`�����W�̎󂯓n��
	op.mUV =ip.mUV;

	// �@���x�N�g���ϊ�
	float3 norm =BoneSkinningNorm(ip.mNorm, ip.mBoneIndx, ip.mWeight);
	op.mNorm =mul(norm, (float3x3)World);
	op.mNorm =mul(op.mNorm, (float3x3)View);

	// �V���h�E�}�b�s���O�p�x�N�g���̎Z�o
	pos =mul(pos, World);
	pos =mul(pos, SView);
	pos =mul(pos, SProjection);
	pos.xyz =pos.xyz /pos.w;
	op.mShadowCoord.x =(pos.x +1.0f) /2.0f;
	op.mShadowCoord.y =(-pos.y +1.0f) /2.0f;
	op.mShadowCoord.z =pos.z;

	return op;
}