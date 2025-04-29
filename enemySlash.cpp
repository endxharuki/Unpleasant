#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "enemySlash.h"
#include "texturePool.h"
#include "capsuleColiderComponent.h"
#include "textDamage.h"
#include <random>


void EnemySlash::Init()
{
	if (initOn == true) {
		VERTEX_3D vertex[4];

		vertex[0].Position = XMFLOAT3(-3.0f, 3.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

		vertex[1].Position = XMFLOAT3(3.0f, 3.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

		vertex[2].Position = XMFLOAT3(-3.0f, -3.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

		vertex[3].Position = XMFLOAT3(3.0f, -3.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

		AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("slashSecound"));
		GetComponent<Transform2DComponent>()->SetVertex(vertex);
		GetComponent<Transform2DComponent>()->SetIsBilboad(true);

		for (auto com : ComponentList) {
			com->Init();
		}
		GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(0.0f, 0.0f, 0.0f));

		length = 3.0f;
		XMFLOAT3 texpos = { enemyPos.x + (length * sinf(enemyRot.y)),enemyPos.y + 1.5f ,enemyPos.z + (length * cosf(enemyRot.y)) };
		XMFLOAT3 capsulePos = { enemyPos.x + (length * sinf(enemyRot.y)),enemyPos.y + 1.5f ,enemyPos.z + (length * cosf(enemyRot.y)) };

		AddComponent<CapsuleColiderComponent>()->SetTransform(capsulePos, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 1.0f, 0.5f));
		GetComponent<CapsuleColiderComponent>()->NoUpdate();

		GetComponent<CapsuleColiderComponent>()->SetRotationAngle(XMFLOAT3(90.0f, 0.0f, 0.0f));
		XMFLOAT3 r = GetComponent<CapsuleColiderComponent>()->GetRotation();

		GetComponent<CapsuleColiderComponent>()->SetRotation(XMFLOAT3(r.x, enemyRot.y, r.z));


		XMFLOAT3 vector = { capsulePos.x - enemyPos.x,(capsulePos.y - enemyPos.y) - 1.0f,capsulePos.z - enemyPos.z };

		//vector = Normalize(vector, length);
		float PI = 3.141592;

		////�΂߂ɂ���
		float Rot = PI / 4;

		//XMFLOAT3 pos = GetComponent<Colider>()->GetAddAnglePosition(XMFLOAT3(0.0f,Rot,0.0f), vector);

		//rot = enemyRot.y;

		//pos = { pos.x * length,pos.y * length,pos.z * length };

		//pos = { enemyPos.x + pos.x ,enemyPos.y + pos.y + 1.0f ,enemyPos.z + pos.z };

		//GetComponent<CapsuleColiderComponent>()->SetPosition(pos);


		//�����W�^�C�v
		GetComponent<Colider>()->RotateAroundInit(enemyPos);
		GetComponent<Colider>()->RotateAround(-Rot, -Rot);


		XMFLOAT3 p = GetComponent<Colider>()->GetPosition();

		p = GetComponent<Colider>()->GetRotatePosition(enemyPos);

		GetComponent<CapsuleColiderComponent>()->SetPosition(p);


		//�J�v�Z���̐ݒ�
		GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(2.0f);

		//�J�v�Z����ݒu
		GetComponent<CapsuleColiderComponent>()->MoveCollision();

		GetComponent<CapsuleColiderComponent>()->DrawImGui();

		GetComponent<CapsuleColiderComponent>()->OnDraw();


		GetComponent<Transform2DComponent>()->SetPosition(texpos);

		//���b�g�̏�����
		rot = 0.0f;

		m_AttackPoint = 50;
	}
}

void EnemySlash::Uninit()
{
	for (auto obj : m_HitObjectList)
	{
		obj->SetHit(false);
		obj->SetHitCount(0);
	}

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void EnemySlash::Update()
{

	
	XMFLOAT3 capsulePos = GetComponent<Colider>()->GetPosition();
	
	float PI = 3.141592;


	//�����W�^�C�v
	float rotScale = PI * 0.005f;

	XMFLOAT3 p = GetComponent<Colider>()->GetPosition();
	GetComponent<Colider>()->RotateAround(rotScale, rotScale);

	p = GetComponent<Colider>()->GetRotatePosition(enemyPos);

	GetComponent<CapsuleColiderComponent>()->SetPosition(p);

	GetComponent<CapsuleColiderComponent>()->MoveCollision();

	HitAttackCollision();

	//�A�j���[�V����
	{

		VERTEX_3D vertex[4];
		for (int i = 0;i < 4;i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 9.0f;
		float y = 1.0f;
		float TexX = (frameCount / 8) % 9 * x;
		float TexY = 0.0f;

		vertex[0].TexCoord = XMFLOAT2(TexX, TexY);
		vertex[1].TexCoord = XMFLOAT2(TexX + x, TexY);
		vertex[2].TexCoord = XMFLOAT2(TexX, TexY + y);
		vertex[3].TexCoord = XMFLOAT2(TexX + x, TexY + y);
		GetComponent<Transform2DComponent>()->SetVertex(vertex);

		frameCount++;
	}

	if (frameCount >= 60.0f) isDelete = true;

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void EnemySlash::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}

void EnemySlash::HitAttackCollision()
{

	std::tuple<bool, GameObject*, std::list<GameObject*>> date = GetComponent<CapsuleColiderComponent>()->GetCollision();

	if (std::get<0>(date) == true)
	{
		std::list<GameObject*> objList = std::get<2>(date);

		bool hit = false;

		for (auto obj : objList)
		{
			if (obj->GetTag() == "Player")
			{
				if (obj->GetStates()->hit == false)
				{
					
					/*float hp = obj->GetStates().HP;
					hp -= 1.0f;
					obj->SetStates(hp, 0.0f, true,count);*/
					Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();

					int count = obj->GetStates()->hitCount;
					count++;

					
					obj->SetHitCount(count);

					//�q�b�g��
					if (count >= 1) obj->SetHit(true);
					m_HitObjectList.push_back(obj);

					XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
					XMFLOAT3 enemyRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();

					pos.y += 2.0f;

					XMFLOAT3 anglePos = GetComponent<GameObjectComponent>()->GetAddAnglePosition(enemyRot, XMFLOAT3(1.0f, 0.0f, 0.0f));
					anglePos = { anglePos.x * 0.5f,anglePos.y * 0.5f ,anglePos.z * 0.5f };
					
					int digits = std::log10(m_AttackPoint);

					digits++;

					float playerHp = player->GetStates()->HP;
					playerHp -= m_AttackPoint;
					player->SetHp(playerHp);

					//�_���[�W�\���𐶐�
					TextAttackPoint(digits);

					hit = true;

				}
			}

			if (hit == true) m_HitCount++;
			
		}
	}


}

void EnemySlash::TextAttackPoint(int n)
{

	//�v���C���[���擾
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerRot = playerTrans->GetRotation();
	XMFLOAT3 playerFoward = playerTrans->GetFoward();
	XMFLOAT3 playerRight = playerTrans->GetRight();

	float textToWiedth = 0.5f;


	XMFLOAT3 anglePos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 textPos = { anglePos.x - playerFoward.x,anglePos.y - playerFoward.y ,anglePos.z - playerFoward.z };


	TextDamage* text[4];

	int num = 0;

	for (int i = 0; i < n; i++)
	{
		if (i == 0) num = m_AttackPoint % 10;
		if (i == 1) num = (m_AttackPoint / 10) % 10;
		if (i == 2) num = (m_AttackPoint / 100) % 10;
		if (i == 3) num = m_AttackPoint / 1000;

		text[i] = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<TextDamage>(2);
		text[i]->GetComponent<Transform>()->SetTransform(
			textPos, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f));
		text[i]->SetTextUV(num);

		textPos = { textPos.x - (playerRight.x * textToWiedth),textPos.y /*- anglePos.y*/,textPos.z - (playerRight.z * textToWiedth) };

	}
	
	
}
