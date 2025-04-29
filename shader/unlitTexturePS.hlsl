
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	//���̃s�N�Z���Ɏg���e�N�Z�����擾
    outDiffuse.rgba = g_Texture.Sample(g_SamplerState, In.TexCoord);


//	outDiffuse.rgb = 1.0f - outDiffuse.rgb;


//	outDiffuse.rgb *= outDiffuse.rgb;


	////20240517 ���K
	////�O���[�X�P�[�������Ă݂�
	//outDiffuse.rgb =	0.299f * outDiffuse.r +
	//					0.587f * outDiffuse.g +
	//					0.114f * outDiffuse.b;
	////�Z�s�A���ϊ�
	//float4	sepia = float4(0.96f, 0.784f, 0.58f, 1.0f);
	//outDiffuse *= sepia;



	//�e�N�Z���̏��Ƀs�N�Z���̃f�t���[�Y�F����Z����
    outDiffuse *= In.Diffuse;

}
