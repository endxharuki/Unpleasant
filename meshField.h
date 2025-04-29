#pragma once
#include "gameObject.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define FIELD_X (46)
#define FIELD_Z (46)

class MeshField : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_Vertex[FIELD_X][FIELD_Z];

	std::vector<std::vector<VERTEX_3D>> m_VertexData;

	std::string m_FileName = "";

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	std::vector<int> m_Index;

	int m_FieldX = 100;
	int m_FieldZ = 100;

	float m_VertexSpace = 5.0f;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui() override;

	void UpdateVertexBuffer();
	void ReCreateVertexAndIndexBuffer();
	void LoadMeshField(const std::string& filename);
	float GetHeight(XMFLOAT3 pos);

};