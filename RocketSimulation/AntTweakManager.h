#pragma once
#include <AntTweakBar.h>
#include <map>
#include <d3d11.h>
#include <directxmath.h>

class AntTweakManager
{
	std::map<std::string, TwBar*> mBars;
	bool mHide = true;

public:
	AntTweakManager() = default;
	~AntTweakManager() = default;
	void Initialize(ID3D11Device* const pDevice, const UINT pWidth, const UINT pHeight) const;
	void Cleanup();

	void AddBar(const std::string& pName);
	void AddVariable(const std::string& pBarName, const std::string& pVarName, const float& pVariable, const std::string& pParameters);
	void AddVariable(const std::string& pBarName, const std::string& pVarName, const int& pVariable, const std::string& pParameters);

	void AddEditVariable(const std::string& pBarName, const std::string& pVarName, const float& pVariable, const std::string& pParameters);
	void AddEditVariable(const std::string& pBarName, const std::string& pVarName, const int& pVariable, const std::string& pParameters);
	void AddEditVariable(const std::string& pBarName, const std::string& pVarName, const DirectX::XMFLOAT4& pVariable, const std::string& pParameters);

	void ToggleVisible();

	void DrawBars();
};

