#include "AntTweakManager.h"
#include <directxmath.h>

using namespace std;
using namespace DirectX;

// initialize anttweak by giving it the window dimensions and the device
void AntTweakManager::Initialize(ID3D11Device* const pDevice, const UINT pWidth, const UINT pHeight) const
{
	TwInit(TW_DIRECT3D11, pDevice);
	TwWindowSize(pWidth, pHeight);
}

/// cleanup function
void AntTweakManager::Cleanup()
{
	//uninitialise and delete
	mBars.clear();
	TwDeleteAllBars();
	TwTerminate();
}

// add a new bar
void AntTweakManager::AddBar(const string& pName)
{
	//add to the new bar to the list
	mBars.insert(pair<string, TwBar*>(pName, TwNewBar(pName.c_str())));
	const auto str = pName + " iconified=true";
	TwDefine(str.c_str());
}

// adding a non editable variable to the bar. overloading helps with either datatypes of integer and float
void AntTweakManager::AddVariable(const std::string & pBarName, const std::string & pVarName, const float & pVariable, const std::string & pParameters)
{
	const auto it = mBars.find(pBarName);

	if (it != mBars.end())
	{
		TwAddVarRO(it->second, pVarName.c_str(), TW_TYPE_FLOAT, reinterpret_cast<void*>(&const_cast<float&>(pVariable)), pParameters.c_str());
	}
}
void AntTweakManager::AddVariable(const std::string & pBarName, const std::string & pVarName, const int & pVariable, const std::string & pParameters)
{
	const auto it = mBars.find(pBarName);

	if (it != mBars.end())
	{
		TwAddVarRO(it->second, pVarName.c_str(), TW_TYPE_INT32, reinterpret_cast<void*>(&const_cast<int&>(pVariable)), pParameters.c_str());
	}
}

// adding an editable variable to the bar. overloading helps with either datatypes of integer, float and xmfloat4
void AntTweakManager::AddEditVariable(const std::string & pBarName, const std::string & pVarName, const float & pVariable, const std::string & pParameters)
{
	const auto it = mBars.find(pBarName);

	if (it != mBars.end())
	{
		TwAddVarRW(it->second, pVarName.c_str(), TW_TYPE_FLOAT, reinterpret_cast<void*>(&const_cast<float&>(pVariable)), pParameters.c_str());
	}
}
void AntTweakManager::AddEditVariable(const std::string & pBarName, const std::string & pVarName, const int & pVariable, const std::string & pParameters)
{
	const auto it = mBars.find(pBarName);

	if (it != mBars.end())
	{
		TwAddVarRW(it->second, pVarName.c_str(), TW_TYPE_INT32, reinterpret_cast<void*>(&const_cast<int&>(pVariable)), pParameters.c_str());
	}
}
void AntTweakManager::AddEditVariable(const std::string & pBarName, const std::string & pVarName, const XMFLOAT4 & pVariable, const std::string & pParameters)
{
	const auto it = mBars.find(pBarName);

	if (it != mBars.end())
	{
		TwAddVarRW(it->second, pVarName.c_str(), TW_TYPE_COLOR4F, reinterpret_cast<void*>(&const_cast<XMFLOAT4&>(pVariable)), pParameters.c_str());
	}
}

//visibilty toggle
void AntTweakManager::ToggleVisible()
{
	mHide = !mHide;
	for (auto& bar : mBars)
	{
		string hiddenBool;
		if (mHide)
		{
			hiddenBool = "true";
		}
		else
		{
			hiddenBool = "false";
		}
		const auto str = bar.first + " iconified=" + hiddenBool;
		TwDefine(str.c_str());
	}
}

// draw function
void AntTweakManager::DrawBars()
{
	for (auto& bar : mBars)
	{
		TwRefreshBar(bar.second);
		TwDraw();
	}
}
