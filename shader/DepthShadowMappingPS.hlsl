#include	"common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureShadow : register(t1);   //シャドウマップ
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    //LightPositionはライトから見たピクセルの3D空間での座標
    In.LightPosition.xyz /= In.LightPosition.w;
    //画面上での座標をテクスチャ座標にする
    In.LightPosition.x = In.LightPosition.x * 0.5f + 0.5f;
    In.LightPosition.y = -In.LightPosition.y * 0.5f + 0.5f;
    
    //PCFシャドウ
    float2 offset = float2(1.0f / 960.0f, 1.0f / 540.0f);   //1ピクセル分の座標値
    float shadow = 0.0f;                                    //周辺で隠蔽されているピクセルの数
    float depth[4];                                         //周辺のピクセルの深度知
    //深度値を取り出す
    depth[0] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy);
    depth[1] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, 0.0f));        //右側
    depth[3] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(offset.x, offset.y));    //右下
    depth[2] = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy + float2(0.0f, offset.y));        //下側
    
    //隠蔽されているピクセルの数を調べる
    for (int i = 0; i < 4; i++)
    {
        if (depth[i] < (In.LightPosition.z - 0.01f))
        {
            shadow++;
        }

    }
    shadow /= 4.0f;     //影の割合 0.0～1.0
    
    float3 shadowColor = outDiffuse.rgb * 0.5f;     //影の色
    //outDiffuseaとshadowColorをshadowの値で線形補間する
    shadowColor = lerp(outDiffuse.rgb, shadowColor, shadow);
    outDiffuse.rgb = shadowColor;
    return;
    
    //深度バッファからこのピクセルの距離
    //    float depth = g_TextureShadow.Sample(g_SamplerState, In.LightPosition.xy);
    
    //ピクセルの3D空間でのZ座標(距離)と震度バッファでの距離値を比較
    //if(depth < In.LightPosition.z - 0.01f)
    //{
    //    outDiffuse.rgb *= 0.5f;
    //}
}


