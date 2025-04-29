#include "main.h"
#include "renderer.h"
#include "animationModelTest.h"

void AnimationModelTest::Draw()
{
	int i = 0;
	BoneMatrices* boneMatrix = new BoneMatrices;
	for (int i = 0; i < 255; i++)
	{
		XMFLOAT4X4 mat = {};

		mat._11 = 0.0f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 0.0f; mat._22 = 0.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 0.0f;

		boneMatrix->boneMatrices[i] = mat;
	}
	for (const std::pair<std::string,BONE> bone: m_Bone)
	{
		if (i >= 255) break;
		const aiMatrix4x4& aiMat = bone.second.Matrix;
		XMFLOAT4X4 mat = {};

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

		//�f�o�b�O�p
		/*mat._11 = 0.0f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 0.0f; mat._22 = 0.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 0.0f;*/

		// �]�u
		XMMATRIX matTranspose = XMMatrixTranspose(XMLoadFloat4x4(&mat));
		XMStoreFloat4x4(&mat, matTranspose);
		
		boneMatrix->boneMatrices[i] = mat;
		i++;
	}

	boneMatrix->vertexCount = static_cast<UINT>(m_VertexCount);
	SetBoneMatrices(boneMatrix);

	ID3D11PixelShader* pixel = m_GameObject->GetComponent<Transform>()->GetPixelShader();
	ID3D11InputLayout* layout = m_GameObject->GetComponent<Transform>()->GetLayout();

	/*�V�F�[�_�ݒ�*/
	
	//Renderer::GetDeviceContext()->PSSetShader(pixel, NULL, 0);
	
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(nullptr);

	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, NULL, 0);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		//�V�F�[�_�[�փf�[�^�𑗐M
		Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &m_ConstantBuffer);
		Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_BeforeVertexShaderSRV[m]);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_AfterVertexShaderUAV[m], NULL);

		UINT numThreadGroup = (/*���_��*/mesh->mNumVertices + 255) / 256;	// 256�X���b�h�̉��O���[�v�K�v���v�Z
		Renderer::GetDeviceContext()->Dispatch(numThreadGroup, 1, 1);	// cs�̎��s

		// uav�̍Đݒ�ɂ�郊�\�[�X�o���A
		ID3D11UnorderedAccessView* nulluav = nullptr;
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nulluav, nullptr);

		// �}�e���A���ݒ�
		aiString texture;
		aiColor3D diffuse;
		float opacity;

		aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);

		if (texture == aiString(""))
		{
			material.TextureEnable = false;
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
			material.TextureEnable = true;
		}


		material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity - m_Alpha);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);


		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		// ComputeShader�ł̃X�L�j���O������̒��_�ݒ�
		Renderer::GetDeviceContext()->VSSetShaderResources(0, 1, &m_SkinningUpdateSRV[m]);

		// ���_�o�b�t�@�̐ݒ�
		//Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_AfterVertexBuffer, &stride, &offset); 

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
		//Renderer::GetDeviceContext()->Draw(mesh->mNumFaces * 3, 0);

	}

	delete boneMatrix;
}


void AnimationModelTest::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];
	// �{�[���� �� �{�[��ID �̃}�b�s���O
	std::map<std::string, int> boneNameIndex;
	int boneCount = 0;

	//�ċA�I�Ƀ{�[������
	CreateBone(m_AiScene->mRootNode,boneNameIndex,boneCount);

	
	
	//GPU�p��ComputeShader�ƒ��_Shader�𐶐�
	Renderer::CreateComputeShader(&m_ComputeShader, "shader\\skinningComputeShader.cso");
	Renderer::CreateVertexShader(&m_VertexShader, nullptr, "shader\\skinningVertexShader.cso");

	m_VertexCount = 0;

	std::vector<BoneVertex> vertexList;
	std::vector<Vertex> afterList;
	std::vector<unsigned int> indexAll;


	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{

		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{

			BoneVertex* b_vertex = new BoneVertex[mesh->mNumVertices];
			m_VertexCount += mesh->mNumVertices;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				b_vertex[v].Position = XMFLOAT4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
				b_vertex[v].Normal = XMFLOAT4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.0f);
				b_vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				b_vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				for (int i = 0; i < 4; i++)
				{
					b_vertex[v].BoneIndices[i] = 0;
					b_vertex[v].BoneWeights[i] = 0.0f;
				}

			}


			//�ό`�㒸�_�f�[�^������
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX deformVertex;
				deformVertex.Position = mesh->mVertices[v];
				deformVertex.Normal = mesh->mNormals[v];
				deformVertex.BoneNum = 0;

				for (unsigned int b = 0; b < 4; b++)
				{
					deformVertex.BoneName[b] = "";
					deformVertex.BoneWeight[b] = 0.0f;
				}

				m_DeformVertex[m].push_back(deformVertex);
			}

			//�{�[���f�[�^������
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];

				m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

				//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];

					int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

					m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
					m_DeformVertex[m][weight.mVertexId].BoneNum++;

					assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
				}
			}

			int n = m_DeformVertex[m].size();
			for (int i = 0; i < n; i++)
			{
				int b = 0;
				for (auto bone : m_Bone)
				{
					for (int v = 0; v < 4; v++)
					{
						if (bone.first == m_DeformVertex[m][i].BoneName[v])
						{
							boneNameIndex[bone.first] = b;
							
						}
					}
					b++;
				}

			}
			for (unsigned int v = 0; v < n; v++)
			{
				for (auto bone : boneNameIndex)
				{
					for (int w = 0; w < 4; w++)
					{
						if (m_DeformVertex[m][v].BoneName[w] == bone.first)
						{
							b_vertex[v].BoneIndices[w] = bone.second;
							b_vertex[v].BoneWeights[w] = m_DeformVertex[m][v].BoneWeight[w];
						}
					}
				}

			}

			
			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				BoneVertex& v = b_vertex[i];

				int wight = v.BoneWeights[0] + v.BoneWeights[1] + v.BoneWeights[2] + v.BoneWeights[3];
				vertexList.push_back(b_vertex[i]);
				if (wight > 1.1f)
				{
					wight = 0;
				}
			}
			vertexList;

			//�ό`�O���_�o�b�t�@�쐬
			CreateBeforeVertexBuffer(mesh->mNumVertices, b_vertex, m);
			//�ό`�O�̒��_�o�b�t�@�̃��\�[�X�r���[�쐬
			CreateBeforeVertexSRV(mesh->mNumVertices, m);
			Vertex* vertex = new Vertex[mesh->mNumVertices];


			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{

				vertex[v].Position = XMFLOAT4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
				vertex[v].Normal = XMFLOAT4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.0f);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				Vertex& ver = vertex[v];
				afterList.push_back(ver);
				//ver.Position.x = 0.0f;
			}

			//�v�Z��̒��_�i�[�p�o�b�t�@�쐬
			CreateAfterVertexBuffer(mesh->mNumVertices, vertex, m);
			CreateAfterVertexSRV(mesh->mNumVertices, m);
			CreateUAV(mesh->mNumVertices, m);

			//CreateConstantBuffer();


		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
				indexAll.push_back(index[f * 3 + 0]);
				indexAll.push_back(index[f * 3 + 1]);
				indexAll.push_back(index[f * 3 + 2]);

			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}
	}


	//�e�N�X�`���ǂݍ���
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// �e�N�X�`���ǂݍ���
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	
	CreateConstantBuffer();

}




void AnimationModelTest::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	//delete[] m_DeformVertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}

}


void AnimationModelTest::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void AnimationModelTest::CreateBone(aiNode* node, std::map<std::string, int>& boneNameIndex,
	int& boneCount)
{
	
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n],boneNameIndex,boneCount);
	}

	//std::string boneName = node->mName.C_Str();

	//// ���łɓo�^�ς݂̃{�[���łȂ����m�F
	//if (m_Bone.find(boneName) == m_Bone.end())
	//{
	//	BONE newBone;

	//	// Assimp �� aiMatrix4x4 ���璼�ڑ��
	//	newBone.Matrix = node->mTransformation;

	//	// AnimationMatrix �͏������ς݂̂܂�
	//	// OffsetMatrix �������ł͕ύX���Ȃ��i��� aiBone->mOffsetMatrix ��K�p����j

	//	m_Bone[boneName] = newBone;
	//}

	//// �{�[���� �� �C���f�b�N�X�ւ̓o�^
	//if (boneNameIndex.find(boneName) == boneNameIndex.end())
	//{
	//	boneNameIndex[boneName] = boneCount;
	//	boneCount++;
	//}

	//// �q�{�[�����ċA�I�ɓo�^
	//for (unsigned int n = 0; n < node->mNumChildren; n++)
	//{
	//	CreateBone(node->mChildren[n], boneNameIndex, boneCount);
	//}
}

void AnimationModelTest::Update(const char* AnimationName1, int Frame1,
	const char* AnimationName2, int Frame2,
	float BlendRatio)
{

	//return;

	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];


	for (auto pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;
		aiNodeAnim* nodeAnim2 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim2 = animation2->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1)
		{
			f = Frame1 % nodeAnim1->mNumRotationKeys;	//�ȈՎ���
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;	//�ȈՎ���
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}

		aiQuaternion rot2;
		aiVector3D pos2;

		if (nodeAnim2)
		{
			f = Frame2 % nodeAnim2->mNumRotationKeys;	//�ȈՎ���
			rot2 = nodeAnim2->mRotationKeys[f].mValue;

			f = Frame2 % nodeAnim2->mNumPositionKeys;	//�ȈՎ���
			pos2 = nodeAnim2->mPositionKeys[f].mValue;
		}

		aiVector3D pos;
		pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;		//���`���

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);		//���ʐ��`���

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//����
	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	if(m_AiScene->mRootNode)

	return;

	
}

void AnimationModelTest::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;

	worldMatrix *= matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;

	//aiString boneName;
	//boneName.Set("Hips"); // ��: "Hips" �Ƃ������O���Z�b�g

	//if (strcmp(boneName.C_Str(), "Hips") == 0) {
	//	
	//	
	//}
	
	//����ŕ��ʂ̃}�g���N�X�͎���
	if (strcmp(node->mName.C_Str(), "mixamorig7:RightHand") == 0)
	{

		XMFLOAT4X4 mat = {};

		mat._11 = 0.0f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 0.0f; mat._22 = 0.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f; mat._44 = 0.0f;

		aiMatrix4x4& aiMat = bone->Matrix;

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;
		
		XMMATRIX matTranspose = XMLoadFloat4x4(&mat);
		m_RightHandMatrix = matTranspose;

	}

	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}


void AnimationModelTest::UpdateBoneBuffer(std::vector<aiMatrix4x4> boneMatrices)
{
	Renderer::GetDeviceContext()->UpdateSubresource(m_BoneBuffer, 0, nullptr, boneMatrices.data(), 0, 0);

}

int AnimationModelTest::GetBoneIndex(const std::string& boneName)
{
	if (m_Bone.find(boneName) == m_Bone.end()) {
		m_Bone[boneName] = BONE();
		return static_cast<int>(m_Bone.size() - 1);
	}
	return 0;
}

void AnimationModelTest::CreateBeforeVertexBuffer(int index, BoneVertex* vertex, int mesh)
{

	// �o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BoneVertex) * index/*���b�V���̒��_��*/;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // Structured Buffer �ɂ���
	bd.StructureByteStride = sizeof(BoneVertex);  // �e�v�f�̃T�C�Y��ݒ�

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_BeforeVertexBuffer[mesh]);

}

void AnimationModelTest::CreateBeforeVertexSRV(int index, int mesh)
{

	// cs�Ŏg����srv�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC notDeformSRVDesc;
	ZeroMemory(&notDeformSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	notDeformSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	notDeformSRVDesc.Buffer.FirstElement = 0;
	notDeformSRVDesc.Buffer.NumElements = index;
	notDeformSRVDesc.Format = DXGI_FORMAT_UNKNOWN;


	Renderer::GetDevice()->CreateShaderResourceView(m_BeforeVertexBuffer[mesh], &notDeformSRVDesc, &m_BeforeVertexShaderSRV[mesh]);

}

void AnimationModelTest::CreateAfterVertexBuffer(int index, Vertex* vertex, int mesh)
{

	// �o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * index;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; // **UAV���쐬���邽�߂ɕK�v**
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(Vertex);  // �e�v�f�̃T�C�Y��ݒ�

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_AfterVertexBuffer[mesh]);

	
}

void AnimationModelTest::CreateUAV(int index, int mesh)
{
	// cs�ŏ����o�����p��UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = index;

	HRESULT hr = Renderer::GetDevice()->CreateUnorderedAccessView(m_AfterVertexBuffer[mesh], &uavDesc, &m_AfterVertexShaderUAV[mesh]);

}

void AnimationModelTest::CreateAfterVertexSRV(int index, int mesh)
{
	// vs�œǂݍ��܂��X�L�j���O������𑗂�SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC updatedSrvDesc = {};
	updatedSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	updatedSrvDesc.Buffer.FirstElement = 0;
	updatedSrvDesc.Buffer.NumElements = index;
	updatedSrvDesc.Format = DXGI_FORMAT_UNKNOWN;

	Renderer::GetDevice()->CreateShaderResourceView(m_AfterVertexBuffer[mesh], &updatedSrvDesc, &m_SkinningUpdateSRV[mesh]);

}

void AnimationModelTest::CreateConstantBuffer()
{

	// CS�ɓn���萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;


	// 16�o�C�g�A���C�����g���m��
	size_t size = sizeof(BoneMatrices);
	bufferDesc.ByteWidth = (size + 15) & ~15;  // 16�̔{���ɐ؂�グ

	bufferDesc.StructureByteStride = 0;
	bufferDesc.MiscFlags = 0;
	

	Renderer::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer);

}

void AnimationModelTest::SetBoneMatrices(const BoneMatrices* matrices)
{

	Renderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, matrices, 0,0);

	/*D3D11_MAPPED_SUBRESOURCE mappedResource;
	Renderer::GetDeviceContext()->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, matrices.boneMatrices, sizeof(XMFLOAT4X4) * matrices.size());
	Renderer::GetDeviceContext()->Unmap(m_ConstantBuffer, 0);*/

}


