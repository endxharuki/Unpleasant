#include "main.h"
#include "sphereColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "boxColiderComponent.h"
#include "titleScene.h"

void SphereColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_ColiderKind = SPHERE;

	m_Sphere =
	{
		m_Position,
		m_Size.x
	};

}

void SphereColiderComponent::Uninit()
{
}

void SphereColiderComponent::Update()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	/*m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();*/
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();
	m_Velocity = GetGameObject()->GetComponent<Transform>()->GetVelocity();

	//�R���C�_�[�̃|�W�V���������̂���
	m_Position = Add(m_Position, m_AddPosition);

	m_Sphere =
	{
		m_Position,
		m_Size.x
	};

	GetCollision();
}

void SphereColiderComponent::Draw()
{
}

bool SphereColiderComponent::SphereOBBCollision(const Box box, Sphere sphere, XMFLOAT3& collisionpoint)
{

	const int MaxCount = 10;
	int count = 0;
	bool collisionResolved = false;
	bool DoOnce = false;

	while (count < MaxCount)
	{

		XMFLOAT3 localSpherePos = ToLocalSpace(sphere.center, box);

		// ���[�J����Ԃł̍ŋߓ_���v�Z�iOBB�ɃN�����v�j
		XMFLOAT3 closestPoint =
		{
			std::clamp(localSpherePos.x,-box.halfSize.x,box.halfSize.x),
			std::clamp(localSpherePos.y,-box.halfSize.y,box.halfSize.y),
			std::clamp(localSpherePos.z,-box.halfSize.z,box.halfSize.z)

		};

		// �ŋߓ_�����[���h��Ԃɕϊ�
		collisionpoint = ToWorldSpace(closestPoint, box);

		// ���̂̒��S�ƍŋߓ_�̋������v�Z
		XMFLOAT3 length = Sub(localSpherePos, closestPoint);
		float distanceSquared = LengthSquared(length);

		// �Փ˔���F�����̕��������̂̔��a�̕����ȉ��ł���ΏՓ�
		if (distanceSquared > sphere.radius * sphere.radius)
		{
			return count > 0;
		}

		XMFLOAT3 penetrationDepths;

		// ���̂��{�b�N�X�����Ɋ��S�ɖ��܂��Ă���ꍇ�̖@���v�Z
		XMFLOAT3 localNormal;
		if (distanceSquared < 1e-4f) {
			penetrationDepths = {
				box.halfSize.x - std::abs(localSpherePos.x),
				box.halfSize.y - std::abs(localSpherePos.y),
				box.halfSize.z - std::abs(localSpherePos.z)
			};

			if (penetrationDepths.x <= penetrationDepths.y && penetrationDepths.x <= penetrationDepths.z) {
				localNormal = { localSpherePos.x < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f };
			}
			else if (penetrationDepths.y <= penetrationDepths.z) {
				localNormal = { 0.0f, localSpherePos.y < 0.0f ? -1.0f : 1.0f, 0.0f };
			}
			else {
				localNormal = { 0.0f, 0.0f, localSpherePos.z < 0.0f ? -1.0f : 1.0f };
			}
		}
		else {
			localNormal = Normalize(length);
		}

		XMFLOAT3 worldNormal = Sub(ToWorldSpace(localNormal, box), box.center);
		worldNormal = Normalize(worldNormal);


		//���܂��Ă���[���̎n�_�ƏI�_
		XMFLOAT3 penetrationStart = ToWorldSpace(closestPoint, box);
		//XMFLOAT3 penetrationEnd = Add(penetrationStart,MulFloat(worldNormal,sphere.radius));

		XMFLOAT3 penetrationEnd = sphere.center;

		float penetrationDepth = sphere.radius - sqrt(distanceSquared);

		//���˂�����
		if (DoOnce == false)
		{
			ReflectSphere(sphere, localNormal);
			DoOnce = true;
		}

		if (penetrationDepth < 1e-4f)
		{
			return count > 0;
		}

		/*XMFLOAT3 penetrationVector = { normal.x * penetrationDepth ,normal.y * penetrationDepth ,normal.z * penetrationDepth };

		penetrationVector = MulFloat(penetrationVector, 1.1f);*/

		m_Position = Add(m_Position,MulFloat(worldNormal,penetrationDepth));
		/*m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);*/
		sphere.center = m_Position;
		collisionResolved = true;

		count++;
	}

	return collisionResolved;
}

void SphereColiderComponent::ReflectSphere(Sphere sphere, const XMFLOAT3 normal)
{

	XMFLOAT3 a = Mul(m_Velocity, normal);

	//���˃x�N�g���̌��� ���˃x�N�g�� = ���݂̑��x - 2 * (���݂̑��x * �@��) * �@��
	m_Velocity = Sub(m_Velocity,MulFloat(Mul(a,normal),2.0f));
	m_GameObject->GetComponent<Transform>()->SetVelocity(m_Velocity);

}

//����������ƍŏ��ɓ��������I�u�W�F�N�g�Ɠ������Ă���I�u�W�F�N�g���X�g��Ԃ��܂�
std::tuple<bool, GameObject*, std::list<GameObject*>> SphereColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//�����ȊO�̃R���C�_�[�̃|�W�V�����ƃT�C�Y
	XMFLOAT3 pos;
	XMFLOAT3 size;

	std::list<GameObject*> list;
	if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
	{
		list = Scene::GetInstance()->GetScene<TitleScene>()->GetGameObjectList(1);
	}
	else if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == GAME)
	{
		list = Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1);
	}


	for (auto obj : list)
	{
		if (obj->GetComponent<Colider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}

		pos = obj->GetComponent<Colider>()->GetPosition();
		size = obj->GetComponent<Colider>()->GetSize();
		
		if (obj->GetComponent<Colider>()->GetColiderKind() == BOX)
		{
			Box obb = obj->GetComponent<BoxColiderComponent>()->GetOBB();

			XMFLOAT3 point;

			if (SphereOBBCollision(obb, m_Sphere, point))
			{
				
				objList.push_back(obj);
				objSize = objList.size();
				
			}
		}

		
		
	}
	
	if (objSize != 0) 
	{
		auto itr =objList.begin();
		GameObject* gameObject = (*itr);
	
		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(true,gameObject,objList);
		return OnCollisionObject;
	}
	else if (objSize == 0) 
	{

		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(false, nullptr, objList);
		return OnCollisionObject;

	}

}

