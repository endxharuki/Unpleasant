#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerAttack1State.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"
#include "input.h"
#include "slashFirst.h"

void PlayerAttack1State::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Slash1");
	//�U�����I���܂łق��̏�Ԃւ̈ڍs�͂ł��Ȃ�
	m_ChangePossible = false;
	attackFrame = 50.0f;
	next = false;
}

void PlayerAttack1State::Update()
{

	attackFrame--;
	
	//�v���C���[��o�^
	if (attackFrame == 40.0f) Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<SlashFirst>(1)->AddParentGameObject(m_ParentPlayer);
	//��t���Ԃ̏���
	if (attackFrame <= 20.0f)
	{
		if (Input::GetInstance()->GetKeyTrigger(VK_SPACE))
		{
			next = true;
		}
	}

	//�I��
	if (attackFrame <= 0.0f)
	{
		m_ChangePossible = true;
		if (next == true)
		{
			m_ParentPlayer->ChangeState(StateType::Attack2);

		}
	}
	

}

void PlayerAttack1State::Exit()
{
}
