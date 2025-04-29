#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObject.h"
#include "gameObjectComponent.h"


//�ό`�㒸�_�\����
//struct DEFORM_VERTEX
//{
//	aiVector3D Position;
//	aiVector3D Normal;
//	int				BoneNum;
//	std::string		BoneName[4];//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
//	float			BoneWeight[4];
//};


class AnimationModelGPU : public GameObjectComponent
{
private:
	//�{�[���\����
	struct BONE
	{
		aiMatrix4x4 OffsetMatrix;       // �{�[���̃I�t�Z�b�g�}�g���N�X
		aiMatrix4x4 FinalTransformation; // �ŏI�ό`�}�g���N�X
	};

	//�{�[���f�[�^
	struct BoneVertex
	{
		XMFLOAT3 Position;  // ���_�ʒu
		XMFLOAT3 Normal;    // �@��
		XMFLOAT2 TexCoord;  // �e�N�X�`�����W
		int BoneIndices[4];          // �{�[���C���f�b�N�X�i�ő�4�j
		float BoneWeights[4];        // �{�[���E�F�C�g�i�ő�4�j
		XMFLOAT4 Diffuse;   // �F
	};

	//�ό`�㒸�_�\����
	struct DEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		int				BoneNum;
		std::string		BoneName[4];//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
		float			BoneWeight[4];
	};

	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//�ό`�㒸�_�f�[�^
	

	std::unordered_map<std::string, BONE> m_Bone;//�{�[���f�[�^�i���O�ŎQ�Ɓj

	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;

	// �V�F�[�_�[
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout{};

	std::vector<VERTEX_3D> m_CPUProcessedVertices; // CPU�ŃX�L�j���O���ꂽ���_�f�[�^

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