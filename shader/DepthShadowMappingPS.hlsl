#include	"common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureShadow : register(t1);   //�V���h�E�}�b�v
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    //LightPosition�̓��C�g���猩���s�N�Z����3D��Ԃł̍��W
    In.LightPosition.xyz /= In.LightPosition.w;
    //��ʏ�ł̍��W���e�N�X�`�����W�ɂ���
    In.LightPosition.x = In.LightPosition.x * 0.5f + 0.5f;
    In.LightPosition.y = -In.LightPosition.y * 0.5f + 0.5f;
    
    //PCF�V���h�E
    float2 offset = float2(1.0f / 960.0f, 1.0f / 540.0f);   //1�s�N�Z�����̍��W�l
    float shadow = 0.0f;                                    //���ӂŉB������Ă���s�N�Z���̐�
    float depth[4];                                         //���ӂ̃s�N�Z���̐[�x�m
    //�[�x�l�����o��
    depth[0] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy);
    depth[1] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, 0.0f));        //�E��
    depth[3] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, offset.y));    //�E��
    depth[2] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(0.0f, offset.y));        //����
    
    //�B������Ă���s�N�Z���̐��𒲂ׂ�
    for (int i = 0; i < 4; i++)
    {
        if (depth[i] < (In.LightPosition.z - 0.01f))
        {
            shadow++;
        }

    }
    shadow /= 4.0f;     //�e�̊��� 0.0�`1.0
    
    float3 shadowColor = outDiffuse.rgb * 0.5f;     //�e�̐F
    //outDiffusea��shadowColor��shadow�̒l�Ő��`��Ԃ���
    shadowColor = lerp(outDiffuse.rgb, shadowColor, shadow);
    outDiffuse.rgb = shadowColor;
    return;
    
    //�[�x�o�b�t�@���炱�̃s�N�Z���̋���
    //    float depth = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy);
    
    //�s�N�Z����3D��Ԃł�Z���W(����)�Ɛk�x�o�b�t�@�ł̋����l���r
    //if(depth < In.LightPosition.z - 0.01f)
    //{
    //    outDiffuse.rgb *= 0.5f;
    //}
}


