#pragma once
#include <directxmath.h>

class Light
{
	DirectX::XMFLOAT4X4 mTransform{};
	DirectX::XMFLOAT4 mScale;
	DirectX::XMFLOAT4 mRotation;
	DirectX::XMFLOAT4 mTranslation;
	DirectX::XMFLOAT4 mOrbit;
	DirectX::XMFLOAT4 mOrbitTranslation;
	DirectX::XMFLOAT4 mColour{};
	DirectX::XMFLOAT4 mPosition{};
	void SetTransform();

public:
	Light(const DirectX::XMFLOAT4& pScale, const DirectX::XMFLOAT4& pRotation, const DirectX::XMFLOAT4& pTranslation, const DirectX::XMFLOAT4& pOrbit, const DirectX::XMFLOAT4& pOrbitTranslation, const DirectX::XMFLOAT4& pColour);
	~Light() = default;

	const DirectX::XMFLOAT4X4 * const Transform() const;

	void Translate(const DirectX::XMFLOAT4& pTranslation);
	void Rotate(const DirectX::XMFLOAT4& pRotation);
	void Orbit(const DirectX::XMFLOAT4& pRotation);
	void OrbitTranslate(const DirectX::XMFLOAT4& pTranslation);
	void Scale(const DirectX::XMFLOAT4& pScale);
	void SetTranslation(const DirectX::XMFLOAT4& pTranslation);

	const DirectX::XMFLOAT4 & Position() const;
	void SetColour(const DirectX::XMFLOAT4& pColour);
	const DirectX::XMFLOAT4 & Colour() const;
	const DirectX::XMFLOAT4 & GetOrbit() const;
};