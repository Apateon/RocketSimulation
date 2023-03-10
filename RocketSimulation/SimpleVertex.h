#pragma once
#include <directxmath.h>

struct SimpleVertex
{
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mNormal;
	DirectX::XMFLOAT3 mTangent;
	DirectX::XMFLOAT3 mBinormal;
	DirectX::XMFLOAT2 mTexCoord;
};
