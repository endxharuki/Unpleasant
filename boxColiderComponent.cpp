#include "main.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "capsuleColiderComponent.h"

/*
座標A = 動かす前の元の座標
座標B = 動かした後の座標
/*
* AはPosition
x軸回転
Bx = Ax * 1 + Ay * 0 + Az * 0,
By = Ax * 0 + Ay * cosβ + Az * sinβ
Bz = Ax * 0 + Ay * -sinβ + Az * cosβ
y軸回転
Bx = Ax * cosβ + Ay * 0 + Az * sinβ,
By = Ax * 0 + Ay * 1 + Az * 0
Bz = Ax * -sinβ + Ay * 0 + Az * cosβ
z軸回転
Bx = Ax * cosβ + Ay * sinβ + Az * 0,
By = Ax * -sinβ + Ay * cosβ + Az * 0
Bz = Ax * 0 + Ay * 0 + Az * 1
*/

void BoxColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_ColiderKind = BOX;

	transform = new Transform3DComponent();

	transform->Init();
	transform->AddModelData("asset\\Model\\boxTest.obj");
}

void BoxColiderComponent::Uninit()
{
	transform->Uninit();
	delete transform;
}

void BoxColiderComponent::Update()
{
	m_Position = m_GameObject->GetComponent<Transform>()->GetPosition();
	/*m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();*/
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_Position = Add(m_Position, m_AddPosition);

	
	if (DrawColider == true)
	{
		XMFLOAT3 size = { m_Size.x * 2,m_Size.y * 2,m_Size.z * 2 };
		transform->SetSize(size);
		transform->SetPosition(m_Position);
		transform->SetRotation(m_Rotation);

		transform->Update();
	}

	MoveCollision();
	if (m_GameObject->GetObjectKind() == Dynamic)
	{
	   
		std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetCollision();

		m_IsCollision = std::get<0>(list);

	}

}

void BoxColiderComponent::Draw()
{

	ID3D11PixelShader* layer = m_GameObject->GetComponent<Transform>()->GetPixelShader();

	Renderer::GetDeviceContext()->PSSetShader(layer, nullptr, 0);

	ID3D11RasterizerState* wireframeState = nullptr;

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // ワイヤーフレームモード
	rasterDesc.CullMode = D3D11_CULL_NONE;      // カリングなし（両面描画）
	rasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);
	Renderer::GetDeviceContext()->RSSetState(wireframeState); // ラスタライザーステートを設定

	if (DrawColider == true)
	{
		transform->Draw();
	}

	Renderer::GetDeviceContext()->PSSetShader(layer, nullptr, 0);

	ID3D11RasterizerState* frameState = nullptr;

	D3D11_RASTERIZER_DESC framerasterDesc = {};
	framerasterDesc.FillMode = D3D11_FILL_SOLID; // 通常モード
	framerasterDesc.CullMode = D3D11_CULL_BACK;      // カリングなし（両面描画）
	framerasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&framerasterDesc, &frameState);
	Renderer::GetDeviceContext()->RSSetState(frameState); // ラスタライザーステートを設定

}

void BoxColiderComponent::MoveCollision()
{

	XMFLOAT3 vector = { 1.0f,1.0f,1.0f };
	Angle angle;
	angle = GetAddAngle(m_Rotation, vector);

	m_OBB =
	{
		m_Position,
		{m_Size.x * 2,m_Size.y * 2,m_Size.z * 2},
		{angle.AngleX,angle.AngleY,angle.AngleZ}

	};

}


//あった判定と最初に当たったオブジェクトと当たっているオブジェクトリストを返します
std::tuple<bool, GameObject*, std::list<GameObject*>> BoxColiderComponent::GetCollision()
{

	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//自分以外のコライダーのポジションとサイズ
	XMFLOAT3 pos;
	XMFLOAT3 size;
	XMFLOAT3 rotation;

	

	for (int i = 0; i < 2; i++) {

		if (m_GameObject->GetObjectKind() == Static) break;
		std::list<GameObject*> gameObjectList = Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(i);

		for (auto obj : gameObjectList)
		{
			if (obj->GetComponent<Colider>() == nullptr) {
				continue;
			}
			if (GetGameObject() == obj) {
				continue;
			}

			
			if (obj->GetComponent<Colider>()->GetColiderKind() == BOX)
			{
				pos = obj->GetComponent<Colider>()->GetPosition();
				size = obj->GetComponent<Colider>()->GetSize();
				rotation = obj->GetComponent<Colider>()->GetRotation();

				Box otherBox = obj->GetComponent<BoxColiderComponent>()->GetOBB();
				if (BoxsCollision(m_OBB, otherBox))
				{

					objList.push_back(obj);
					objSize = objList.size();

				}
			}
			else if (obj->GetComponent<Colider>()->GetColiderKind() == CAPSULE)
			{

				if (CapsuleBoxCollision(obj, m_GameObject))
				{
					objList.push_back(obj);
					objSize = objList.size();
				}
			}
			
		}
	}
	if (objSize != 0)
	{
		auto itr = objList.begin();
		GameObject* gameObject = (*itr);

		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(true, gameObject, objList);
		return OnCollisionObject;
	}
	else if (objSize == 0)
	{

		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(false, nullptr, objList);
		return OnCollisionObject;

	}

}


//bool BoxColiderComponent::BoxsCollision(Box obb1, Box obb2)
//{
//	
//	XMFLOAT3 distance = { obb2.center.x - obb1.center.x,obb2.center.y - obb1.center.y,obb2.center.z - obb1.center.z };
//
//
//	for (int i = 0; i < 3; i++)
//	{
//
//
//		float r1 = obb1.halfSize.x * std::fabs(Dot(obb1.axis[i], obb1.axis[0])) +
//			obb1.halfSize.y * std::fabs(Dot(obb1.axis[i], obb1.axis[1])) +
//			obb1.halfSize.z * std::fabs(Dot(obb1.axis[i], obb1.axis[2]));
//
//		float r2 = obb2.halfSize.x * std::fabs(Dot(obb1.axis[i], obb2.axis[0])) +
//			obb2.halfSize.y * std::fabs(Dot(obb1.axis[i], obb2.axis[1])) +
//			obb2.halfSize.z * std::fabs(Dot(obb1.axis[i], obb2.axis[2]));
//
//		float distanceOnAxis = std::fabs(Dot(distance, obb1.axis[i]));
//
//		if (distanceOnAxis > r1 + r2)
//		{
//			return false;
//		}
//	}
//
//	for (int i = 0; i < 3; i++)
//	{
//
//		XMFLOAT3 distance = { obb2.center.x - obb1.center.x,obb2.center.y - obb1.center.y,obb2.center.z - obb1.center.z };
//
//		float r1 = obb1.halfSize.x * std::fabs(Dot(obb2.axis[i], obb1.axis[0])) +
//			obb1.halfSize.y * std::fabs(Dot(obb2.axis[i], obb1.axis[1])) +
//			obb1.halfSize.z * std::fabs(Dot(obb2.axis[i], obb1.axis[2]));
//
//		float r2 = obb2.halfSize.x * std::fabs(Dot(obb2.axis[i], obb2.axis[0])) +
//			obb2.halfSize.y * std::fabs(Dot(obb2.axis[i], obb2.axis[1])) +
//			obb2.halfSize.z * std::fabs(Dot(obb2.axis[i], obb2.axis[2]));
//
//		float distanceOnAxis = std::fabs(Dot(distance, obb2.axis[i]));
//
//		if (distanceOnAxis > r1 + r2)
//		{
//			return false;
//		}
//	}
//
//	return true;
//
//	
//}
