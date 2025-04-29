#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"
#include "tree.h"
#include "meshField.h"
#include "texturePool.h"

void Tree::Init()
{

	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\Tree\\Tree.obj");
	Transform* trans = GetComponent<Transform>();
	
	trans->SetSize(XMFLOAT3(2.0f, 2.0f, 2.0f));
	AddComponent<CapsuleColiderComponent>()->SetSize(XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(3.0f);

	//trans->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	
	m_GameObjectKind = Static;
	Tag = "Tree";

}

void Tree::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Tree::Update()
{

	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
	float hight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<MeshField>()->GetHeight(pos);
	float coliderPosY = GetComponent<Colider>()->GetSize().y;

	GetComponent<Transform>()->SetPosition(XMFLOAT3(pos.x, hight, pos.z));

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void Tree::Draw()
{
	//GetComponent<Transform>()->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	//Renderer::SetDepthEnable(false);
	for (auto com : ComponentList)
	{
		com->Draw();
	}
	//Renderer::SetDepthEnable(true);
}
