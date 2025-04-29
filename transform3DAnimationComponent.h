#pragma once
#include "transform.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "animationModel.h"
#include "animationModelGPU.h"
#include "animationModelTest.h"
#include <map>

class Transform3DAnimationComponent : public Transform
{

private:

	AnimationModel* m_Model = {};
	AnimationModelTest* m_TestModel = {};
	std::string m_NextAnimationName{};
	std::string m_CurrentAnimationName{};


	std::map<std::string,std::string> m_AnimationList;

	int m_AnimationFrame = 0;
	float m_AnimationBolendRatio = 0.0f;
	int m_DelayFrame = 0;
	int m_Delay = 1;

	const char* m_ModelData = "�Ȃ�";

	bool m_ChangeState = false;

public:

	Transform3DAnimationComponent() = default;
	Transform3DAnimationComponent(GameObject* obj) : Transform(obj) {}
	Transform3DAnimationComponent(GameObject* obj,const char* date) : Transform(obj) , m_ModelData(date){}

	void AddModelData(const char* data) { 

		m_ModelData = data; 
	    m_TestModel->Load(m_ModelData);

	}
	void AddAnimationData(const char* modeldata,const char* statedata)
	{
		if (m_ModelData != "�Ȃ�") {
			m_TestModel->LoadAnimation(modeldata, statedata);
			m_AnimationList.emplace(statedata, modeldata);

		}
	}

	void SetAnimationState(std::string name)
	{
		if (m_AnimationList.count(name))
		{

			if (name != m_NextAnimationName)
			{

				//��ԕω�
				m_NextAnimationName = name;
				m_AnimationBolendRatio = 0.0f;
				m_ChangeState = true;
				m_AnimationFrame = 0.0f;

			}
		}
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetDelay(int delay) { m_Delay = delay; }
	int GetDelay() const { return m_Delay; }

	AnimationModelTest* GetModel() const { return m_TestModel; }

	
};

