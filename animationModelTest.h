#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObjectComponent.h"
#include "gameObject.h"
#include <map>
#define MAX_MESH (100)

class AnimationModelTest: public GameObjectComponent
{
private:
	//�ό`�㒸�_�\����
	struct DEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		int				BoneNum;
		int             BoneIndices[4];
		std::string		BoneName[4];//�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
		float			BoneWeight[4];
	};

	//�{�[���\����
	struct BONE
	{
		aiMatrix4x4 Matrix = { 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 AnimationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 OffsetMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f };

	};

	//�{�[���f�[�^
	struct BoneVertex
	{
		XMFLOAT4 Position;  // ���_�ʒu
		XMFLOAT4 Normal;    // �@��
		XMFLOAT4 Diffuse;   // �F
		XMFLOAT2 TexCoord;  // �e�N�X�`�����W
		int BoneIndices[4];          // �{�[���C���f�b�N�X�i�ő�4�j
		float BoneWeights[4];        // �{�[���E�F�C�g�i�ő�4�j

	};
	struct Vertex
	{
		XMFLOAT4 Position;  // ���_�ʒu
		XMFLOAT4 Normal;    // �@��
		XMFLOAT4 Diffuse;   // �F
		XMFLOAT2 TexCoord;  // �e�N�X�`�����W
	};
	struct BoneMatrices
	{
		// ���_��
		
		XMFLOAT4X4 boneMatrices[256];
		UINT vertexCount;
		
	};


	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;
	ID3D11Buffer* m_IndexAllBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;

//	std::unordered_map<std::string, BONE> m_Bone;//�{�[���f�[�^�i���O�ŎQ�Ɓj
	std::unordered_map<std::string, BONE> m_Bone;//�{�[���f�[�^�i���O�ŎQ�Ɓj
	BoneMatrices m_BoneMatrices;
	// �{�[��ID�����蓖�Ă�}�b�v
	std::unordered_map<std::string, int> boneNameToID;

	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;

	//GPU�X�L�j���O�p�o�b�t�@��SRV��UAV
	//CS
	ID3D11Buffer* m_BeforeVertexBuffer[MAX_MESH];
	ID3D11Buffer* m_AfterVertexBuffer[MAX_MESH];

	ID3D11ShaderResourceView* m_BeforeVertexShaderSRV[MAX_MESH];
	ID3D11UnorderedAccessView* m_AfterVertexShaderUAV[MAX_MESH];
	//VS
	ID3D11Buffer* m_SkinningUpdateBuffer[MAX_MESH];
	ID3D11ShaderResourceView* m_SkinningUpdateSRV[MAX_MESH];

	ID3D11Buffer* m_ConstantBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11ComputeShader* m_ComputeShader{};

	int m_BoneCount = 0;
	float m_Alpha = 0.0f;
	int m_VertexCount = 0;

	XMMATRIX m_RightHandMatrix = {};

public:
	using GameObjectComponent::GameObjectComponent;

	void Load(const char* FileName);
	void Uninit() override;
	void Draw() override;

	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node,// �{�[���� �� �{�[��ID �̃}�b�s���O
		std::map<std::string, int>& boneNameIndex,
	int& boneCount);
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
	void UpdateBoneBuffer(std::vector<aiMatrix4x4> boneMatrices);

	int GetBoneIndex(const std::string& boneName);

	void CreateBeforeVertexBuffer(int index, BoneVertex* vertex ,int mesh);
	void CreateBeforeVertexSRV(int index, int mesh);
	void CreateAfterVertexBuffer(int index, Vertex* vertex, int mesh);

	void CreateUAV(int index, int mesh);
	void CreateAfterVertexSRV(int index, int mesh);
	void CreateConstantBuffer();

	void SetBoneMatrices(const BoneMatrices* matrices);


	std::unordered_map<std::string, BONE> GetBone() { return m_Bone; }

	void SetAlpha(float a) { m_Alpha = a; }
	XMMATRIX GetRightHandMatrix() const { return m_RightHandMatrix; }
	

};