#pragma once
#include "gameObject.h"
#include "component.h"
#include "shadowVolume.h"
#include "polygon2D.h"

class Enemy : public GameObject
{
private:

	Component* m_Compornent = nullptr;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ShadowVolume* m_ShadowVolume = nullptr;
	Polygon2D* m_Shadow = nullptr;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui();

};

