#pragma once
#include "gameObjectComponent.h"
#include "transform3DComponent.h"
#include <tuple>
#include <cmath>
#include <array>
#include <iostream>
#include <algorithm>
#include <limits>
#include <DirectXMath.h>



class Colider : public GameObjectComponent
{
private:

	

protected:
	
	Transform3DComponent* transform = nullptr;

	//�R���C�_�[�̎��
	enum ColiderKind
	{
		BOX,
		CAPSULE,
		SPHERE,
		NONE
	};
	//�J�v�Z���̓����蔻��p�\����
	struct Capsule {
		XMFLOAT3 startPos, endPos;
		float radius;
	};
	//�{�b�N�X�̓����蔻��p�\����
	struct Box
	{

		XMFLOAT3 center;
		XMFLOAT3 halfSize;
		XMFLOAT3 axis[3];

	};
	//�X�t�B�A�̓����蔻��p�\����
	struct Sphere
	{
		XMFLOAT3 center;
		float radius;
	};


	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };//���[���h���W
	XMFLOAT3 m_OldPosition = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 0.0f,0.0f,0.0f };//�����蔻��T�C�Y
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };//��]���

	XMFLOAT3 m_AddPosition = { 0.0f,0.0f,0.0f };

	//�������̃x�N�g��
	XMFLOAT3 m_Velocity = { 0.0f,0.0f,0.f };

	bool m_IsCollision = false;

	ColiderKind m_ColiderKind = NONE;

	float m_Angle = 0.0f;//��
	float m_Fai = 0.0f;//��
	float m_Len = 0.0f;//�^�[�Q�b�g�Ƃ̋���

	bool m_RotateAroundDoOnce = false;

	bool DrawColider = false;

	// ���[�J����ԂŃN�����v
	XMFLOAT3 clampToBox(const XMFLOAT3 point, const XMFLOAT3 halfSize);


	//�{�b�N�X���m�̓����蔻��
	bool BoxsCollision(Box obb1, Box obb2);
	//��̐����̍ŒZ���������߂�֐�
	float SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2);
	//�J�v�Z���ƃJ�v�Z���̓����蔻��p
	bool CapsulesCollision(GameObject* capsuleColider1, GameObject* capsuleColider2);
	// �J�v�Z���ƃ{�b�N�X�̏Փ˔���ƏC��
	bool CapsuleBoxCollision(GameObject* capsuleObj, GameObject* boxObj);

public:

	Colider() { ComponentNumber = 1; }
	virtual ~Colider() {};

	virtual void Init() override {}
	virtual void Uninit() override {}
	virtual void Update() override {}
	virtual void Draw() override {}

	virtual void DrawImGui(){}


	virtual void MoveCollision(){}
	//GameObject* GetGameObject() { return m_GameObject; }

	void OnDraw()
	{
		DrawColider = true;
	}
	const XMFLOAT3 GetPosition() const { return m_Position; }
	const XMFLOAT3 GetOldPosition() const { return m_OldPosition;}
	const XMFLOAT3 GetRotation() const { return m_Rotation; }
	const XMFLOAT3 GetSize() const { return m_Size; }
	XMFLOAT3 GetVelocity() const { return m_Velocity; }

	const ColiderKind GetColiderKind() const { return m_ColiderKind; }

	void SetTransform(const XMFLOAT3 pos, const XMFLOAT3 rot, const XMFLOAT3 size)
	{

		m_Position = pos;
		m_Rotation = rot;
		m_Size = size;

		MoveCollision();
	}

	void SetPosition(const XMFLOAT3 pos) 
	{ 
		m_Position = pos; 
		MoveCollision();
	}
	void SetOldPosition(const XMFLOAT3 pos) { m_OldPosition = pos; }
	void SetSize(const XMFLOAT3 size) 
	{ 
		m_Size = size; 
		MoveCollision();
	}
	void SetRotation(const XMFLOAT3 rot) 
	{
		m_Rotation = rot; 
	}
	void SetVelocity(XMFLOAT3 velocity) { m_Velocity = velocity; }

	void SetAddPosition(const XMFLOAT3 add) 
	{ 
		m_AddPosition = add; 
	}

	void SetRotationAngle(XMFLOAT3 angle)
	{

		//�p�x�����W�A���ɒ���
		float PI = 3.141592;
		angle.x = (PI / 180.0f) * angle.x;
		angle.y = (PI / 180.0f) * angle.y;
		angle.z = (PI / 180.0f) * angle.z;


		m_Rotation = angle;
	}

	void SetIsCollision(bool state) { m_IsCollision = state; }
	bool GetIsCollision() const { return m_IsCollision; }

	virtual std::tuple<bool, GameObject*, std::list<GameObject*>> GetCollision() = 0;

	void RotateAroundInit(XMFLOAT3 targetPos);
	void RotateAround(float rotateX, float rotateY);
	XMFLOAT3 GetRotatePosition(XMFLOAT3 targetPos) const;

	// �x�N�g�����{�b�N�X�̃��[�J�����W�ɕϊ�
	XMFLOAT3 ToLocalSpace(const XMFLOAT3 vec, const Box box) {
		XMFLOAT3 relVec = Sub(vec, box.center);
		return {
			Dot(relVec,box.axis[0]),
			Dot(relVec,box.axis[1]),
			Dot(relVec,box.axis[2])
		};
	}

	// �x�N�g�������[���h���W�ɕϊ�
	XMFLOAT3 ToWorldSpace(const XMFLOAT3& localVec, const Box& box) {
		return Add({
			localVec.x * box.axis[0].x + localVec.y * box.axis[1].x + localVec.z * box.axis[2].x,
			localVec.x * box.axis[0].y + localVec.y * box.axis[1].y + localVec.z * box.axis[2].y,
			localVec.x * box.axis[0].z + localVec.y * box.axis[1].z + localVec.z * box.axis[2].z
			}, box.center);
	}
	
	bool Equal(const XMFLOAT3& a, const XMFLOAT3& b, float epsilon = 0.0001f)
	{
		return (fabs(a.x - b.x) < epsilon) &&
			(fabs(a.y - b.y) < epsilon) &&
			(fabs(a.z - b.z) < epsilon);
	}
	bool IsPointInsideBox(const XMFLOAT3& point, const XMFLOAT3& halfSize) {
		return fabs(point.x) <= halfSize.x &&
			fabs(point.y) <= halfSize.y &&
			fabs(point.z) <= halfSize.z;
	}

};

