#pragma once
#include <directxmath.h>

struct Instance
{
	DirectX::XMFLOAT3 mPosition;
};

inline bool operator==(const Instance& pLhs, const Instance& pRhs)
{
	return (abs(pLhs.mPosition.x - pRhs.mPosition.x) < std::numeric_limits<float>::epsilon() &&
			abs(pLhs.mPosition.y - pRhs.mPosition.y) < std::numeric_limits<float>::epsilon() &&
			abs(pLhs.mPosition.z - pRhs.mPosition.z) < std::numeric_limits<float>::epsilon());
}
