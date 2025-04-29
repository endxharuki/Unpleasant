// GPU�X�L�j���O�p���_�V�F�[�_�[

// �X�g���N�`���[�h�o�b�t�@�Ƃ��ă{�[���s���錾
StructuredBuffer<float4x4> g_BoneMatrices : register(t0);

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    int4 BoneIndices : BLENDINDICES;
    float4 BoneWeights : BLENDWEIGHT;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

[numthreads(256, 1, 1)]
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // �X�L�j���O�̌v�Z
    float4x4 skinningMatrix = 0;
    for (int i = 0; i < 4; i++)
    {
        if (input.BoneWeights[i] > 0)
        {
            skinningMatrix += g_BoneMatrices[input.BoneIndices[i]] * input.BoneWeights[i];
        }
    }
    // �ʒu�Ɩ@�����X�L�j���O
    float4 worldPosition = mul(float4(input.Position, 1.0f), skinningMatrix);
    float3 worldNormal = mul(float4(input.Normal, 0.0f), skinningMatrix).xyz;

    // �o�̓f�[�^
    output.Position = worldPosition;
    output.Normal = normalize(worldNormal);
    output.TexCoord = input.TexCoord;

    return output;

}