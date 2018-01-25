

#define PI 3.1415926535f



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



struct PSInput
{
	float4 mPos : SV_POSITION; //���_���W(�����ϊ����W)
	float2 mUV : TEXCOORD0; //�e�N�X�`�����W
	float3 mNorm : TEXCOORD1; //�@���x�N�g��
	float3 mViewDir : TEXCOORD2; //�����x�N�g��
	float3 mShadowCoord : TEXCOORD3; //�V���h�E�}�b�v���W
};


struct PSOutput
{
	float4 mColor : SV_TARGET;
};


// �e�N�X�`��
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);
Texture2D ShadowMap : register(t1);
SamplerComparisonState CmpSampler : register(s1);



// �s�N�Z���V�F�[�_�֐�
PSOutput ModelPS(PSInput ip)
{
	PSOutput op;
	float4 diffuse =Texture.Sample(Sampler, ip.mUV);
	float NL =max(dot(ip.mNorm, LDir), 0.0f); //�@���ƃ��C�g�x�N�g���Ƃ̓���
	float bright =NL*(1.0f/PI) *LStrength +Ambient; //�����o�[�g�g�U�Ɩ� *�����x +�A���r�G���g
	bright +=max(dot(normalize((LDir-2.0f)*NL*ip.mNorm), normalize(ip.mViewDir)), 0.0f) *Specular; //+�X�y�L������

	//�e�`��
	float shadowThreshold =ShadowMap.SampleCmpLevelZero(CmpSampler, ip.mShadowCoord.xy, ip.mShadowCoord.z-0.001f);
	float shadow =lerp(0.0f, 1.0f, shadowThreshold);
	shadow =shadow+Ambient;
	shadow =saturate(shadow);

	//�f�B�t���[�Y *���邳 *���F *�e�̖��邳
	op.mColor.rgb =diffuse.rgb *bright *LColor.rgb *shadow;
	op.mColor.a =diffuse.a;

	return op;
}