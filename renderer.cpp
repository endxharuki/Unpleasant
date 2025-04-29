#include "main.h"
#include "renderer.h"
#include <io.h>


D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::m_Device{};
ID3D11DeviceContext* Renderer::m_DeviceContext{};
IDXGISwapChain* Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer* Renderer::m_WorldBuffer{};
ID3D11Buffer* Renderer::m_ViewBuffer{};
ID3D11Buffer* Renderer::m_ProjectionBuffer{};
ID3D11Buffer* Renderer::m_ViewProjectionBuffer{};
ID3D11Buffer* Renderer::m_MaterialBuffer{};
ID3D11Buffer* Renderer::m_LightBuffer{};
ID3D11Buffer* Renderer::m_CameraBuffer{};
//ボーン処理用
ID3D11Buffer* Renderer::m_MatrixBuffer{};


ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};
ID3D11DepthStencilState* Renderer::m_StencilWrite{};
ID3D11DepthStencilState* Renderer::m_StencilRead{};


ID3D11BlendState* Renderer::m_BlendState{};
ID3D11BlendState* Renderer::m_BlendStateAdd{};
ID3D11BlendState* Renderer::m_BlendStateATC{};
ID3D11BlendState* Renderer::m_BlendStateMask{};

ID3D11RasterizerState* Renderer::m_RasterizerStateCullBack{};
ID3D11RasterizerState* Renderer::m_RasterizerStateCullNone{};

ID3D11RenderTargetView* Renderer::m_PERendererTargetView[3] = { NULL,NULL };//11/01変更
ID3D11ShaderResourceView* Renderer::m_PEShaderResourceView[3] = { NULL,NULL };//1101

ID3D11DepthStencilView* Renderer::m_ShadowDepthStencilView = NULL;
ID3D11ShaderResourceView* Renderer::m_ShadowDepthShaderResourceView = NULL;




void Renderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_DeviceContext);






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();


	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerStateCullBack);

	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerStateCullNone);

	m_DeviceContext->RSSetState(m_RasterizerStateCullBack);



	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState(&blendDesc, &m_BlendState);

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateAdd);
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateMask);
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateATC);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);


	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = TRUE;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_StencilWrite);

	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_StencilRead);


	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState(&samplerDesc, &samplerState);

	m_DeviceContext->PSSetSamplers(0, 1, &samplerState);



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);


	//ボーン処理のためのバーファーを生成
	// ビュー射影行列用の定数バッファを作成
	D3D11_BUFFER_DESC BoneBufferDesc = {};
	BoneBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BoneBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	BoneBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BoneBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_Device->CreateBuffer(&BoneBufferDesc, nullptr, &m_MatrixBuffer);


	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(1.0f, -0.5f, 1.0f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	SetMaterial(material);

	//レンダリグテクスチャ0作成
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));//構造体を0クリア

		td.Width = swapChainDesc.BufferDesc.Width;	//バックバッファサイズ
		td.Height = swapChainDesc.BufferDesc.Height;

		td.MipLevels = 1;	//作成するミップマップの数
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		m_Device->CreateTexture2D(&td, NULL, &ppTexture);
		//レンダ―ターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC	rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(ppTexture,
			&rtvd,
			&m_PERendererTargetView[0]);



		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC		srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(ppTexture,
			&srvd,
			&m_PEShaderResourceView[0]);
	}

	//レンダリグテクスチャ1作成 0920
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));//構造体を0クリア

		td.Width = swapChainDesc.BufferDesc.Width;	//バックバッファサイズ
		td.Height = swapChainDesc.BufferDesc.Height;

		td.MipLevels = 1;	//作成するミップマップの数
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		m_Device->CreateTexture2D(&td, NULL, &ppTexture);
		//レンダ―ターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC	rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(ppTexture,
			&rtvd,
			&m_PERendererTargetView[1]);

		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC		srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(ppTexture,
			&srvd,
			&m_PEShaderResourceView[1]);
	}


	//レンダリグテクスチャ2作成 1101
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));//構造体を0クリア

		td.Width = swapChainDesc.BufferDesc.Width;	//バックバッファサイズ
		td.Height = swapChainDesc.BufferDesc.Height;

		td.MipLevels = 1;	//作成するミップマップの数
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		m_Device->CreateTexture2D(&td, NULL, &ppTexture);
		//レンダ―ターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC	rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(ppTexture,
			&rtvd,
			&m_PERendererTargetView[2]);

		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC		srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(ppTexture,
			&srvd,
			&m_PEShaderResourceView[2]);
	}

	//深度バッファテクスチャ3作成 1202
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));//構造体を0クリア

		td.Width = swapChainDesc.BufferDesc.Width;	//バックバッファサイズ
		td.Height = swapChainDesc.BufferDesc.Height;

		td.MipLevels = 1;	//作成するミップマップの数
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R32_TYPELESS;//32Bitの自由な形式な値
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		m_Device->CreateTexture2D(&td, NULL, &ppTexture);
		//デプスステンシルターゲットビュー
		D3D11_DEPTH_STENCIL_VIEW_DESC	rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_D32_FLOAT;		//32Bit float型
		rtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		rtvd.Flags = 0;
		m_Device->CreateDepthStencilView(ppTexture,
			&rtvd,
			&m_ShadowDepthStencilView);

		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC		srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32_FLOAT;		//32Bit float型
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(ppTexture,
			&srvd,
			&m_ShadowDepthShaderResourceView);
	}


}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();


	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

}




void Renderer::Begin()
{
	//レンダリングターゲットをバックバッファにする
	m_DeviceContext->OMSetRenderTargets(1,
		&m_RenderTargetView,
		m_DepthStencilView);

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}



void Renderer::End()
{
	m_SwapChain->Present(1, 0);
}




void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}



void Renderer::SetATCEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetStencilEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_StencilWrite, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_StencilRead, NULL);
}

void Renderer::SetBlendAddEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetBlendMaskEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateMask, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
}

void Renderer::SetCullEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->RSSetState(m_RasterizerStateCullBack);
	else
		m_DeviceContext->RSSetState(m_RasterizerStateCullNone);
}

void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projectionf, 0, 0);

}



void Renderer::SetMaterial(MATERIAL Material)
{
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosision(XMFLOAT3 CameraPosition)
{
	XMFLOAT4 cameraPos;
	cameraPos.x = CameraPosition.x;
	cameraPos.y = CameraPosition.y;
	cameraPos.z = CameraPosition.z;
	cameraPos.w = 0.0f;
	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &cameraPos, 0, 0);
}

void Renderer::BeginDepth()
{
	//深度バッファをテクスチャへ切り替え、内容を1.0でクリアする
	m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
	m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::CreateComputeShader(ID3D11ComputeShader** ComputeShader, const char* FileName)
{

	// ファイルポインタとサイズの宣言
	FILE* file;
	long int fsize;

	// ファイルを開く
	file = fopen(FileName, "rb");
	assert(file); // ファイルが開けない場合はエラー

	// ファイルサイズを取得
	fsize = _filelength(_fileno(file));

	// シェーダーバイナリを格納するバッファを確保
	unsigned char* buffer = new unsigned char[fsize];

	// ファイルからデータを読み込む
	fread(buffer, fsize, 1, file);

	// ファイルを閉じる
	fclose(file);

	// コンピュートシェーダーを作成
	m_Device->CreateComputeShader(buffer, fsize, NULL, ComputeShader);
	
	// バッファを解放
	delete[] buffer;
}

void Renderer::CreateSkinningVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{




}
