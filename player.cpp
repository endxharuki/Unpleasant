#include "main.h"
#include "player.h"
#include "transform3DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "PlayerCamera.h"
#include "boxColiderComponent.h"
#include "input.h"
#include "audio.h"
#include "resultScene.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"
#include "capsuleColiderComponent.h"
#include "slashFirst.h"
#include "slashSecound.h"
#include "circle2DColider.h"
#include "playerIdleState.h"
#include "playerMoveState.h"
#include "playerJumpState.h"
#include "playerAttack1State.h"
#include "playerAttack2State.h"
#include "playerSkill1State.h"
#include "timer.h"
#include "sord.h"


void Player::Init()
{
	Transform3DAnimationComponent* trans = AddComponent<Transform3DAnimationComponent>();

	/*GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\Akai.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Akai_Idle.fbx","Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Akai_Run.fbx","Run");*/

	GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\PlayerTest.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Idle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\RunTest.fbx", "Run");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash1.fbx", "Slash1");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash2.fbx", "Slash2");


	GetComponent<Transform>()->SetSize(XMFLOAT3(0.01f, 0.01f, 0.01f));

	/*GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");*/

	AddComponent<CapsuleColiderComponent>();
	GetComponent<Colider>()->SetSize(XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(0.5f);

	//�R���C�_�[��`��
	//GetComponent<CapsuleColiderComponent>()->OnDraw();
	//�f�o�b�O��\��
	/*GetComponent<CapsuleColiderComponent>()->OnImGui();*/

	AddComponent<Audio>()->InitMaster();
	GetComponent<Audio>()->Load("asset\\Audio\\wan.wav");

	//���̃I�u�W�F�N�g�͓��I
	m_GameObjectKind = Dynamic;
	m_Weight = 10;

	Velocity = 1.0f;

	//HP��MP
	m_States =
	{
		500.0f,
		50.0f,
		false,
		0
	};

	Tag = "Player";

	RegisterState(StateType::Idle, std::make_shared<PlayerIdleState>());
	RegisterState(StateType::Move, std::make_shared<PlayerMoveState>());
	RegisterState(StateType::Jump, std::make_shared<PlayerJumpState>());
	RegisterState(StateType::Attack1, std::make_shared<PlayerAttack1State>());
	RegisterState(StateType::Attack2, std::make_shared<PlayerAttack2State>());
	RegisterState(StateType::Skill1, std::make_shared<PlayerSkill1State>());

	//�A�C�h����Ԃɂ��Ă���
	ChangeState(StateType::Idle);
	m_PlayerTime = new Timer; 


	XMFLOAT3 pos = trans->GetSize();

	//�����ǉ�
	Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Sord>(1);

	//�V���h�E�{�����[��
	m_Shadow = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Polygon2D>(2);
	m_ShadowVolume = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<ShadowVolume>(2);

}

void Player::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
	/*m_ShadowVolume->Uninit();
	m_Shadow->Uninit();

	delete m_ShadowVolume;
	delete m_Shadow;*/
	
}

void Player::Update()
{
	m_PlayerTime->Update();

	if (m_HitStop == true)
	{
		m_HitStopSecond += m_PlayerTime->GetDeltaTime();
		if (m_HitStopSecond >= 0.05f)
		{
			m_HitStop = false;
			m_HitStopSecond = 0.0f;
		}
		else return;
	}

	ComponentListSort();

	//�v���C���[����
	PlayerControl();

	//�v���C���[�̓����蔻��ƈړ��̔��f
	PlayerColision();

	if (m_States.HP <= 0.0f)
	{

		GetComponent<Colider>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));
		GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));

		//HP��MP
		m_States =
		{
			500.0f,
			50.0f,
			false,
			0
		};
	}

	//�v���C���[�𓮂����Ă���e�R���|�[�l���g�̃A�b�v�f�[�g
	for (auto com : ComponentList)
	{
		com->Update();
	}


	//MP������
	if (m_States.MP < 50.0f)
	{
		m_States.MP += 0.01f;
	}
	if (m_States.MP > 50.0f)
	{
		m_States.MP = 50.0f;
	}
	
	//�e�̃|�W�V����
	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
	m_ShadowVolume->SetPosition(pos);


}

void Player::Draw()
{
	for (auto com : ComponentList)
	{
		com->Draw();
	}

	DrawImGui();

	//XMMATRIX chaildScale, chaildRot, childTrans, childWorld;
	//
	//chaildRot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	//chaildScale = XMMatrixScaling(1.0f / m_Size.x, 1.0f / m_Size.y, 1.0f / m_Size.z);
	//childTrans = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	//childWorld = childTrans * chaildScale *  world;
	////�q�̃��[���h�@���q�̃��[�J�� * �e�̃��[���h

	//Renderer::SetWorldMatrix(childWorld);

	/*m_CildModel->Draw();*/
}

void Player::PlayerControl()
{
	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 rot = GetComponent<Colider>()->GetRotation();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVelocity();

	float posRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 cameraRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetComponent<Transform>()->GetRotation();
	float PI = 3.141592;

	if (Input::GetInstance()->GetKeyTrigger(VK_SPACE))
	{
		ChangeState(StateType::Attack1);
	}

	if (sonick == false) {

		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
		{
			ChangeState(StateType::Move);
		}
		else {
			ChangeState(StateType::Idle);
		}

	}


	//�W�����v����
	if (GetKeyState('Q') & 0x8000 && m_IsGravity == false) {

		ChangeState(StateType::Jump);
		//GetComponent<Audio>()->Play(false);

	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeState(StateType::Skill1);
	}
	
	
	//�d�͏���
	if (m_CurrentState != nullptr)
	{
		if (m_CurrentState->GetName() == StateType::Jump)
		{
			m_IsGravity = false;
		}
	}
	if (m_IsGravity == true) {
		TransPos.y -= m_GravityScale;
		m_GravityScale += 0.01f;
	}
	else {
		m_GravityScale = 0.05f;
	}

	if (Input::GetInstance()->GetKeyTrigger('G'))
	{
		m_IsGravity = !m_IsGravity;
	}

	GetComponent<Colider>()->SetPosition(TransPos);
	GetComponent<Colider>()->SetRotation(rot);
	GetComponent<Colider>()->SetVelocity(vel);
	//GetComponent<CapsuleColiderComponent>()->MoveCollision();

	GetComponent<Transform>()->SetPosition(TransPos);
	GetComponent<Transform>()->SetRotation(rot);
	GetComponent<Transform>()->SetVelocity(vel);


	if (m_CurrentState == nullptr) return;
	m_CurrentState->Update();

	
}

void Player::PlayerColision()
{
	////���_
	////1 �J�����o�O�邱�Ƃ��邩�璍�ӑ��U�߂�
	Transform* trans = GetComponent<Transform>();
	Colider* col = GetComponent<Colider>();

	XMFLOAT3 TransPos = trans->GetPosition();
	XMFLOAT3 TransRot = trans->GetRotation();
	XMFLOAT3 pos = col->GetPosition();
	XMFLOAT3 rot = col->GetRotation();
	float posRot = trans->GetRot();

	float hight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<MeshField>()->GetHeight(pos);

	
	if (NoCollicion == false)
	{
		if (col->GetIsCollision() == true)
		{
			m_IsGravity = true;
			if(m_CurrentState->GetName() != StateType::Jump)
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
		else 
		{
		
			m_IsGravity = true;

			if (m_CurrentState->GetName() != StateType::Jump) 
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}
			else 
			{
				m_IsGravity = false;
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
	

	}

	

	

	//if (GetComponent<Colider>()->GetColiderTag() == "Capsule")
	//{
	//	std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetComponent<CapsuleColiderComponent>()->GetCollision();
	//	//GetCollision��2���ǂ񂶂���Ă邩�璼���܂��傤
	//	if (std::get<0>(list))
	//	{
	//		/*if (NoCollicion == false)
	//		{*/
	//			std::list<GameObject*> objs = std::get<2>(list);

	//			for (auto onCollisionObj : objs)
	//			{

	//				if (onCollisionObj->GetComponent<Colider>()->GetColiderTag() == "Box")
	//				{
	//					/*if (GetComponent<CapsuleColiderComponent>()->GetCorrection().y > 0.0f)
	//					{
	//						isGravity = false;
	//					}*/
	//					XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	//					XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();

	//					if (isJump == false) {
	//						if (TransPos.y <= hight + 0.1f) {
	//							TransPos.y = hight;
	//							m_IsGravity = false;
	//						}
	//					}

	//					//XMFLOAT3 velocity = GetComponent<Colider>()->GetVelocity();
	//					////�R���C�_�[�̃|�W�V�����X�V
	//					
	//					GetComponent<Transform>()->SetPosition(TransPos);
	//					GetComponent<Transform>()->SetRotation(rot);
	//					GetComponent<Transform>()->SetRot(posRot);

	//				}
	//				if (onCollisionObj->GetComponent<Colider>()->GetColiderTag() == "Capsule")
	//				{
	//					////�R���C�_�[�̃|�W�V�����X�V
	//					//XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();

	//					//GetComponent<Transform>()->SetPosition(pos);
	//					//GetComponent<Transform>()->SetRotation(rot);
	//					//GetComponent<Transform>()->SetRot(posRot);
	//				}
	//			}
	//		//}
	//	}
	//	else {
	//		m_IsGravity = true;

	//		if (isJump == false) {
	//			if (TransPos.y <= hight + 0.1f) {
	//				TransPos.y = hight;
	//				m_IsGravity = false;
	//			}
	//		}

	//		GetComponent<Transform3DAnimationComponent>()->SetPosition(TransPos);
	//		GetComponent<Transform3DAnimationComponent>()->SetRotation(rot);
	//		GetComponent<Transform3DAnimationComponent>()->SetRot(posRot);
	//	}
	//}

}
float currentTime = 0.0f;

void Player::DrawImGui()
{

	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 ColiderPos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 CroiderRot = GetComponent<Colider>()->GetRotation();
	currentTime += m_PlayerTime->GetDeltaTime();

	{

		ImguiManager* imgui = Scene::GetInstance()->GetScene<GameScene>()->GetImguiManager();

		ImGui::Begin("Player");                          // Create a window called "Hello, world!" and append into it.              // Display some text (you can use a format strings too)

		static bool draw;

		ImGui::Checkbox("DrawColider", &draw);

		GetComponent<CapsuleColiderComponent>()->CapsuleDrawState(draw);

		ImGui::Text("playerPos , x = %.1f , y = %.1f , z = %.1f", TransPos.x, TransPos.y, TransPos.z);

		static  float add[3] = {0.0f,0.0f,0.0f};

		ImGui::SliderFloat3("AddPos", add, -1.0f, 1.0f);

		GetComponent<Colider>()->SetAddPosition(XMFLOAT3(add[0],add[1],add[2]));

		ImGui::Text("PlayerHP = %.1f", m_States.HP);

		ImGui::Text("FPS = %.1f", imgui->FrameRate);

		ImGui::Text("deltaTime = %.1f", currentTime);

		ImGui::End();

	}

}




