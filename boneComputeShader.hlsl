// 入力バッファ
StructuredBuffer<matrix> gAnimationMatrices1 : register(t0); // アニメーション1行列
StructuredBuffer<matrix> gAnimationMatrices2 : register(t1); // アニメーション2行列
StructuredBuffer<int> gParentIndices : register(t2); // 親ボーンのインデックス

// 定数バッファ
cbuffer BlendBuffer : register(b0)
{
    float BlendRatio; // アニメーションのブレンド比率
}

// 出力バッファ
RWStructuredBuffer<matrix> gOutputBoneMatrices : register(u0); // 計算されたボーン行列

// スレッドグループサイズ
[numthreads(64, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    uint boneIndex = DTid.x;

    // 親ボーンのインデックスを取得
    int parentIndex = gParentIndices[boneIndex];

    // アニメーション行列の補間
    matrix blendedMatrix = lerp(gAnimationMatrices1[boneIndex], gAnimationMatrices2[boneIndex], BlendRatio);

    // 親ボーンの行列を適用
    if (parentIndex >= 0)
    {
        gOutputBoneMatrices[boneIndex] = mul(blendedMatrix, gOutputBoneMatrices[parentIndex]);
    }
    else
    {
        gOutputBoneMatrices[boneIndex] = blendedMatrix; // ルートボーンの場合
    }
}