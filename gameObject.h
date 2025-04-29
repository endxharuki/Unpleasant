#pragma once
//�Q�[���V�[�����C���N���[�h������ʖڂ�
#include <string>
#include <list>
#include "sceneState.h"
#include "colider.h"
#include <iostream>
#include <tuple>
#include <string>
#include "renderer.h"
#include "modelRenderer.h"
#include "imguiManager.h"

enum GameObjectKind
{
	Static,
	Dynamic
};

class GameObject
{
private:
	

protected:

	//���ݏo�����e�̃Q�[���I�u�W�F�N�g
	GameObject* m_ParentObject = nullptr;

	struct States
	{
		float HP;
		float MP;
		bool hit;
		int hitCount;
	};

	//�Q�[���I�u�W�F�N�g�̎��(�ÓI�����I��)
	GameObjectKind m_GameObjectKind = Static;
	//���I�Q�[���I�u�W�F�N�g�̏d��
	int m_Weight = 0;


	SceneState* InScene = nullptr;
	std::list<Component*> ComponentList; //���̃Q�[���I�u�W�F�N�g�ɂ������Ă���R���|�[�l���g�B
	bool isDelete = false;
	std::string Tag = "";

	float m_GravityScale = 0.05f;
	bool m_IsGravity = true;

	States m_States =
	{
		0.0f,
		0.0f,
		false,
		0
	};

	bool m_HitStop = false;
	float m_HitStopSecond = 0.0f;

public:

	GameObject() {}
	~GameObject() {};
	bool DeleteRecest() {

		return isDelete;
	}
	void SetDelete(bool del)
	{
		isDelete = del;
	}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;

	SceneState* GetScene() { return InScene; }
	void SetScene(SceneState* scene) { InScene = scene; }
	std::string GetTag() { return Tag; }
	GameObjectKind GetObjectKind() const { return m_GameObjectKind; }
	int GetWeight() const { return m_Weight; }

	std::list<Component*> GetComponentList() { return ComponentList; }

	void ComponentListSort()
	{
		ComponentList.sort([](const Component* a, const Component* b)
			{
				return a->GetComponentNumber() < b->GetComponentNumber();
			}
		);
	}

	template <class T>
	T* AddComponent()
	{
		T* comp = new T;
		//�e�̃Q�[���I�u�W�F�N�g��ǉ�
		comp->AcceptGameObject(this);
		ComponentList.push_back(comp);
		comp->Init();

		return comp;
	}

	template <class T>
	T* GetComponent()
	{
		for (auto com : ComponentList) {
			T* ret = dynamic_cast<T*>(com);
			if (ret != nullptr)
			{
				return ret;
			}

		}
		return nullptr;
	}
	
	void DeleteComponent()
	{
		ComponentList.remove_if([](Component* com) {

			if (com->DeleteRecest() == true)
			{
				com->Uninit();
				delete com;
				return true;
			}
			return false;
		
			}
		);
	}

	States* GetStates()
	{
		return &m_States;
	}
	void SetStates(float hp,float mp,bool hit,int count)
	{
		m_States = 
		{
			hp,
			mp,
			hit,
			count
		};
	}

	void SetGravitiScale(float scale) { m_GravityScale = scale; }

	void SetHit(bool hit) { m_States.hit = hit; }
	void SetHitCount(int count) { m_States.hitCount = count; }
	void SetHp(float hp){ m_States.HP = hp; }
	void SetMp(float mp) { m_States.MP = mp; }
	
	void SetGravityState(bool state) { m_IsGravity = state; }

	//�f�o�b�O�p
	virtual void DrawImGui() {};

	void SetHitStopState(bool state) { m_HitStop = state; }
	bool GetHitStopState() const { return m_HitStop; }

	void AddParentGameObject(GameObject* obj) { m_ParentObject = obj; }


};
