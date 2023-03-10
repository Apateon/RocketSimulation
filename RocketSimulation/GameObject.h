#pragma once
#include "Shape.h"

class GameObject
{
	DirectX::XMFLOAT4X4 mTransform{};
	DirectX::XMFLOAT4 mScale;
	DirectX::XMFLOAT4 mRotation;
	DirectX::XMFLOAT4 mTranslation;
	std::vector<Shape> mShapes;
	DirectX::XMFLOAT4 mForward{};
	DirectX::XMFLOAT4 mUp{};
	DirectX::XMFLOAT4 mRight{};
	DirectX::XMFLOAT4 mPosition{};

	void SetTransform();

public:
	GameObject(const DirectX::XMFLOAT4& pScale, const DirectX::XMFLOAT4&pRotation, const DirectX::XMFLOAT4& pTranslation);
	~GameObject();

	void Scale(const DirectX::XMFLOAT4& pScale);
	void Rotate(const DirectX::XMFLOAT4& pRotation);
	void Translate(const DirectX::XMFLOAT4& pTranslation);
	void SetTranslation(const DirectX::XMFLOAT4& pTranslation);

	const DirectX::XMFLOAT4& Rotation() const;

	const DirectX::XMFLOAT4& Position() const;

	const std::vector<Shape> & Shapes() const;

	void AddShape(const std::vector<Instance> * const pInstances,
		const DirectX::XMFLOAT4& pScale,
		const DirectX::XMFLOAT4& pRotation,
		const DirectX::XMFLOAT4& pTranslation,
		const std::wstring& pDiffuseTex,
		const std::wstring& pNormalMap,
		const std::wstring& pHeightMap,
		const std::wstring& pShader,
		const std::string& pName,
		const bool& pEnvironment,
		const bool& pBlended,
		const GeometryType& pGeometryType);
	const DirectX::XMFLOAT4X4 * const Transform() const;
	const DirectX::XMFLOAT4& Forward() const;
	const DirectX::XMFLOAT4& Up() const;
	const DirectX::XMFLOAT4& Right() const;
	void ResetObject();

	void RotateShape(const int& pIndex, const DirectX::XMFLOAT4 & pRotation);
	void SetShapeRotation(const int& pIndex, const DirectX::XMFLOAT4 & pRotation);
	void RemoveInstancesFromShape(const int& pIndex, const std::vector<Instance> & pInstances);
	void SetShapeInstances(const int& pIndex, const std::vector<Instance> & pInstances);
};