#include "common.hlsl"

//エントリー関数
void main(in VS_IN In, out PS_IN Out)
{
	//頂点変換処理
    matrix wvp; //ワールドビュープロジェクション行列
    wvp = mul(World, View); //wvp = World行列 * View行列
    wvp = mul(wvp, Projection); //wvp = wvp + Projection行列
    Out.Position = mul(In.Position, wvp); //頂点座標を変換して出力
    
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    
    float light = -dot(Light.Direction.xyz, worldNormal.xyz);
    light = saturate(light);
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    
    Out.Position = mul(In.Position, wvp);
    Out.Normal = worldNormal;
    Out.TexCoord = In.TexCoord;
    
    //ライト亀rでt頂点変換して出力
    matrix lightwvp;
    lightwvp = mul(World, Light.ViewMatrix);
    lightwvp = mul(lightwvp, Light.ProjectonMatrix);
    Out.LightPosition = mul(In.Position, lightwvp);
    
}

