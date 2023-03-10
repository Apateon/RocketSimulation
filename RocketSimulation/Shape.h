#pragma once
#include <directxmath.h>
#include <vector>
#include "GeometryType.h"
#include "windows.h"
#include "SimpleVertex.h"
#include "Instance.h"
#include <algorithm>
#include <string>

class Shape
{
	std::vector<SimpleVertex> mVertices;
	std::vector<WORD> mIndices;
	std::vector<Instance> mInstances;

	DirectX::XMFLOAT4X4 mTransform{};
	DirectX::XMFLOAT4 mScale;
	DirectX::XMFLOAT4 mRotation;
	DirectX::XMFLOAT4 mTranslation;

	std::wstring mDiffuseTexture;
	std::wstring mNormalMap;
	std::wstring mHeightMap;
	std::wstring mShader;
	std::string mName;

	bool mIsEnvironment;
	bool mBlended;

	GeometryType mGeometryType = GeometryType::CUBE;

	void SetGeometry(GeometryType pGeoType);
	void SetCube();
	void SetCylinder();
	void SetCone();
	void SetQuad();

	void SetTransform();

public:
	Shape(const std::vector<Instance> * const pInstances,
		const DirectX::XMFLOAT4& pScale,
		const DirectX::XMFLOAT4& pRotation,
		const DirectX::XMFLOAT4& pTranslation,
		const std::wstring& pDiffuseTex,
		const std::wstring& pNormalMap,
		const std::wstring& pHeightMap,
		const std::wstring& pShader,
		const std::string& pName,
		const bool& pIsEnvironment,
		const bool& pBlended,
		const GeometryType& pGeometryType
	);

	~Shape() = default;

	Shape& operator=(const Shape& pShape) = delete;

	const DirectX::XMFLOAT4X4 * const Transform() const;
	void Translate(const DirectX::XMFLOAT4& pTranslation);
	void Rotate(const DirectX::XMFLOAT4& pRotation);
	void Scale(const DirectX::XMFLOAT4& pScale);

	const std::vector<SimpleVertex>& Vertices() const;
	const std::vector<WORD>& Indices() const;
	const std::vector<Instance>& Instances() const;
	const std::wstring& DiffuseTexture() const;
	const std::wstring& NormalMap() const;
	const std::wstring& HeightMap() const;
	const std::wstring& Shader() const;
	const GeometryType& Geometry() const;
	const std::string& Name() const;
	const bool IsEnvironment() const;
	const bool IsBlended() const;
	void RemoveInstances(const std::vector<Instance>& pIndexToDelete);
	void SetInstances(const std::vector<Instance>& pInstances);
	void SetRotation(const DirectX::XMFLOAT4& pRotation);
};