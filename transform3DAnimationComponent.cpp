#include "main.h"
#include "transform3DAnimationComponent.h"


void Transform3DAnimationComponent::Init()
{
	
	if (m_GameObject != nullptr) {


		m_Model = new AnimationModel(m_GameObject);
		m_TestModel = new AnimationModelTest(m_GameObject);
		//m_Model->Load(m_ModelData);*/

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
		//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
		Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);

		ComponentNumber = 2;

		m_AnimationFrame = 0.0f;
	}
	
}

void Transform3DAnimationComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	//Model->UnloadAll();*/
	delete m_Model;
	delete m_TestModel;

}

void Transform3DAnimationComponent::Update()
{
	
	//m_Model->Update(m_AnimationName.c_str(), m_AnimationFrame, m_AnimationName.c_str(), m_AnimationFrame, m_AnimationBolendRatio);
	m_TestModel->Update(m_CurrentAnimationName.c_str(), m_AnimationFrame, m_NextAnimationName.c_str(), m_AnimationFrame, m_AnimationBolendRatio);

	if (m_DelayFrame == m_Delay)
	{
		m_AnimationFrame++;
		m_DelayFrame = 0;
	}
	
	m_DelayFrame++;

	//�A�j���[�V�����̃X�e�[�g���ς������AnimationBolendRatio��0�ɂ���
	if (m_ChangeState == true)
	{
		m_AnimationBolendRatio += 0.1f;
		if (m_AnimationBolendRatio > 1.0f)
		{
			m_AnimationBolendRatio = 1.0f;
			m_CurrentAnimationName = m_NextAnimationName;
			m_ChangeState = false;
		}
	}

}

void Transform3DAnimationComponent::Draw()
{
	if (m_ModelData != "�Ȃ�") {
		/*if (m_Model != nullptr) {*/

			//���̓��C�A�E�g�ݒ�
			Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

			/*�V�F�[�_�ݒ�*/
			Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

			//���[���h�}�g���N�X�ݒ�
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
			//scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
			trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
			world = scale * rot * trans;

			Renderer::SetWorldMatrix(world);

			//m_Model->Draw();
			m_TestModel->Draw();
		//}
	}
	

}
