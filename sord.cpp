#include "main.h"
#include "sord.h"
#include "transform3DComponent.h"
#include "transform3DAnimationComponent.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "capsuleColiderComponent.h";
#include "scene.h"
#include "gameScene.h"
#include "player.h"

void Sord::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\sord.obj");

	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 1.0f, 0.0f));
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(0.01f, 0.01f, 0.01f));

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	
	Tag = "Sord";

	
	//colider->OnDraw();
	//このオブジェクトは静的
	m_GameObjectKind = Dynamic;

	//描画しない
	if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
	{
		GetComponent<Transform3DComponent>()->SetDraw(false);
	}

}

void Sord::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Sord::Update()
{
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerRot = playerTrans->GetRotation();
	//右手のマトリックスを取得を取得
	XMMATRIX rightHand = player->GetComponent<Transform3DAnimationComponent>()->GetModel()->GetRightHandMatrix();
	Transform3DComponent* trans = GetComponent<Transform3DComponent>();
	
	XMFLOAT3 rot = { playerRot.x,playerRot.y,playerRot.z };
	trans->SetRotation(rot);


	trans->SetPosition(playerTrans->GetPosition());

	trans->SetPosition(player->GetComponent<Transform>()->GetPosition());

	trans->SetLocalMatrix(rightHand);


	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void Sord::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
}

