
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	//このピクセルに使うテクセルを取得
    outDiffuse.rgba = g_Texture.Sample(g_SamplerState, In.TexCoord);


//	outDiffuse.rgb = 1.0f - outDiffuse.rgb;


//	outDiffuse.rgb *= outDiffuse.rgb;


	////20240517 練習
	////グレースケール化してみる
	//outDiffuse.rgb =	0.299f * outDiffuse.r +
	//					0.587f * outDiffuse.g +
	//					0.114f * outDiffuse.b;
	////セピア調変換
	//float4	sepia = float4(0.96f, 0.784f, 0.58f, 1.0f);
	//outDiffuse *= sepia;



	//テクセルの情報にピクセルのデフューズ色を乗算する
    outDiffuse *= In.Diffuse;

}
