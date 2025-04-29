#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerAttack2State.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"
#include "input.h"
#include "slashFirst.h"
#include "slashSecound.h"

void PlayerAttack2State::Enter()
{

	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Slash2");
	//�U�����I���܂łق��̏�Ԃւ̈ڍs�͂ł��Ȃ�
	m_ChangePossible = false;
	attackFrame = 50.0f;

}

void PlayerAttack2State::Update()
{

	attackFrame--;

	//�v���C���[��o�^
	if (attackFrame == 35.0f) Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<SlashSecound>(1)->AddParentGameObject(m_ParentPlayer);
	

	//�I��
	if (attackFrame <= 0.0f)
	{
		m_ChangePossible = true;
	}

}

void PlayerAttack2State::Exit()
{
}
