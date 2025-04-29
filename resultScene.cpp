#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "resultScene.h"
#include "gameScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "titleScene.h"
#include "texturePool.h"

void ResultScene::Init()
{

	m_SceneName = RESULT;

	audio = new Audio;

	audio->InitMaster();
	audio->Load("asset\\Audio\\title.wav");

	Renderer::Init();
	//���W�ƃe�N�X�`���ƃi���o�[��ݒ�

	//���W�ƃe�N�X�`���ƃi���o�[��ݒ�
	AddTexture<Texture2D>()->SetPosTexNumber(200.0f, 0.0f, 1000.0f, 500.0f, TexturePool::GetInstance()->GetTexture("Fade"), 1);
	AddTexture<Texture2D>()->SetPosTexNumber(420.0f, 500.0f, 500.0f, 180.0f, TexturePool::GetInstance()->GetTexture("Fade"), 2);
	


	for (auto tex : m_TextureList)
	{

		tex->Init();
	}

	
	//���C���[���ԃo�J�厖������C��t����
	audio->Play(false);

}

void ResultScene::Uninit()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Uninit();
			delete obj;
		}

		m_GameObjectList[i].clear();
	}
	for (auto tex : m_TextureList) {
		tex->Uninit();
		delete tex;
	}
	Renderer::Uninit();

	audio->Uninit();
	delete audio;

}

void ResultScene::Update()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Update();
		}
	}
	for (auto tex : m_TextureList) {
		tex->Update();
		
	}

	drawFrame++;
	if (drawFrame >= 40.0f)
	{

		GetTexture(2)->SetOnDraw(false);
	}
	if (drawFrame >= 80.0f)
	{

		GetTexture(2)->SetOnDraw(true);
		drawFrame = 0.0f;
	}

	DeleteGameObject();
	//�V�[����؂�ւ���O��DeletegameObject���Ă΂Ȃ��ƃG���[�o��
	if (GetKeyState(VK_SPACE) & 0x8000) {
		
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new TitleScene);
	}

}

void ResultScene::Draw()
{

	Renderer::Begin();
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i])
		{
			obj->Draw();
		}
	}
	for (auto tex : m_TextureList) {
		tex->Draw();
	}
	Renderer::End();

}
