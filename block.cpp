#include "main.h"
#include "block.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"


void Block::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\boxTest.obj");
	BoxColiderComponent* colider = AddComponent<BoxColiderComponent>();

	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, -10.0f, 0.0f));
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(3.0f, 3.0f, 3.0f));

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	colider->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));
	Tag = "Block";

	
	//colider->OnDraw();
	//このオブジェクトは静的
	m_GameObjectKind = Static;
	//静的オブジェクトは最初の位置を設定してあげるだけでいい
	colider->MoveCollision();

	//描画しない
	if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
	{
		GetComponent<Transform3DComponent>()->SetDraw(false);
	}

}

void Block::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Block::Update()
{
	float add = 0.01f;

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<Colider>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));

	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void Block::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
}

