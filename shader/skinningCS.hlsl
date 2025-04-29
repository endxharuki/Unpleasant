
#include "common.hlsl"

struct Vertex
{
    float4 position;
    float4 normal;
    float4 diffuse;
    float2 uv;
    
    int4 BoneIndices;
    float4 BoneWeights;
};

cbuffer ModelBuffer : register(b0)
{
    uint maxVertexCount; // モデルの最大頂点数
    float4x4 boneMatrix[256]; // 各ボーンの変換行列
    
};

StructuredBuffer<Vertex> InputVertexBuffer : register(t0);
RWStructuredBuffer<VS_IN> OutputVertexBuffer : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // スレッドidを頂点idとする
    
    if (index >= maxVertexCount)
        return;
       
    Vertex inputVertex = InputVertexBuffer[index];
    
    // スキニング計算処理
    float4x4 skinMatrix;
    //skinMatrix = boneMatrix[inputVertex.BoneIndices.x];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.y];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.z];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.w];
    skinMatrix = mul(boneMatrix[inputVertex.BoneIndices.x], inputVertex.BoneWeights.x);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.y], inputVertex.BoneWeights.y);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.z], inputVertex.BoneWeights.z);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.w], inputVertex.BoneWeights.w);

    // 出力
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.position, skinMatrix);
    outputVertex.Position = inputVertex.position;

    // 法線用に移動を消す
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    
    skinMatrix = transpose(skinMatrix);
    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    outputVertex.Normal = inputVertex.normal;

    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;

    // 格納用バッファに入れていく
    OutputVertexBuffer[index] = outputVertex;
    
  
}
