// ���̓o�b�t�@
StructuredBuffer<matrix> gAnimationMatrices1 : register(t0); // �A�j���[�V����1�s��
StructuredBuffer<matrix> gAnimationMatrices2 : register(t1); // �A�j���[�V����2�s��
StructuredBuffer<int> gParentIndices : register(t2); // �e�{�[���̃C���f�b�N�X

// �萔�o�b�t�@
cbuffer BlendBuffer : register(b0)
{
    float BlendRatio; // �A�j���[�V�����̃u�����h�䗦
}

// �o�̓o�b�t�@
RWStructuredBuffer<matrix> gOutputBoneMatrices : register(u0); // �v�Z���ꂽ�{�[���s��

// �X���b�h�O���[�v�T�C�Y
[numthreads(64, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    uint boneIndex = DTid.x;

    // �e�{�[���̃C���f�b�N�X���擾
    int parentIndex = gParentIndices[boneIndex];

    // �A�j���[�V�����s��̕��
    matrix blendedMatrix = lerp(gAnimationMatrices1[boneIndex], gAnimationMatrices2[boneIndex], BlendRatio);

    // �e�{�[���̍s���K�p
    if (parentIndex >= 0)
    {
        gOutputBoneMatrices[boneIndex] = mul(blendedMatrix, gOutputBoneMatrices[parentIndex]);
    }
    else
    {
        gOutputBoneMatrices[boneIndex] = blendedMatrix; // ���[�g�{�[���̏ꍇ
    }
}