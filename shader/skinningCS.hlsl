
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
    uint maxVertexCount; // ���f���̍ő咸�_��
    float4x4 boneMatrix[256]; // �e�{�[���̕ϊ��s��
    
};

StructuredBuffer<Vertex> InputVertexBuffer : register(t0);
RWStructuredBuffer<VS_IN> OutputVertexBuffer : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // �X���b�hid�𒸓_id�Ƃ���
    
    if (index >= maxVertexCount)
        return;
       
    Vertex inputVertex = InputVertexBuffer[index];
    
    // �X�L�j���O�v�Z����
    float4x4 skinMatrix;
    //skinMatrix = boneMatrix[inputVertex.BoneIndices.x];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.y];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.z];
    //skinMatrix += boneMatrix[inputVertex.BoneIndices.w];
    skinMatrix = mul(boneMatrix[inputVertex.BoneIndices.x], inputVertex.BoneWeights.x);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.y], inputVertex.BoneWeights.y);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.z], inputVertex.BoneWeights.z);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices.w], inputVertex.BoneWeights.w);

    // �o��
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.position, skinMatrix);
    outputVertex.Position = inputVertex.position;

    // �@���p�Ɉړ�������
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    
    skinMatrix = transpose(skinMatrix);
    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    outputVertex.Normal = inputVertex.normal;

    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;

    // �i�[�p�o�b�t�@�ɓ���Ă���
    OutputVertexBuffer[index] = outputVertex;
    
  
}
