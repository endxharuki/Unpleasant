#include "main.h"
#include "capsuleColiderComponent.h"
#include "gameScene.h"
#include "scene.h"
#include "transform2DComponent.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"


/*
���WA = �������O�̌��̍��W
���WB = ����������̍��W
/*
* A��Position
x����]
Bx = Ax * 1 + Ay * 0 + Az * 0,
By = Ax * 0 + Ay * cos�� + Az * sin��
Bz = Ax * 0 + Ay * -sin�� + Az * cos��
y����]
Bx = Ax * cos�� + Ay * 0 + Az * sin��,
By = Ax * 0 + Ay * 1 + Az * 0
Bz = Ax * -sin�� + Ay * 0 + Az * cos��
z����]
Bx = Ax * cos�� + Ay * sin�� + Az * 0,
By = Ax * -sin�� + Ay * cos�� + Az * 0
Bz = Ax * 0 + Ay * 0 + Az * 1
*/

void CapsuleColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_ColiderKind = CAPSULE;

	transform = new Transform3DComponent();

	transform->Init();
	transform->AddModelData("asset\\Model\\capsule.obj");

}

void CapsuleColiderComponent::Uninit()
{
	transform->Uninit();
	delete transform;
}

void CapsuleColiderComponent::Update()
{

	////�������W�̉�]�s��
	//{
	//	Angle angle;
	//	// �ŏ��@0,m_CenterPosLength,0�ɂȂ邭��
	//	//m_Position�������Ηǂ�
	//	XMFLOAT3 vec;
	//	XMFLOAT3 topVec;
	//	XMFLOAT3 bottomVec;

	//	
	//	vec = { 0.0f,1.0f,0.0f };
	//	angle = GetAddAngle(m_Rotation, vec);

	//	topVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
	//	topVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
	//	topVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	//	m_StartPosition.x = topVec.x + m_Position.x;
	//	m_StartPosition.y = topVec.y + m_Position.y;
	//	m_StartPosition.z = topVec.z + m_Position.z;

	//	vec = { 0.0f,-1.0f,0.0f };

	//	angle = GetAddAngle(m_Rotation, vec);

	//	bottomVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
	//	bottomVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
	//	bottomVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	//	m_EndPosition.x = bottomVec.x + m_Position.x;
	//	m_EndPosition.y = bottomVec.y + m_Position.y;
	//	m_EndPosition.z = bottomVec.z + m_Position.z;


	//}
	if (update == true) {
		
		m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		/*m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();*/
		m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

		if (m_GameObject->GetObjectKind() == Dynamic)
		{
			
			//�����蔻�菈��
			std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetCollision();

			m_IsCollision = std::get<0>(list);
		}
	}

	MoveCollision();

	if (DrawColider == true)
	{

		transform->SetSize(XMFLOAT3(m_Size.x, m_CenterPosLngth, m_Size.z));
		transform->SetPosition(m_Position);
		transform->SetRotation(m_Rotation);

		transform->Update();
	}
	/*m_Cupsule =
	{
		m_StartPosition,
		m_EndPosition,
		m_Size.x
	};*/

	/*rotatePosition.x = (m_CenterPosLngth * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
    rotatePosition.y = (m_CenterPosLngth * cos(m_Fai)) + targetPos.y;
    rotatePosition.z = (m_CenterPosLngth * sin(m_Fai) * sin(m_Angle)) + targetPos.z;*/

}

void CapsuleColiderComponent::Draw()
{

	ID3D11PixelShader* layer = m_GameObject->GetComponent<Transform>()->GetPixelShader();

	Renderer::GetDeviceContext()->PSSetShader(layer, nullptr, 0);

	ID3D11RasterizerState* wireframeState = nullptr;

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // ���C���[�t���[�����[�h
	rasterDesc.CullMode = D3D11_CULL_NONE;      // �J�����O�Ȃ��i���ʕ`��j
	rasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);
	Renderer::GetDeviceContext()->RSSetState(wireframeState); // ���X�^���C�U�[�X�e�[�g��ݒ�

	if (DrawColider == true)
	{
		transform->Draw();
	}
	DrawImGui();

	Renderer::GetDeviceContext()->PSSetShader(layer, nullptr, 0);

	ID3D11RasterizerState* frameState = nullptr;

	D3D11_RASTERIZER_DESC framerasterDesc = {};
	framerasterDesc.FillMode = D3D11_FILL_SOLID; // �ʏ탂�[�h
	framerasterDesc.CullMode = D3D11_CULL_BACK;      // �J�����O�Ȃ��i���ʕ`��j
	framerasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&framerasterDesc, &frameState);
	Renderer::GetDeviceContext()->RSSetState(frameState); // ���X�^���C�U�[�X�e�[�g��ݒ�

}


//�J�v�Z���{�b�N�X
float boxCupsuleLengthStart = 0.0f;
float boxCupsuleLengthEnd = 0.0f;
XMFLOAT3 end;
XMFLOAT3 start;

void CapsuleColiderComponent::DrawImGui()
{

	{
		if (onImGui == true) {
			ImGui::Begin("CapsuleToCapsule");                          // Create a window called "Hello, world!" and append into it.


			ImGui::Text("CupsulePos , x = %.1f , y = %.1f , z = %.1f", m_Position.x, m_Position.y, m_Position.z);
			ImGui::Text("StartPoint , x = %.1f , y = %.1f , z = %.1f", m_Capsule.startPos.x, m_Capsule.startPos.y, m_Capsule.startPos.z);
			ImGui::Text("EndPoint , x = %.1f , y = %.1f , z = %.1f", m_Capsule.endPos.x, m_Capsule.endPos.y, m_Capsule.endPos.z);

			ImGui::Text("ClossPoint1 ,x = %.1f , y = %.1f , z = %.1f", clossPoint1.x, clossPoint1.y, clossPoint1.z);
			ImGui::Text("ClossPoint2 ,x = %.1f , y = %.1f , z = %.1f", clossPoint2.x, clossPoint2.y, clossPoint2.z);

			ImGui::Text("Lenght = %.1f", m_length);

			ImGui::End();

			ImGui::Begin("CapsuleToBox");

			ImGui::Text("LenghtStartPoint = %.1f", boxCupsuleLengthStart);
			ImGui::Text("LenghtEndPoint = %.1f", boxCupsuleLengthEnd);

			ImGui::Text("ClosestStart , x = %.1f , y = %.1f , z = %.1f", start.x, start.y, start.z);
			ImGui::Text("ClosestEnd , x = %.1f , y = %.1f , z = %.1f", end.x, end.y, end.z);


			ImGui::End();

		}
	}


}

void CapsuleColiderComponent::MoveCollision()
{

	m_Position = Add(m_Position, m_AddPosition);

	//�������W�̉�]�s��
	{
		Angle angle;
		// �ŏ��@0,m_CenterPosLength,0�ɂȂ邭��
		//m_Position�������Ηǂ�
		XMFLOAT3 vec;
		XMFLOAT3 topVec;
		XMFLOAT3 bottomVec;


		vec = { 0.0f,1.0f,0.0f };
		angle = GetAddAngle(m_Rotation, vec);

		topVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
		topVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
		topVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

		topVec = MulFloat(topVec, m_CenterPosLngth);

		m_Capsule.startPos.x = topVec.x + m_Position.x;
		m_Capsule.startPos.y = topVec.y + m_Position.y;
		m_Capsule.startPos.z = topVec.z + m_Position.z;

		vec = { 0.0f,-1.0f,0.0f };

		angle = GetAddAngle(m_Rotation, vec);

		bottomVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
		bottomVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
		bottomVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

		bottomVec = MulFloat(bottomVec, m_CenterPosLngth);

		m_Capsule.endPos.x = bottomVec.x + m_Position.x;
		m_Capsule.endPos.y = bottomVec.y + m_Position.y;
		m_Capsule.endPos.z = bottomVec.z + m_Position.z;


	}

	m_Capsule.radius = m_Size.x;

}


//float CapsuleColiderComponent::SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2)
//{
//	XMFLOAT3 d1 = { vec1end.x - vec1start.x, vec1end.y - vec1start.y ,vec1end.z - vec1start.z };
//	XMFLOAT3 d2 = { vec2end.x - vec2start.x, vec2end.y - vec2start.y ,vec2end.z - vec2start.z };
//	XMFLOAT3 r = { vec1start.x - vec2start.x, vec1start.y - vec2start.y ,vec1start.z - vec2start.z };
//
//
//	float a = Dot(d1, d1); //����1�̒�����2��
//	float e = Dot(d2, d2); //����2�̒�����2��
//	//float b = Dot(d1, d2);
//	//float c = Dot(d1, r);
//	float f = Dot(d2, r);
//
//	
//	float s = 0.0f, t = 0.0f;
//	//float denom = a * e - b * b;
//	/*if (std::abs(denom) > 1e-6f)
//	{
//		s = (b * f - c * e) / denom;
//		t = (a * f - b * c) / denom;
//	}*/
//
//	if (a > 1e-6f && e > 1e-6)//�l�����������Ȃ�������
//	{
//		float b = Dot(d1, d2);
//		float c = Dot(d1, r);
//		float denom = a * e - b * b;
//
//		if (std::abs(denom) > 1e-6f)//�������m�����s����Ȃ�
//		{
//			
//			s = (b * f - c * e) / denom;
//			t = (a * f - b * c) / denom;
//		}
//
//		s = std::clamp(s, 0.0f, 1.0f);
//		t = std::clamp(t, 0.0f, 1.0f);
//	}
//
//	t1 = s;
//	t2 = t;
//
//	XMFLOAT3 closestPoint1 = { d1.x * s,d1.y * s,d1.z * s};
//	closestPoint1 = { vec1start.x + closestPoint1.x,vec1start.y + closestPoint1.y ,vec1start.z + closestPoint1.z };
//
//	XMFLOAT3 closestPoint2 = { d2.x * t,d2.y * t,d2.z * t };
//	closestPoint2 = { vec2start.x + closestPoint2.x, vec2start.y + closestPoint2.y ,vec2start.z + closestPoint2.z };
//
//	//clossPoint1 = closestPoint1;
//	//clossPoint2 = closestPoint2;
//
//	XMFLOAT3 length = { closestPoint1.x - closestPoint2.x,closestPoint1.y - closestPoint2.y ,closestPoint1.z - closestPoint2.z };
//
//	return LengthSquared(length);
//
//}
//
//bool CapsuleColiderComponent::CapsulesCollision(GameObject* capsuleColider1, GameObject* capsuleColider2)
//{
//	Capsule capsule1 = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule();
//	Capsule capsule2 = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule();
//
//
//	float t1, t2;
//	float distanceSquared = SegmentDistance(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, t1, t2);
//
//	float radiusSum = capsule1.radius + capsule2.radius;
//
//	if (distanceSquared <= radiusSum * radiusSum)
//	{
//		if (capsuleColider1->GetTag() == "Attack") return true;
//		if (capsuleColider2->GetTag() == "Attack") return true;
//
//		XMFLOAT3 cap1Len = { capsule1.endPos.x - capsule1.startPos.x, capsule1.endPos.y - capsule1.startPos.y ,capsule1.endPos.z - capsule1.startPos.z };
//		XMFLOAT3 cap2Len = { capsule2.endPos.x - capsule2.startPos.x, capsule2.endPos.y - capsule2.startPos.y ,capsule2.endPos.z - capsule2.startPos.z };
//
//		cap1Len = { cap1Len.x * t1,cap1Len.y * t1 ,cap1Len.z * t1 };
//		cap1Len = { cap2Len.x * t2,cap2Len.y * t2 ,cap2Len.z * t2 };
//
//
//		// �Փˎ��̍ŋߓ_�Ɩ@���v�Z
//		XMFLOAT3 point1 = { capsule1.startPos.x + cap1Len.x,capsule1.startPos.y + cap1Len.y ,capsule1.startPos.z + cap1Len.z };
//		XMFLOAT3 point2 = { capsule2.startPos.x + cap2Len.x,capsule2.startPos.y + cap2Len.y ,capsule2.startPos.z + cap2Len.z };
//
//		XMFLOAT3 pointLen = { point1.x - point2.x,point1.y - point2.y ,point1.z - point2.z };
//
//		XMFLOAT3 normal = Normalize(pointLen);
//
//		//���܂��Ă���[�����v�Z
//		float penetrationDepth = radiusSum - sqrt(distanceSquared);
//
//		XMFLOAT3 obj1Pos = capsuleColider1->GetComponent<Colider>()->GetPosition();
//		XMFLOAT3 obj2Pos = capsuleColider2->GetComponent<Colider>()->GetPosition();
//
//		XMFLOAT3 vectorObjects = { obj1Pos.x - obj2Pos.x,0.0f ,obj1Pos.z - obj2Pos.z };
//
//		vectorObjects = Normalize(vectorObjects);
//
//		//�C���x�N�g��
//		XMFLOAT3 correction = {vectorObjects.x * penetrationDepth,vectorObjects.y * penetrationDepth,vectorObjects.z * penetrationDepth};
//		/*XMFLOAT3 correction = MulFloat(normal, penetrationDepth);*/
//		if (Length(correction) <= 0.00001f)
//		{
//			return false;
//		}
//
//
//		//�ʒu��␳
//		//���������I�u�W�F�N�g���ÓI��������
//		if (capsuleColider2->GetObjectKind() == Static)
//		{
//			XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//			XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//			m_Capsule.startPos = Add(myStartPos, correction);
//			m_Capsule.endPos = Add(myEndPos, correction);
//			m_Position = Add(m_Position, correction);
//		
//			m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//		}
//		//���������I�u�W�F�N�g�����I��������
//		else if (capsuleColider2->GetObjectKind() == Dynamic)
//		{
//
//			int weight1 = capsuleColider1->GetWeight();
//			int weight2 = capsuleColider2->GetWeight();
//
//
//			//���������I�u�W�F�N�g�̏d�������������傫��������
//			if (weight2 > weight1)
//			{
//				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//				m_Capsule.startPos = Add(myStartPos, correction);
//				m_Capsule.endPos = Add(myEndPos, correction);
//				m_Position = Add(m_Position, correction);
//				
//				m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//			}
//			//���������I�u�W�F�N�g�̏d������������������������
//			else if (weight2 < weight1)
//			{
//				XMFLOAT3 otherPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetPosition();
//				XMFLOAT3 otherStartPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 otherEndPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//				CapsuleColiderComponent* col = capsuleColider2->GetComponent<CapsuleColiderComponent>();
//
//
//				col->SetCapsuleStartPos(Sub(otherStartPos, correction));
//				col->SetCapsuleStartPos(Sub(otherEndPos, correction));
//
//				XMFLOAT3 pos = Sub(otherPos, correction);
//				col->SetPosition(pos);
//				capsuleColider2->GetComponent<Transform>()->SetPosition(pos);
//			}
//			//���������I�u�W�F�N�g�̏d���������Ɠ�����������
//			else if (weight2 == weight1)
//			{
//				//�����𓮂���
//				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//				correction = MulFloat(correction, 0.5f);
//
//				m_Capsule.startPos = Add(myStartPos, correction);
//				m_Capsule.endPos = Add(myEndPos, correction);
//				m_Position = Add(m_Position, correction);
//
//				m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//				//����𓮂���
//				XMFLOAT3 otherPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetPosition();
//				XMFLOAT3 otherStartPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 otherEndPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//				CapsuleColiderComponent* col = capsuleColider2->GetComponent<CapsuleColiderComponent>();
//
//
//				col->SetCapsuleStartPos(Sub(otherStartPos, correction));
//				col->SetCapsuleStartPos(Sub(otherEndPos, correction));
//
//				XMFLOAT3 pos = Sub(otherPos, correction);
//				col->SetPosition(pos);
//				capsuleColider2->GetComponent<Transform>()->SetPosition(pos);
//			}
//		}
//
//
//		return true;
//
//	}
//
//	return false;
//
//}
//
////OBB�̓����蔻��
//bool CapsuleColiderComponent::CapsuleBoxCollision(Capsule& capsule, GameObject* obj, XMFLOAT3& moveDir)
//{
//
//	const float MIN = 0.00001f; //��������l
//	const int MAX_COUNT = 5;//�ő又����
//
//	Box box = obj->GetComponent<BoxColiderComponent>()->GetOBB();
//	
//	Angle angle;
//	
//	int count = 0;
//	while (count < MAX_COUNT)
//	{
//		// �J�v�Z���̃��[�J�����W�ϊ�
//		XMFLOAT3 localStart = ToLocalSpace(capsule.startPos, box);
//		XMFLOAT3 localEnd = ToLocalSpace(capsule.endPos, box);
//
//		// �{�b�N�X�͈͓̔��ɃN�����v
//		XMFLOAT3 clampedStart = clampToBox(localStart, box.halfSize);
//		XMFLOAT3 clampedEnd = clampToBox(localEnd, box.halfSize);
//
//		// �Փ˔���i�n�_�܂��͏I�_�����܂��Ă���ꍇ�j
//		float distStartSquared = Dot(Sub(localStart, clampedStart), Sub(localStart, clampedStart));
//		float distEndSquared = Dot(Sub(localEnd, clampedEnd), Sub(localEnd, clampedEnd));
//
//		//�Փ˔���
//		if (distStartSquared > capsule.radius * capsule.radius && distEndSquared > capsule.radius * capsule.radius) {
//			return count > 0; // ��x�ł�����������true��Ԃ�
//		}
//
//		//��Ԑ[���Ƃ���̃|�W�V����
//		XMFLOAT3 DeepPoint;
//
//		//���܂����[���̐����̎n�_�ƏI�_
//		XMFLOAT3 penetrationStart;
//		XMFLOAT3 penetrationEnd;
//
//
//		if (distStartSquared > distEndSquared)
//		{
//			DeepPoint = clampedEnd;// �ł��[�����܂����{�b�N�X���̓_
//			penetrationStart = localEnd;// �J�v�Z�����̓_�i�I�_�j
//			penetrationEnd = clampedEnd;// �{�b�N�X���̓_
//		}
//		else
//		{
//			DeepPoint = clampedStart;// �ł��[�����܂����{�b�N�X���̓_
//			penetrationStart = localStart;// �J�v�Z�����̓_�i�n�_�j
//			penetrationEnd = clampedStart;// �{�b�N�X���̓_
//		}
//
//		XMFLOAT3 penetrationVector = Sub(penetrationStart, penetrationEnd);
//
//
//		// ���܂�[�����v�Z
//		float penetrationDepth = capsule.radius - Length(penetrationVector);
//		if (penetrationDepth <= MIN) {
//			break; // ���܂肪�Ȃ��ꍇ�������I��
//		}
//
//		//�J�v�Z���̖@��
//		XMFLOAT3 localNormal = Normalize(penetrationVector);
//		//���������{�b�N�X�̖ʂ̖@��
//		XMFLOAT3 worldNormal = Sub(ToWorldSpace(localNormal, box), box.center);
//
//		XMFLOAT3 correction = MulFloat(Normalize(worldNormal), penetrationDepth);
//
//		DeepPoint = ToWorldSpace(DeepPoint, box);
//		penetrationStart = ToWorldSpace(penetrationStart, box);
//		penetrationEnd = ToWorldSpace(penetrationEnd, box);
//
//		float PI = 3.14159265359 / 2;
//		/*(PI / 180.0f)* angle.x;*/
//		XMFLOAT3 t = { 0.0f,1.0f,0.0f };
//
//		float DotProdct = Dot(t, worldNormal);
//		float len = Length(t) * Length(worldNormal);
//
//		float cosineTheta = DotProdct / len;
//
//		cosineTheta = std::fmax(-1.0f, std::fmin(1.0f, cosineTheta));
//
//		//��̃x�N�g���̂Ȃ��p
//		float radian = std::acos(cosineTheta);
//
//		XMVECTOR deep = { DeepPoint.x,DeepPoint.y,DeepPoint.z,0.0f };
//		XMVECTOR cE = { clampedEnd.x,clampedEnd.y ,clampedEnd.z ,0.0f };
//
//		//�@���Ɛ^��̃x�N�g���̂Ȃ��p��50�x��菬����������
//		if (radian <= 50.0f * (3.14159265359 / 180.0f) && penetrationStart.y <= capsule.endPos.y + m_Size.x)
//		{
//
//			float a = 90.0f;
//			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
//			float c = 180.0f - (a + b);
//
//			//���a
//			float r = (penetrationDepth / sinf(a)) / 2;
//
//			float A = (2 * r) * sinf(a);//a�̊p�̑΂̕�
//			float B = (2 * r) * sinf(b);//b�̊p�̑΂̕�
//			float C = (2 * r) * sinf(c);//���킩���Ă��钷��
//
//			float baiA = sqrt((B * B) + (C * C));
//
//			float ratioA = A / C;
//
//			float reA = ratioA * penetrationDepth;
//
//			//�i��ł�����̋t�x�N�g��
//			XMFLOAT3 reverseVelocity = { 0.0f,worldNormal.y,0.0f };
//			XMFLOAT3 reverseVector = Normalize(reverseVelocity);
//
//			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };
//
//			/*m_Position = Add(m_Position, reverseVector);*/
//			//���܂������
//			//���������I�u�W�F�N�g���ÓI��������
//			if (obj->GetObjectKind() == Static)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//			//���������I�u�W�F�N�g�����I��������
//			else if (obj->GetObjectKind() == Dynamic)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//
//			//�I�u�W�F�N�g�ɏ���Ă���I�u�W�F�N�g�̏d�͂�off
//			m_GameObject->SetGravityState(false);
//			m_GameObject->SetGravitiScale(0.05f);
//		
//		}
//		else
//		{
//
//			float a = 90.0f;
//			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
//			float c = 180.0f - (a + b);
//
//			//���a
//			float r = (penetrationDepth / sinf(a)) / 2;
//
//			float A = (2 * r) * sinf(a);//a�̊p�̑΂̕�
//			float B = (2 * r) * sinf(b);//b�̊p�̑΂̕�
//			float C = (2 * r) * sinf(c);//���킩���Ă��钷��
//
//			float baiA = sqrt((B * B) + (C * C));
//
//			//�@���x�N�g��
//			XMFLOAT3 reverseVelocity = { worldNormal.x,0.0f,worldNormal.z };
//			XMFLOAT3 reverseVector = Normalize(reverseVelocity);
//
//			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };
//
//			/*m_Position = Add(m_Position, reverseVector);*/
//			//���������I�u�W�F�N�g���ÓI��������
//			if (obj->GetObjectKind() == Static)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//			//���������I�u�W�F�N�g�����I��������
//			else if (obj->GetObjectKind() == Dynamic)
//			{
//				//���������I�u�W�F�N�g�̏d�������������傫��������
//				if (obj->GetWeight() > m_GameObject->GetWeight())
//				{
//					capsule.startPos = Add(capsule.startPos, reverseVector);
//					capsule.endPos = Add(capsule.endPos, reverseVector);
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Add(Pos, reverseVector);
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//				//���������I�u�W�F�N�g�̏d������������������������
//				else if (obj->GetWeight() < m_GameObject->GetWeight())
//				{
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Sub(Pos, reverseVector);
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//				//���������I�u�W�F�N�g�̏d���������Ɠ�����������
//				else if (obj->GetWeight() == m_GameObject->GetWeight())
//				{
//					//�����𓮂���
//					capsule.startPos = Add(capsule.startPos, MulFloat(reverseVector,0.5f));
//					capsule.endPos = Add(capsule.endPos, MulFloat(reverseVector, 0.5f));
//					XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//					m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, MulFloat(reverseVector, 0.5f)));
//
//					//����𓮂���
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Sub(Pos, MulFloat(reverseVector,0.5f));
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//			}
//
//		}
//		count++;
//		m_Correction = correction;
//	}
//	/*m_Position = Add(m_Position, worldCorrection);*/
//
//	//// �@���Ɋ�Â��Ĉړ��x�N�g�����C���i�ǂ�����l��
//	//XMFLOAT3 normal = Normalize(correction, NULL);
//
//
//	//float dotProduct = Dot(moveDir, worldNormal);
//	//XMFLOAT3 velo = Sub(moveDir, MulFloat(worldNormal, dotProduct));// �@���������������Ėʉ����̓����ɂ���
//
//}


//����������ƍŏ��ɓ��������I�u�W�F�N�g�Ɠ������Ă���I�u�W�F�N�g���X�g��Ԃ��܂�
std::tuple<bool, GameObject*, std::list<GameObject*>> CapsuleColiderComponent::GetCollision()
{
	
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//�{�b�N�X�ɂ��������̖@���̏��
	std::vector<std::pair<XMFLOAT3, XMFLOAT3>> CollsionDate;

	//�����ȊO�̃R���C�_�[�̃|�W�V�����ƃT�C�Y
	XMFLOAT3 pos;
	XMFLOAT3 size;

	for (auto obj : Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1))
	{
		if (obj->GetComponent<Colider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}

		pos = obj->GetComponent<Colider>()->GetPosition();
		size = obj->GetComponent<Colider>()->GetSize();

		//�R���C�_�[���J�v�Z����������
		if (obj->GetComponent<Colider>()->GetColiderKind() == CAPSULE)
		{

			if (CapsulesCollision(this->m_GameObject, obj))
			{

				objList.push_back(obj);
				objSize = objList.size();

			}
		}
		//�R���C�_�[��BOX��������
		if (obj->GetComponent<Colider>()->GetColiderKind() == BOX)
		{
			XMFLOAT3 collisionNormal;
			Box otherObb = obj->GetComponent<BoxColiderComponent>()->GetOBB();


			if (CapsuleBoxCollision(m_GameObject, obj))
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




