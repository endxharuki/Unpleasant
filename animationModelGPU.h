#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObject.h"
#include "gameObjectComponent.h"


//変形後頂点構造体
//struct DEFORM_VERTEX
//{
//	aiVector3D Position;
//	aiVector3D Normal;
//	int				BoneNum;
//	std::string		BoneName[4];//本来はボーンインデックスで管理するべき
//	float			BoneWeight[4];
//};


class AnimationModelGPU : public GameObjectComponent
{
private:
	//ボーン構造体
	struct BONE
	{
		aiMatrix4x4 OffsetMatrix;       // ボーンのオフセットマトリクス
		aiMatrix4x4 FinalTransformation; // 最終変形マトリクス
	};

	//ボーンデータ
	struct BoneVertex
	{
		XMFLOAT3 Position;  // 頂点位置
		XMFLOAT3 Normal;    // 法線
		XMFLOAT2 TexCoord;  // テクスチャ座標
		int BoneIndices[4];          // ボーンインデックス（最大4つ）
		float BoneWeights[4];        // ボーンウェイト（最大4つ）
		XMFLOAT4 Diffuse;   // 色
	};

	//変形後頂点構造体
	struct DEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		int				BoneNum;
		std::string		BoneName[4];//本来はボーンインデックスで管理するべき
		float			BoneWeight[4];
	};

	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	

	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）

	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;

	// シェーダー
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout{};

	std::vector<VERTEX_3D> m_CPUProcessedVertices; // CPUでスキニングされた頂点データ

	int m_BoneCount = 0;
	float m_Alpha = 0.0f;

	

public:
	using GameObjectComponent::GameObjectComponent;

	void Load(const char* FileName);
	void Uninit() override;
	void Draw() override;

	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node);
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	void UpdateAnimation(const char* Animation, float frame);
	void UpdateBoneBuffer(std::vector<XMMATRIX>& boneMatrices);
	void CreateBoneBuffer(int boneCount);
	void LoadMeshData(aiMesh* mesh, int meshIndex);
	void LoadTextures();
    void CalculateBoneTransforms(const aiNode* node, const aiMatrix4x4& parentTransform, std::vector<DirectX::XMMATRIX>& transforms);
	aiMatrix4x4 GetBoneMatrices(const aiScene* animationScene, const std::string& boneName, int frame);


	std::unordered_map<std::string, BONE> GetBone() { return m_Bone; }

	void SetAlpha(float a) { m_Alpha = a; }
};