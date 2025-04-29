#include "main.h"
#include "EnemyAIController.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "sceneState.h"
#include "enemy.h"
#include "player.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"
#include "enemySlash.h"
#include "capsuleColiderComponent.h"

//EnemyStateBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum class BehaviorResult { SUCCESS, FAILURE, RUNNING };

// �r�w�r�A�c���[�̃m�[�h�̊��N���X
class BehaviorNode {
private:
	EnemyState* m_EnemyState = nullptr;

public:
	virtual ~BehaviorNode() = default;
	virtual BehaviorResult Execute() = 0;
	virtual void Enter() {};//���̃m�[�h�ɓ��������̏���
	virtual void Update() {};//���̃m�[�h�̓����̍X�V
	virtual void Exit() {};//���̃m�[�h����o��Ƃ�

};

// Sequence (���ׂĂ̎q�m�[�h�� SUCCESS �Ȃ� SUCCESS)
class Sequence : public BehaviorNode {
	std::vector<std::unique_ptr<BehaviorNode>> children;
public:
	void AddChild(std::unique_ptr<BehaviorNode> child) { children.push_back(std::move(child)); }
	BehaviorResult Execute() override {
		for (auto& child : children) {
			if (child->Execute() != BehaviorResult::SUCCESS) return BehaviorResult::FAILURE;
		}
		return BehaviorResult::SUCCESS;
	}

};


//Idle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyIdleState::Init()
{



}

void EnemyIdleState::Update()
{

	//�Q�[���V�[�����擾
	GameScene* scene = Scene::GetInstance()->GetScene<GameScene>();
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 enemyPos = parentEnemy->GetComponent<Transform>()->GetPosition();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");

	//�v���C���[�܂ł̃x�N�g��
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//���������b�V���t�B�[���h�ɍ��킹��
	float higth = scene->GetGameObject<MeshField>()->GetHeight(enemyPos);
	enemyPos.y = higth;

	//�v���C���[�܂ł̃x�N�g���ƃG�l�~�[�������Ă�������̃x�N�g���̂Ȃ��p�x�����߂�
	{
		// ���ς��v�Z
		float dotProduct = Dot(vector, FrontVector);

		// �x�N�g���̑傫�����v�Z
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//�x�N�g���̑傫����0�������珈���𒆎~
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cos�Ƃ��v�Z
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//�قڐ��ʂ������Ă���
		if (angleDegrees < 5.0f)
		{
			//�߂�������U������
			if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
			}
			else if(len <= 30.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Move, parentEnemy);

			}
			return;
		}
		else
		{
			parentEnemy->GetComponent<EnemyAI>()->ChangeState(Rotate, parentEnemy);
		}

		//parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	}


	//hp���Ȃ��Ȃ����玀�ʃ��[�V�����ֈڍs
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

	parentEnemy->GetComponent<Transform>()->SetPosition(enemyPos);

}

void EnemyIdleState::Uninit()
{
}
//MoveState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyMoveState::Init()
{



}

void EnemyMoveState::Update()
{
	//�A�j���[�V�����̕ύX
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");

	Transform* transform = parentEnemy->GetComponent<Transform>();

	//�Q�[���V�[�����擾
	GameScene* scene = Scene::GetInstance()->GetScene<GameScene>();
	XMFLOAT3 playerPos = scene->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();

	XMFLOAT3 enemyPos = transform->GetPosition();
	XMFLOAT3 rotation = transform->GetRotation();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	//�x�N�g���̌���
	//XMFLOAT3 vector = {  playerPos.x - enemyPos.x,playerPos.y - enemyPos.y,playerPos.z - enemyPos.z};
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };

	float length = Length(vector);
	
	//�v���C���[�܂ł̃x�N�g��
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//�v���C���[�܂ł̃x�N�g���ƃG�l�~�[�������Ă�������̃x�N�g���̂Ȃ��p�x�����߂�
	{
		// ���ς��v�Z
		float dotProduct = Dot(vector, FrontVector);

		// �x�N�g���̑傫�����v�Z
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//�x�N�g���̑傫����0�������珈���𒆎~
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cos�Ƃ��v�Z
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//�قڐ��ʂ������Ă���
		if (angleDegrees < 5.0f)
		{
			//�߂�������U������
			if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
				return;
			}
			else if (len >= 30.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);
				return;
			}
			//�ړ�������
			{
				float speed = 0.02f;
				XMFLOAT3 moveVector = { vector.x * speed,vector.y * speed,vector.z * speed };

				enemyPos = { enemyPos.x + moveVector.x,enemyPos.y + moveVector.y ,enemyPos.z + moveVector.z };

				//���������b�V���t�B�[���h�ɍ��킹��
				float higth = scene->GetGameObject<MeshField>()->GetHeight(enemyPos);
				enemyPos.y = higth;
			}

			transform->SetPosition(enemyPos);
			transform->SetRotation(rotation);


			return;
		}
		else
		{
			parentEnemy->GetComponent<EnemyAI>()->ChangeState(Rotate, parentEnemy);
		}
	}

	

	//hp���Ȃ��Ȃ����玀�ʃ��[�V�����ֈڍs
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}
	//�v���C���[�ɂ�����x�߂��Ȃ�����U������
	if (length <= 5.0f) parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);

}

void EnemyMoveState::Uninit()
{
}
//Rotate
////
////
void EnemyRotateState::Init()
{

}

void EnemyRotateState::Update()
{

	//�A�j���[�V�����̕ύX
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	//�Q�[���V�[�����擾
	GameScene* scene = Scene::GetInstance()->GetScene<GameScene>();
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 enemyPos = parentEnemy->GetComponent<Transform>()->GetPosition();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	//�v���C���[�܂ł̃x�N�g��
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//�v���C���[�܂ł̃x�N�g���ƃG�l�~�[�������Ă�������̃x�N�g���̂Ȃ��p�x�����߂�
	{
		// ���ς��v�Z
		float dotProduct = Dot(vector, FrontVector);

		// �x�N�g���̑傫�����v�Z
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//�x�N�g���̑傫����0�������珈���𒆎~
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cos�Ƃ��v�Z
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//���������b�V���t�B�[���h�ɍ��킹��
		float higth = scene->GetGameObject<MeshField>()->GetHeight(enemyPos);
		enemyPos.y = higth;

		//�قڐ��ʂ������Ă���
		if (angleDegrees < 5.0f)
		{
			//�߂�������U������
			if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
			}
			else if(len >= 30.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);

			}
			else
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Move, parentEnemy);

			}
			return;
		}

		//parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	}

	//�v���C���[�܂ł̃x�N�g���ƃG�l�~�[�������Ă�������̉E���̃x�N�g���̂Ȃ��p�x�����߂�
	{
		// ���ς��v�Z
		float dotProduct = Dot(vector, RightVector);

		// �x�N�g���̑傫�����v�Z
		float vec1len = Length(vector);
		float vec2len = Length(RightVector);

		//�x�N�g���̑傫����0�������珈���𒆎~
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cos�Ƃ��v�Z
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;


		if (angleDegrees < 90.0f)
		{
			XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();
			rot.y += 0.02f;
			parentEnemy->GetComponent<Transform>()->SetRotation(rot);
		}
		else
		{
			XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();
			rot.y -= 0.02f;
			parentEnemy->GetComponent<Transform>()->SetRotation(rot);
		}


	}
	
	//hp���Ȃ��Ȃ����玀�ʃ��[�V�����ֈڍs
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

}

void EnemyRotateState::Uninit()
{
}

//Attack
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyAttackState::Init()
{
}

void EnemyAttackState::Update()
{
	XMFLOAT3 pos = parentEnemy->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();

	

	if (attackFrame == 1.0f)
	{
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack");
	}
	if (attackFrame == 40.0f) {
		EnemySlash* slash = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<EnemySlash>(2);
		slash->SetEnemyPos(pos);
		slash->SetEnemyRot(rot);
		slash->OnInit();
		slash->Init();

	}
	if (attackFrame == 200)
	{
		
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);
	}
	if (attackFrame == 220) attackFrame = 0.0f;
	attackFrame++;

	//hp���Ȃ��Ȃ����玀�ʃ��[�V�����ֈڍs
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

	/*if(attackFrame >= endFrame) parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);*/
	
}

void EnemyAttackState::Uninit()
{
}
//Died
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyDiedState::Init()
{
}

void EnemyDiedState::Update()
{
	Colider* com = parentEnemy->GetComponent<CapsuleColiderComponent>();
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Died");
	if (downFrame == 0.0f)
	{
		com->SetDelete(true);
	}
	if (downFrame >= 300.0f)
	{
		
		//���񂾃��[�V�����̐Ղ͏���
		/*parentEnemy->SetDelete(true);*/
		alpah += 0.005f;
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->GetModel()->SetAlpha(alpah);
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetDelay(1000);
		if (alpah >= 1.0f)
		{
			parentEnemy->SetDelete(true);
		}
	}

	downFrame++;
}

void EnemyDiedState::Uninit()
{
}

//EnemyAI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyAI::Init()
{
	EnemyIdleState* state = new EnemyIdleState;
	State = state;
	State->Init();

	State;
}

void EnemyAI::Update()
{

	State->AddParentEnemy(m_Parent);
	State->Update();

}

void EnemyAI::Uninit()
{

	State->Uninit();
	delete State;
}

void EnemyAI::ChangeState(AIControllerState state, Enemy* parent)
{

	if (state == Idle)
	{
		EnemyIdleState* state = new EnemyIdleState;
		State = state;
		State->AddParentEnemy(m_Parent);
		
	}
	if (state == Move)
	{
		EnemyMoveState* state = new EnemyMoveState;
		State = state;
		State->AddParentEnemy(m_Parent);

	}
	if (state == Rotate)
	{
		EnemyRotateState* state = new EnemyRotateState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
	if (state == Attack)
	{
		EnemyAttackState* state = new EnemyAttackState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
	if (state == Died)
	{
		EnemyDiedState* state = new EnemyDiedState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
}


