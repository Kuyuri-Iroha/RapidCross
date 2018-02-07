
static const float PI =3.14159265f;

cbuffer g_cb : register(b0)
{
    matrix World;
    float Param;
	float3 dammy;
}


// �e�N�X�`��
Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);
Texture2D Mask : register(t1);


// �s�N�Z���V�F�[�_�̓��̓f�[�^
struct PSInput
{
    float4 mPos : SV_POSITION; //���_���W
    float2 mUV : TEXCOORD0; //UV���W
};


// �s�N�Z���V�F�[�_�̏o�̓f�[�^
struct PSOutput
{
    float4 mColor : SV_TARGET;
};


// �s�N�Z���V�F�[�_�֐�
PSOutput ParamPS(PSInput input)
{
    PSOutput op;
	op.mColor = float4(0.0, 0.0, 0.0, 0.0);
    float2 pos =input.mUV *2.0f -float2(1.0f, 1.0f);
    
    float mask =(atan2(pos.x, pos.y) +PI) /(PI *2.0f);
    if (mask <= Param)
    {
        //draw
        op.mColor.rgb = Texture.Sample(Sampler, input.mUV).rgb;
        op.mColor.a = 1.0;
    }
    else
    {
        op.mColor.a =0.0;
    }

    return op;
}