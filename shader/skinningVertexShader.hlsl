#include "common.hlsl"

StructuredBuffer<VS_IN> vertexBuffer : register(t0); // スキニング処理が施された頂点バッファ

struct CSVS_IN
{
    uint vertexID : SV_VertexID;
};

void main(in CSVS_IN In, out PS_IN Out)
{
    VS_IN vtx = vertexBuffer[In.vertexID];
	
	// なにか頂点に色々処理する
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(vtx.Position, wvp);
    Out.TexCoord = vtx.TexCoord;
    Out.Diffuse = vtx.Diffuse * Material.Diffuse;
    //Out.Diffuse = 1.0f;
    
}