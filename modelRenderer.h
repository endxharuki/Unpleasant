#pragma once
#include "gameObjectComponent.h"


// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "component.h"
#include <string>
#include <unordered_map>


class ModelRenderer : public GameObjectComponent
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;


	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	MODEL* m_Model{};

	std::vector<DirectX::XMMATRIX> m_BoneMatrices;//�{�[���s��
	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;


	struct BoneVertex
	{
		DirectX::XMFLOAT3 Position;  // ���_�ʒu
		DirectX::XMFLOAT3 Normal;    // �@��
		DirectX::XMFLOAT2 TexCoord;  // �e�N�X�`�����W
		int BoneIndices[4];          // �{�[���C���f�b�N�X�i�ő�4�j
		float BoneWeights[4];        // �{�[���E�F�C�g�i�ő�4�j
	};

public:

	static void Preload( const char *FileName );
	static void UnloadAll();


	using GameObjectComponent::GameObjectComponent;

	void Load( const char *FileName );

	void Draw() override;

	void CreateBoneBuffer(ID3D11Device* device, int boneCount);

};