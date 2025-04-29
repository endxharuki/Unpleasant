#pragma once
#include "gameObject.h"
#include "component.h"
#include "animationModel.h"
#include "modelRenderer.h"
#include "magicBlade.h"
#include "playerState.h"
#include "timer.h"
#include "sord.h"
#include "shadowVolume.h"
#include "polygon2D.h"

class Player : public GameObject
{
private:

	std::unordered_map<StateType, std::shared_ptr<PlayerState>> m_StateList;
	std::shared_ptr<PlayerState> m_CurrentState = nullptr;

	XMFLOAT3 Vector = {0.2f,0.5f,0.2f};
	float GravityScale = 0.05f;
	float Velocity = 0.0f;

	int sonickMoveFrame = 0.0f;
	XMFLOAT3 sonickVector = {};
	bool sonick = false;
	bool NoCollicion = false;

	Timer* m_PlayerTime = nullptr;

	Sord* m_Sord = nullptr;

	ShadowVolume* m_ShadowVolume = nullptr;
	Polygon2D* m_Shadow = nullptr;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void PlayerControl();
	void PlayerColision();

	void DrawImGui() override;

	
	// �X�e�[�g��o�^
	void RegisterState(StateType type, std::shared_ptr<PlayerState> state) {
		m_StateList[type] = state;
	}
	// �X�e�[�g��ύX
	void ChangeState(StateType type)
	{

		if (m_CurrentState != nullptr) {
			
			if (m_CurrentState->GetChangePossible() == false) return;
			if (m_CurrentState == m_StateList[type]) return;

			m_CurrentState->Exit();

		}
		
		m_CurrentState = m_StateList[type];
 		m_CurrentState->AddParentPlayer(this);
		m_CurrentState->Enter();
	};

};

