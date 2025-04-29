#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "playerCamera.h"
#include "field.h"
#include "player.h"
#include "gameScene.h"
#include "resultScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "score.h"
#include "timeLimit.h"
#include "sky.h"
#include "goal.h"
#include "resultScene.h"
#include "meshField.h"
#include "house.h"
#include "treeFactory.h"
#include "tree.h"
#include "fade.h"
#include "texturePool.h"
#include "input.h"
#include "shadowVolume.h"
#include "gameSky.h"
#include "magicBlade.h"
#include "textHp.h"
#include "textGauge.h"
//#include "slashFirst.h"


void GameScene::Init()
{

	m_SceneName = GAME;

	//レイヤー順番バカ大事だから気を付けて
	TexturePool::GetInstance()->LoadTextrue();
	TexturePool::GetInstance()->Init();
	Input::GetInstance()->Init();

	AddGameObject<PlayerCamera>(0);//2Dのゲームオブジェクトを入れたらカメラは一番最後


	AddGameObject<Player>(1)->GetComponent<Colider>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));


	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(5.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(0.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(10.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));
	/*AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(13.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(18.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));
	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(20.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));*/



	//AddGameObject<House>(1);

	LoadMap("MapData1.csv");

	/*AddGameObject<GameSky>(1);
	AddGameObject<MeshField>(1);*/
	//GetGameObject<MeshField>()->LoadMeshField("");

	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(5.0f, 3.0f, 20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(20.0f, 0.0f, -39.0f), XMFLOAT3(45.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(25.0f, 3.0f, -30.0f), XMFLOAT3(0.0f, 45.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(10.0f, 0.0f, -25.0f), XMFLOAT3(44.0f, 0.0f, 40.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));

	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(5.0f, 8.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));


	////シャドウボリューム
	//AddGameObject<ShadowVolume>(1);
	//AddGameObject<Polygon2D>(1);

	AddUIObject<Fade>()->SetPosTexNumber(0.0f, 0.0f, 1400.0f, 800.0f, TexturePool::GetInstance()->GetTexture("Fade"), 1);
	GetUIObject<Fade>(1)->SetOnDraw(true);
	GetUIObject<Fade>(1)->SetFadeState(In);

	AddUIObject<TextHp>();
	AddUIObject<TextGauge>();

	for (auto tex : m_TextureList) {
		tex->Init();
	}
	
	imgui = new ImguiManager;
	imgui->Init();
}

void GameScene::Uninit()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Uninit();
			delete obj;
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Uninit();
			}
		}
		m_GameObjectList[i].clear();
	}
	TexturePool::GetInstance()->UnLoadTexture();
	imgui->Uninit();
	Input::GetInstance()->Uninit();
	
	Renderer::Uninit();
}

void GameScene::Update()
{
	//SortGameObject();

	Input::GetInstance()->Update();

	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Update();
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Update();
			}
		}
	}

	DeleteGameObject();

	
	if (isGoal == true)
	{
		AddGameObject<Goal>(1)->GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 220.0f));
	}
	if (isClear == true)
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}
	/*if (GetGameObject<Player>()->GetScore() >= 3) 
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}*/
}

void GameScene::Draw()
{
	//シャドウマッピングテスト
	LIGHT light;
	light.Enable = true;
	light.Direction = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0);
	
	XMVECTOR dir = XMLoadFloat4(&light.Direction);
	dir = XMVector4Normalize(dir);

	light.Direction.x = dir.m128_f32[0];
	light.Direction.y = dir.m128_f32[1];
	light.Direction.z = dir.m128_f32[2];
	light.Direction.w = dir.m128_f32[3];

	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラ作成
	XMFLOAT3 cpos = XMFLOAT3(-10.0f, 10.0f, -10.0f);
	XMFLOAT3 cat = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 cup = XMFLOAT3(0.0f, 1.0f, 0.0f);

	light.ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&cpos), XMLoadFloat3(&cat), XMLoadFloat3(&cup));
	//ライトプロジェクション作成
	light.ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);
	//Renderer::SetLight(light);

	/*light.ProjectionMatrix = XMMatrixTranspose(light.ProjectionMatrix);
	light.ViewMatrix = XMMatrixTranspose(light.ViewMatrix);*/

	//描画１回目	深度バッファシャドウの作成
	Renderer::BeginDepth();
	////ライトカメラ行列のセット
	Renderer::SetViewMatrix(light.ViewMatrix);
	Renderer::SetProjectionMatrix(light.ProjectionMatrix);


	//描画二回目
	Renderer::Begin();
	
	imgui->ImGuiRendererInit();

	for (int i = 0;i < 3;i++) {
		
		for (auto obj : m_GameObjectList[i])
		{
			obj->Draw();
		}
		if (i == 2)
		{
			for (auto tex : m_TextureList)
			{
				tex->Draw();
			}
		}
		
	}
	imgui->ImGuiRenderer();
	
	Renderer::End();
}

void GameScene::DeleteGameObject()
{
	for (int i = 0;i < 3;i++) {
		m_GameObjectList[i].remove_if([](GameObject* obj) {

			if (obj->DeleteRecest() == true) {
			  //if(obj->GetTag() == "Enemy") Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->SetScore(1);
				obj->Uninit();
				delete obj;
				return true;
			}

			return false;
			}
		);
	}
	
	m_TextureList.remove_if([](UI* obj) {

		if (obj->DeleteRecest() == true)
		{
			obj->Uninit();
			delete obj;

			return true;
		}
		return false;
		}
	);
}

void GameScene::SortGameObject()
{
	m_GameObjectList[1].sort([]( GameObject* a,  GameObject* b)
		{
			return a->GetTag() < b->GetTag();
		}
	);

}

void GameScene::LoadMap(const std::string& filename)
{

	m_MapFileName = filename;
	std::ifstream file(filename);
	if (!file) return;

	std::string line;
	std::getline(file, line);  //ヘッダー行を読み飛ばす 種類

	while (std::getline(file, line))
	{
		
		std::stringstream ss(line);
		std::string objName;

		std::getline(ss, objName, ',');

		if (objName == "MeshField")
		{
			std::string fieldFileName;
			std::getline(ss, fieldFileName, ',');
			AddGameObject<MeshField>(0)->LoadMeshField(fieldFileName);
		}
		else
		{
			XMFLOAT3 pos;
			XMFLOAT3 size;
			XMFLOAT3 rot;


			std::string posX, posY, posZ;
			std::string sizeX, sizeY, sizeZ;
			std::string rotX, rotY, rotZ;
			//座標
			std::getline(ss, posX, ',');
			std::getline(ss, posY, ',');
			std::getline(ss, posZ, ',');
			//サイズ
			std::getline(ss, sizeX, ',');
			std::getline(ss, sizeY, ',');
			std::getline(ss, sizeZ, ',');
			//回転
			std::getline(ss, rotX, ',');
			std::getline(ss, rotY, ',');
			std::getline(ss, rotZ, ',');

			pos.x = std::stof(posX);
			pos.y = std::stof(posY);
			pos.z = std::stof(posZ);

			size.x = std::stof(sizeX);
			size.y = std::stof(sizeY);
			size.z = std::stof(sizeZ);

			rot.x = std::stof(rotX);
			rot.y = std::stof(rotY);
			rot.z = std::stof(rotZ);

			if (objName == "Sky")
			{
				AddGameObject<GameSky>(1)->GetComponent<Transform>()->SetTransform(pos, rot, size);

			}
			if (objName == "Block")
			{
				AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransform(pos, rot, size);

			}
			else if (objName == "Tree")
			{
				AddGameObject<Tree>(2)->GetComponent<Transform>()->SetTransform(pos,rot,size);
			}

		}
	}


}
