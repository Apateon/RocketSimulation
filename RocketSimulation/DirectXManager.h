#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include <map>
#include "AntTweakManager.h"

class DirectXManager
{
	//buffer values
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 g_CBWorld;
		DirectX::XMFLOAT4X4 g_CBView;
		DirectX::XMFLOAT4X4 g_CBProjection;
		DirectX::XMFLOAT4 g_CBCameraPosition;
		DirectX::XMFLOAT4 g_Time;
	};

	struct ConstantBufferUniform
	{
		DirectX::XMFLOAT4 g_LightPosition[5];
		DirectX::XMFLOAT4 g_LightColour[5];
		DirectX::XMUINT4 g_NumberOfLights;
	};

	std::map<std::wstring, ID3D11ShaderResourceView*> g_TexMap;
	std::map<std::wstring, std::tuple<ID3D11VertexShader*, ID3D11InputLayout*, ID3D11PixelShader*>> g_ShaderMap;
	std::map<GeometryType, std::tuple<ID3D11Buffer*, ID3D11Buffer*>> g_GeometryBufferMap;
	std::map<std::string, ID3D11Buffer*> g_InstanceMap;

	//initializing the values
	D3D_DRIVER_TYPE				mDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*				mDevice = nullptr;
	ID3D11Device1*				mDevice1 = nullptr;
	ID3D11DeviceContext*		mImmediateContext = nullptr;
	ID3D11DeviceContext1*		mImmediateContext1 = nullptr;
	IDXGISwapChain*				mSwapChain = nullptr;
	IDXGISwapChain1*			mSwapChain1 = nullptr;
	ID3D11RenderTargetView*		mRenderTargetView = nullptr;
	ID3D11Texture2D*			mDepthStencil = nullptr;
	ID3D11DepthStencilView*		mDepthStencilView = nullptr;
	ID3D11DepthStencilState*	mDepthStencilState = nullptr;
	ID3D11Buffer*				mConstantBuffer = nullptr;
	ID3D11Buffer*				mConstantBufferUniform = nullptr;
	ID3D11SamplerState*			mTexSampler = nullptr;
	ID3D11RasterizerState*		mNoCullRasterizerState = nullptr;
	ID3D11RasterizerState*		mDefaultRasterizerState = nullptr;
	ID3D11BlendState*			mAlphaBlend = nullptr;

	//GUI class Pointer
	AntTweakManager* g_AwManager;

	static HRESULT CompileShaderFromFile(const WCHAR * const pFileName, const LPCSTR pEntryPoint, const LPCSTR pShaderModel, ID3DBlob ** const pBlobOut);
	HRESULT InitializeDevice(const HWND& pHWnd);
	HRESULT CreateConstantBuffers();
	HRESULT LoadGeometryBuffers(const Shape& pShape);
	HRESULT LoadTextures(const Shape& pShape);
	HRESULT LoadShaders(const Shape& pShape);
	HRESULT LoadInstanceBuffers(const Shape& pShape);

public:

	explicit DirectXManager(const HWND& pHWnd, AntTweakManager& pAwManager);
	~DirectXManager();
	void Cleanup();

	DirectXManager& operator=(const DirectXManager& pDirectXManager) = delete;
	DirectXManager(const DirectXManager& pDirectXManager) = delete;

	HRESULT Render(const std::vector<GameObject>& pGameObjects, const Camera * const pCam, const std::vector<Light> & pLights, const float pTime);
};

