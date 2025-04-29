#pragma once





struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	XMFLOAT4	Direction;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;
	//XMFLOAT4 SkyColor; //�V���F
	//XMFLOAT4 GroundColor; //�n�ʐF
	//XMFLOAT4 GroundNormal; //�n�ʂ̖@��

	//XMFLOAT4 Position; //�����̍��W
	//XMFLOAT4 PointLightParam; //���͈̔�

	XMMATRIX	ViewMatrix;			//���C�g�J�����s�� 11/18
	XMMATRIX	ProjectionMatrix;	//���C�g�v���W�F�N�V�����s�� 11/18

};



class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_ViewProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_CameraBuffer;
	//�r���[�ˉe�s��̒萔�o�b�t�@
	static ID3D11Buffer* m_MatrixBuffer;
	
	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11DepthStencilState* m_StencilWrite;
	static ID3D11DepthStencilState* m_StencilRead;

	static ID3D11BlendState* m_BlendState;
	static ID3D11BlendState* m_BlendStateAdd;	//���Z����
	static ID3D11BlendState* m_BlendStateATC;
	static ID3D11BlendState* m_BlendStateMask;

	static ID3D11RasterizerState* m_RasterizerStateCullBack;
	static ID3D11RasterizerState* m_RasterizerStateCullNone;

	static	ID3D11RenderTargetView* m_PERendererTargetView[3];//1101�ύX
	static	ID3D11ShaderResourceView* m_PEShaderResourceView[3];//1101�ύX

	static ID3D11DepthStencilView* m_ShadowDepthStencilView;
	static ID3D11ShaderResourceView* m_ShadowDepthShaderResourceView;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetStencilEnable(bool Enable);
	static void SetBlendAddEnable(bool Enable);
	static void SetBlendMaskEnable(bool Enable);
	static void SetCullEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosision(XMFLOAT3 CameraPosition);
	
	//�V���h�E�}�b�s���O�p
	static void BeginDepth();

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	static ID3D11RenderTargetView* GetRenderTagetView(void) { return m_RenderTargetView; }
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
	static void CreateComputeShader(ID3D11ComputeShader** ComputeShader, const char* FileName);
	static void CreateSkinningVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);

};
