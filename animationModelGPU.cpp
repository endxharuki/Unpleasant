#include "main.h"
#include "renderer.h"
#include "animationModelGPU.h"
#include "transform.h"

void AnimationModelGPU::Draw()
{

	Renderer::GetDeviceContext()->VSSetShaderResources(0, 1, &m_BoneSRV);


	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	/*ID3D11VertexShader* vertex = m_GameObject->GetComponent<Transform>()->GetVertexShader();
	ID3D11PixelShader* pixel = m_GameObject->GetComponent<Transform>()->GetPixelShader();
	ID3D11InputLayout* layout = m_GameObject->GetComponent<Transform>()->GetLayout();*/

	////入力レイアウト設定
	//Renderer::GetDeviceContext()->IASetInputLayout(layout);

	///*シェーダ設定*/
	//Renderer::GetDeviceContext()->VSSetShader(vertex, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(pixel, NULL, 0);

	for (unsigned int i = 0; i < m_AiScene->mNumMeshes; i++) {


		aiMesh* mesh = m_AiScene->mMeshes[i];


		// マテリアル設定
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

		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[i], &stride, &offset);
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		Renderer::GetDeviceContext()->DrawIndexed(m_AiScene->mMeshes[i]->mNumFaces * 3, 0, 0);
	}

	//return;

	//// プリミティブトポロジ設定
	//Renderer::GetDeviceContext()->IASetPrimitiveTopology(
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// マテリアル設定
	//MATERIAL material;
	//ZeroMemory(&material, sizeof(material));
	//material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//material.TextureEnable = true;
	//Renderer::SetMaterial(material);

	//for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	//{
	//	aiMesh* mesh = m_AiScene->mMeshes[m];


	//	// マテリアル設定
	//	aiString texture;
	//	aiColor3D diffuse;
	//	float opacity;

	//	aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
	//	aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
	//	aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	//	aimaterial->Get(AI_MATKEY_OPACITY, opacity);

	//	if (texture == aiString(""))
	//	{
	//		material.TextureEnable = false;
	//	}
	//	else
	//	{
	//		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
	//		material.TextureEnable = true;
	//	}

	//	/*float alpah = 0.5f;
	//	material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, alpah);*/

	//	material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity - m_Alpha);
	//	material.Ambient = material.Diffuse;
	//	Renderer::SetMaterial(material);


	//	// 頂点バッファ設定
	//	UINT stride = sizeof(VERTEX_3D);
	//	UINT offset = 0;
	//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

	//	// インデックスバッファ設定
	//	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

	//	// ポリゴン描画
	//	Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	//}
}


void AnimationModelGPU::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	CreateBone(m_AiScene->mRootNode);

	for (unsigned int i = 0; i < m_AiScene->mNumMeshes; i++)
	{
		LoadMeshData(m_AiScene->mMeshes[i], i);
	}
	
	/*LoadTextures();*/

	CreateBoneBuffer(m_Bone.size());

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\modelSkinning.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd,
				&m_VertexBuffer[m]);

			delete[] vertex;
		}


		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
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


		//変形後頂点データ初期化
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


		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
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
	}



	//テクスチャ読み込み
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	//
	//int boneCount = m_Bone.size();

}




void AnimationModelGPU::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;


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


void AnimationModelGPU::LoadAnimation(const char* FileName, const char* Name)
{
	const aiScene* animationScene = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(animationScene && animationScene->HasAnimations());

	m_Animation[Name] = animationScene;
}

void AnimationModelGPU::CreateBone(aiNode* node)
{
	BONE bone;

	if (m_Bone.find(node->mName.C_Str()) == m_Bone.end()) {
		BONE bone = {};
		bone.OffsetMatrix = aiMatrix4x4();
		bone.FinalTransformation = aiMatrix4x4();
		m_Bone[node->mName.C_Str()] = bone;
	}

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

void AnimationModelGPU::Update(const char* AnimationName1, int Frame1,
	const char* AnimationName2, int Frame2,
	float BlendRatio)
{
	if (m_Animation.find(AnimationName1) == m_Animation.end() ||
		m_Animation.find(AnimationName2) == m_Animation.end()) {
		return;
	}

	const aiScene* animScene1 = m_Animation[AnimationName1];
	const aiScene* animScene2 = m_Animation[AnimationName2];

	std::vector<XMMATRIX> transforms(m_Bone.size());

	for (auto& pair : m_Bone) {
		const std::string& boneName = pair.first;

		aiMatrix4x4 transform1 = GetBoneMatrices(animScene1, boneName, Frame1);
		aiMatrix4x4 transform2 = GetBoneMatrices(animScene2, boneName, Frame2);

		aiMatrix4x4 blendedTransform = transform1 * (1.0f - BlendRatio) +
			transform2 * BlendRatio;

		transforms[std::distance(m_Bone.begin(), m_Bone.find(boneName))] =
			DirectX::XMMATRIX(&blendedTransform.a1);
	}

	

	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//アニメーションデータからボーンマトリクス算出
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
			f = Frame1 % nodeAnim1->mNumRotationKeys;	//簡易実装
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim1->mNumPositionKeys;	//簡易実装
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}

		aiQuaternion rot2;
		aiVector3D pos2;

		if (nodeAnim2)
		{
			f = Frame2 % nodeAnim2->mNumRotationKeys;	//簡易実装
			rot2 = nodeAnim2->mRotationKeys[f].mValue;

			f = Frame2 % nodeAnim2->mNumPositionKeys;	//簡易実装
			pos2 = nodeAnim2->mPositionKeys[f].mValue;
		}

		aiVector3D pos;
		pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;		//線形補間

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);		//球面線形補間

		//bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//ここより下に一回簡易的にGPUスキニングをしてみる



	//再帰的にボーンマトリクスを更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	std::vector<XMMATRIX> matrices;

	//頂点返還(CPUスキニング)	
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{//目と頭と左と右の肩と脊髄の情報はとれる
			DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

			aiMatrix4x4 matrix[4];
			aiMatrix4x4 outMatrix;
			/*matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
			matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
			matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
			matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;*/


			//ウェイトを考慮してマトリクス算出
			outMatrix = matrix[0] * deformVertex->BoneWeight[0]
				+ matrix[1] * deformVertex->BoneWeight[1]
				+ matrix[2] * deformVertex->BoneWeight[2]
				+ matrix[3] * deformVertex->BoneWeight[3];
			{
				outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
					+ matrix[1].a1 * deformVertex->BoneWeight[1]
					+ matrix[2].a1 * deformVertex->BoneWeight[2]
					+ matrix[3].a1 * deformVertex->BoneWeight[3];

				outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
					+ matrix[1].a2 * deformVertex->BoneWeight[1]
					+ matrix[2].a2 * deformVertex->BoneWeight[2]
					+ matrix[3].a2 * deformVertex->BoneWeight[3];

				outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
					+ matrix[1].a3 * deformVertex->BoneWeight[1]
					+ matrix[2].a3 * deformVertex->BoneWeight[2]
					+ matrix[3].a3 * deformVertex->BoneWeight[3];

				outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
					+ matrix[1].a4 * deformVertex->BoneWeight[1]
					+ matrix[2].a4 * deformVertex->BoneWeight[2]
					+ matrix[3].a4 * deformVertex->BoneWeight[3];

				outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
					+ matrix[1].b1 * deformVertex->BoneWeight[1]
					+ matrix[2].b1 * deformVertex->BoneWeight[2]
					+ matrix[3].b1 * deformVertex->BoneWeight[3];

				outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
					+ matrix[1].b2 * deformVertex->BoneWeight[1]
					+ matrix[2].b2 * deformVertex->BoneWeight[2]
					+ matrix[3].b2 * deformVertex->BoneWeight[3];

				outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
					+ matrix[1].b3 * deformVertex->BoneWeight[1]
					+ matrix[2].b3 * deformVertex->BoneWeight[2]
					+ matrix[3].b3 * deformVertex->BoneWeight[3];

				outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
					+ matrix[1].b4 * deformVertex->BoneWeight[1]
					+ matrix[2].b4 * deformVertex->BoneWeight[2]
					+ matrix[3].b4 * deformVertex->BoneWeight[3];

				outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
					+ matrix[1].c1 * deformVertex->BoneWeight[1]
					+ matrix[2].c1 * deformVertex->BoneWeight[2]
					+ matrix[3].c1 * deformVertex->BoneWeight[3];

				outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
					+ matrix[1].c2 * deformVertex->BoneWeight[1]
					+ matrix[2].c2 * deformVertex->BoneWeight[2]
					+ matrix[3].c2 * deformVertex->BoneWeight[3];

				outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
					+ matrix[1].c3 * deformVertex->BoneWeight[1]
					+ matrix[2].c3 * deformVertex->BoneWeight[2]
					+ matrix[3].c3 * deformVertex->BoneWeight[3];

				outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
					+ matrix[1].c4 * deformVertex->BoneWeight[1]
					+ matrix[2].c4 * deformVertex->BoneWeight[2]
					+ matrix[3].c4 * deformVertex->BoneWeight[3];

				outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
					+ matrix[1].d1 * deformVertex->BoneWeight[1]
					+ matrix[2].d1 * deformVertex->BoneWeight[2]
					+ matrix[3].d1 * deformVertex->BoneWeight[3];

				outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
					+ matrix[1].d2 * deformVertex->BoneWeight[1]
					+ matrix[2].d2 * deformVertex->BoneWeight[2]
					+ matrix[3].d2 * deformVertex->BoneWeight[3];

				outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
					+ matrix[1].d3 * deformVertex->BoneWeight[1]
					+ matrix[2].d3 * deformVertex->BoneWeight[2]
					+ matrix[3].d3 * deformVertex->BoneWeight[3];

				outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
					+ matrix[1].d4 * deformVertex->BoneWeight[1]
					+ matrix[2].d4 * deformVertex->BoneWeight[2]
					+ matrix[3].d4 * deformVertex->BoneWeight[3];

			}

			

			deformVertex->Position = mesh->mVertices[v];
			deformVertex->Position *= outMatrix;

			//法線変換用に移動成分を削除
			outMatrix.a4 = 0.0f;
			outMatrix.b4 = 0.0f;
			outMatrix.c4 = 0.0f;

			deformVertex->Normal = mesh->mNormals[v];
			deformVertex->Normal *= outMatrix;

			//頂点バッファへ書き込み
			vertex[v].Position.x = deformVertex->Position.x;
			vertex[v].Position.y = deformVertex->Position.y;
			vertex[v].Position.z = deformVertex->Position.z;

			vertex[v].Normal.x = deformVertex->Normal.x;
			vertex[v].Normal.y = deformVertex->Normal.y;
			vertex[v].Normal.z = deformVertex->Normal.z;

			vertex->TexCoord.x = mesh->mTextureCoords[0][v].x;
			vertex->TexCoord.y = mesh->mTextureCoords[0][v].y;

			vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
		//UpdateBoneBuffer(transforms);
		// プリミティブトポロジ設定
	}
		
}

void AnimationModelGPU::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	//BONE* bone = &m_Bone[node->mName.C_Str()];

	////マトリクスの乗算順番に注意
	//aiMatrix4x4 worldMatrix;

	//worldMatrix *= matrix;
	//worldMatrix *= bone->AnimationMatrix;

	//bone->Matrix = worldMatrix;
	//bone->Matrix *= bone->OffsetMatrix;

	//for (unsigned int n = 0; n < node->mNumChildren; n++)
	//{
	//	UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	//}
}

void AnimationModelGPU::UpdateAnimation(const char* Animation, float frame)
{

	if (m_Animation.find(Animation) == m_Animation.end()) return;

	const aiScene* animationScene = m_Animation[Animation];
	const aiAnimation* animation = animationScene->mAnimations[0];

	float ticksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0);
	float timeInTicks = frame * ticksPerSecond;
	float animationTime = fmod(timeInTicks, (float)animation->mDuration);

	std::vector<XMMATRIX> transforms(m_Bone.size());
	CalculateBoneTransforms(animationScene->mRootNode, aiMatrix4x4(), transforms);

	UpdateBoneBuffer(transforms);

}

void AnimationModelGPU::UpdateBoneBuffer(std::vector<XMMATRIX>& boneMatrices)
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//入力レイアウト設定

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	// ボーン行列のサイズとバッファサイズを確認
	assert(boneMatrices.size() <= m_BoneCount);

	// GPUのボーンバッファを更新
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_BoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		// マップ失敗時のエラー処理
		assert(false && "Failed to map bone buffer");
		return;
	}

	// 行列データをコピー
	memcpy(mappedResource.pData, boneMatrices.data(), sizeof(XMMATRIX) * boneMatrices.size());
	Renderer::GetDeviceContext()->Unmap(m_BoneBuffer, 0);

}

void AnimationModelGPU::CreateBoneBuffer(int boneCount)
{

	m_BoneCount = boneCount;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(XMMATRIX) * boneCount;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(XMMATRIX);

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&desc, nullptr, &m_BoneBuffer);
	assert(SUCCEEDED(hr) && "Failed to create bone buffer");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = boneCount;

	hr = Renderer::GetDevice()->CreateShaderResourceView(m_BoneBuffer, &srvDesc, &m_BoneSRV);
	assert(SUCCEEDED(hr) && "Failed to create shader resource view");
}

void AnimationModelGPU::LoadMeshData(aiMesh* mesh, int meshIndex)
{

	std::vector<BoneVertex> vertices(mesh->mNumVertices);

	for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
		vertices[v].Position = {
			mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z
		};
		vertices[v].Normal = {
			mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z
		};
		vertices[v].TexCoord = {
			mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y
		};
	}

	// ボーンインデックスとウェイトの設定
	for (unsigned int b = 0; b < mesh->mNumBones; b++) {
		aiBone* aiBone = mesh->mBones[b];
		std::string boneName = aiBone->mName.C_Str();

		if (m_Bone.find(boneName) == m_Bone.end()) {
			m_Bone[boneName] = { aiBone->mOffsetMatrix, aiMatrix4x4() };
		}

		for (unsigned int w = 0; w < aiBone->mNumWeights; w++) {
			unsigned int vertexID = aiBone->mWeights[w].mVertexId;
			float weight = aiBone->mWeights[w].mWeight;

			for (int i = 0; i < 4; i++) {
				if (vertices[vertexID].BoneWeights[i] == 0.0f) {
					vertices[vertexID].BoneIndices[i] = b;
					vertices[vertexID].BoneWeights[i] = weight;
					break;
				}
			}
		}
	}

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(BoneVertex) * vertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	Renderer::GetDevice()->CreateBuffer(&vbDesc, &initData, &m_VertexBuffer[meshIndex]);
	Renderer::GetDevice()->CreateBuffer(&vbDesc, &initData, &m_IndexBuffer[meshIndex]);


}

void AnimationModelGPU::LoadTextures()
{

	if (!m_AiScene || !m_AiScene->HasMaterials()) {
		return; // マテリアルが存在しない場合は終了
	}

	//テクスチャ読み込み
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// テクスチャ読み込み
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

}

void AnimationModelGPU::CalculateBoneTransforms(const aiNode* node, const aiMatrix4x4& parentTransform, std::vector<DirectX::XMMATRIX>& transforms)
{

	std::string nodeName(node->mName.C_Str());
	aiMatrix4x4 nodeTransform = node->mTransformation;

	if (m_Bone.find(nodeName) != m_Bone.end()) {
		const BONE& bone = m_Bone[nodeName];
		nodeTransform = bone.FinalTransformation * nodeTransform;
	}

	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

	if (m_Bone.find(nodeName) != m_Bone.end()) {
		const BONE& bone = m_Bone[nodeName];
		transforms[std::distance(m_Bone.begin(), m_Bone.find(nodeName))] = DirectX::XMMATRIX(&globalTransform.a1);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		CalculateBoneTransforms(node->mChildren[i], globalTransform, transforms);
	}

}

aiMatrix4x4 AnimationModelGPU::GetBoneMatrices(const aiScene* animationScene, const std::string& boneName, int frame)
{
	const aiAnimation* animation = animationScene->mAnimations[0];
	aiMatrix4x4 boneTransform;

	// 対応するアニメーションチャネルを探す
	const aiNodeAnim* nodeAnim = nullptr;
	for (unsigned int i = 0; i < animation->mNumChannels; i++) {
		if (animation->mChannels[i]->mNodeName.C_Str() == boneName) {
			nodeAnim = animation->mChannels[i];
			break;
		}
	}

	if (!nodeAnim) {
		// ボーンがアニメーションに含まれていない場合、単位行列を返す
		return aiMatrix4x4();
	}

	// スケールの取得
	aiVector3D scaling = nodeAnim->mScalingKeys[frame % nodeAnim->mNumScalingKeys].mValue;

	// 回転の取得
	aiQuaternion rotation = nodeAnim->mRotationKeys[frame % nodeAnim->mNumRotationKeys].mValue;

	// 平行移動の取得
	aiVector3D translation = nodeAnim->mPositionKeys[frame % nodeAnim->mNumPositionKeys].mValue;

	// スケール、回転、平行移動を行列に変換
	aiMatrix4x4 scalingMatrix;
	aiMatrix4x4::Scaling(scaling, scalingMatrix);

	aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotation.GetMatrix());

	aiMatrix4x4 translationMatrix;
	aiMatrix4x4::Translation(translation, translationMatrix);

	// トランスフォーム行列を計算
	boneTransform = translationMatrix * rotationMatrix * scalingMatrix;

	return boneTransform;

}
