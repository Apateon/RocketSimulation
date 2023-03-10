#include "Shape.h"

using namespace DirectX;
using namespace std;

Shape::Shape(const std::vector<Instance>* const pInstances, const XMFLOAT4& pScale, const XMFLOAT4& pRotation, const XMFLOAT4& pTranslation,
	const wstring& pDiffuseTex, const wstring& pNormalMap, const wstring& pHeightMap,
	const wstring& pShader, const string& pName, const bool& pIsEnvironment, const bool& pBlended, const GeometryType& pGeometryType) :
	mScale(pScale),
	mRotation(pRotation),
	mTranslation(pTranslation),
	mDiffuseTexture(pDiffuseTex),
	mNormalMap(pNormalMap),
	mHeightMap(pHeightMap),
	mShader(pShader),
	mName(pName),
	mIsEnvironment(pIsEnvironment),
	mBlended(pBlended),
	mGeometryType(pGeometryType)
{
	if (pInstances)
	{
		mInstances = *pInstances;
	}
	SetGeometry(mGeometryType);
	SetTransform();
}

void Shape::SetTransform()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixScalingFromVector(XMLoadFloat4(&mScale)));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixTranslationFromVector(XMLoadFloat4(&mTranslation)));
}

void Shape::Translate(const XMFLOAT4& pTranslation)
{
	XMStoreFloat4(&mTranslation, XMLoadFloat4(&mTranslation) + XMLoadFloat4(&pTranslation));
	SetTransform();
}

void Shape::Rotate(const XMFLOAT4& pRotation)
{
	XMStoreFloat4(&mRotation, XMLoadFloat4(&mRotation) + XMLoadFloat4(&pRotation));
	SetTransform();
}

void Shape::Scale(const XMFLOAT4& pScale)
{
	XMStoreFloat4(&mScale, XMLoadFloat4(&mScale) + XMLoadFloat4(&pScale));
	SetTransform();
}

const DirectX::XMFLOAT4X4* const Shape::Transform() const
{
	return &mTransform;
}

const std::vector<SimpleVertex>& Shape::Vertices() const
{
	return mVertices;
}

const std::vector<WORD>& Shape::Indices() const
{
	return mIndices;
}

const std::vector<Instance>& Shape::Instances() const
{
	return mInstances;
}

const std::wstring& Shape::DiffuseTexture() const
{
	return mDiffuseTexture;
}

const std::wstring& Shape::NormalMap() const
{
	return mNormalMap;
}

const std::wstring& Shape::HeightMap() const
{
	return mHeightMap;
}

const GeometryType& Shape::Geometry() const
{
	return mGeometryType;
}

const std::string& Shape::Name() const
{
	return mName;
}

const bool Shape::IsEnvironment() const
{
	return mIsEnvironment;
}

const bool Shape::IsBlended() const
{
	return mBlended;
}

void Shape::RemoveInstances(const std::vector<Instance>& pIndexToDelete)
{
	for (auto& index : pIndexToDelete)
	{
		mInstances.erase(remove(mInstances.begin(), mInstances.end(), index), mInstances.end());
	}
}

void Shape::SetInstances(const std::vector<Instance>& pInstances)
{
	mInstances = pInstances;
}

void Shape::SetRotation(const DirectX::XMFLOAT4& pRotation)
{
	mRotation = pRotation;
	SetTransform();
}

const wstring& Shape::Shader() const
{
	return mShader;
}

void Shape::SetGeometry(const GeometryType pGeoType)
{
	switch (pGeoType)
	{
	case GeometryType::CUBE:
		SetCube();
		break;
	case GeometryType::CYLINDER:
		SetCylinder();
		break;
	case GeometryType::CONE:
		SetCone();
		break;
	case GeometryType::QUAD:
		SetQuad();
		break;
	}
}

void Shape::SetCube()
{
	//set vertices
	//top
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f),		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	//back
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	//right
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f,-0.5f),		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f),		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f,0.5f),		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f,0.5f),		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	//front
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f,-0.5f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f,-0.5f),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	//left
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f,0.5f),		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f,0.5f),		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f),		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f,-0.5f),	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	//bottom
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0),		DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f),	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);

	// Set Indices
	mIndices.emplace_back(0);
	mIndices.emplace_back(1);
	mIndices.emplace_back(2);
	mIndices.emplace_back(0);
	mIndices.emplace_back(2);
	mIndices.emplace_back(3);

	mIndices.emplace_back(4);
	mIndices.emplace_back(5);
	mIndices.emplace_back(6);
	mIndices.emplace_back(4);
	mIndices.emplace_back(6);
	mIndices.emplace_back(7);

	mIndices.emplace_back(8);
	mIndices.emplace_back(9);
	mIndices.emplace_back(10);
	mIndices.emplace_back(8);
	mIndices.emplace_back(10);
	mIndices.emplace_back(11);

	mIndices.emplace_back(12);
	mIndices.emplace_back(13);
	mIndices.emplace_back(14);
	mIndices.emplace_back(12);
	mIndices.emplace_back(14);
	mIndices.emplace_back(15);

	mIndices.emplace_back(16);
	mIndices.emplace_back(17);
	mIndices.emplace_back(18);
	mIndices.emplace_back(16);
	mIndices.emplace_back(18);
	mIndices.emplace_back(19);

	mIndices.emplace_back(20);
	mIndices.emplace_back(21);
	mIndices.emplace_back(22);
	mIndices.emplace_back(20);
	mIndices.emplace_back(22);
	mIndices.emplace_back(23);
}

void Shape::SetCylinder()
{
	const auto pointsOnCircumference = 50;

	//Centres
	mVertices.emplace_back(
		//Position							Normal							Tangent							Binormal							TexCoord						
		SimpleVertex{ XMFLOAT3(0.0f, 0.5f, 0.0f),			XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),			XMFLOAT2(0.5f, 0.5f) });

	mVertices.emplace_back(
		//Position							Normal							Tangent							Binormal							TexCoord						
		SimpleVertex{ XMFLOAT3(0.0f, -0.5f, 0.0f),		XMFLOAT3(0.0f, -1.0f, 0.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),			XMFLOAT2(0.5f, 0.5f) });

	for (auto i = 0; i < pointsOnCircumference; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircumference - 1);
		const float theta = 2 * XM_PI * fraction;

		XMFLOAT3 normal = XMFLOAT3(sin(theta), 0.0f, cos(theta));
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 1.0f, 0.0f);
		XMFLOAT3 binormal{};
		XMStoreFloat3(&binormal, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));

		//Tube
		//Top Edge
		mVertices.emplace_back(
			//Position										Normal			Tangent			Binormal		TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), 0.5f, cos(theta)),			normal,			tangent,		binormal,		XMFLOAT2(fraction, 1.0f) });

		//Bottom Edge
		mVertices.emplace_back(
			//Position										Normal			Tangent			Binormal		TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), -0.5f, cos(theta)),		normal,			tangent,		binormal,		XMFLOAT2(fraction, 0.0f) });

		//Cylinder Caps
		//Top
		mVertices.emplace_back(
			//Position									Normal								Tangent							Binormal						TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), 0.5f, cos(theta)),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),		XMFLOAT2((sin(theta) + 1) / 2, (cos(theta) + 1) / 2) });

		//Bottom
		mVertices.emplace_back(
			//Position									Normal								Tangent							Binormal						TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), -0.5f, cos(theta)),	XMFLOAT3(0.0f, -1.0f, 0.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, -1.0f),		XMFLOAT2((sin(theta) + 1) / 2, (cos(theta) + 1) / 2) });
	}

	//tube indices
	for (auto i = 4; i < (pointsOnCircumference * 4); i += 4)
	{
		//tube
		mIndices.push_back(i - 1);
		mIndices.push_back(i + 3);
		mIndices.push_back(i - 2);

		mIndices.push_back(i - 2);
		mIndices.push_back(i + 3);
		mIndices.push_back(i + 2);

		//top cap
		mIndices.push_back(0);
		mIndices.push_back(i);
		mIndices.push_back(i + 4);

		//bottom cap
		mIndices.push_back(1);
		mIndices.push_back(i + 5);
		mIndices.push_back(i + 1);
	}
}

void Shape::SetCone()
{
	const auto pointsOnCircumference = 500;

	for (auto i = 0; i < pointsOnCircumference; i++)
	{
		const float fraction = static_cast<float>(i) / (pointsOnCircumference - 1);
		const float theta = 2 * XM_PI * fraction;
		const float lengthOfSlope = 1 / sqrt(2);
		const XMFLOAT2 crossSectionNormal = XMFLOAT2(-lengthOfSlope, lengthOfSlope);
		XMFLOAT3 normal = XMFLOAT3(sin(theta) * -crossSectionNormal.y, crossSectionNormal.x, cos(theta) * -crossSectionNormal.y);
		XMFLOAT3 tangent = XMFLOAT3(sin(theta), -1.0f, cos(theta));
		XMFLOAT3 binormal{};
		XMStoreFloat3(&binormal, XMVector3Cross(XMLoadFloat3(&normal), XMLoadFloat3(&tangent)));

		//Cone point
		mVertices.emplace_back(
			//Position							Normal				Tangent				Binormal			TexCoord						
			SimpleVertex{ XMFLOAT3(0.0f, 0.5f, 0.0f),			normal,				tangent,			binormal,			XMFLOAT2(fraction, 1.0f) });

		//Cone base
		mVertices.emplace_back(
			//Position										Normal			Tangent			Binormal		TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), -0.5f, cos(theta)),			normal,			tangent,		binormal,		XMFLOAT2(fraction, 0.0f) });

		//Cone circle
		mVertices.emplace_back(
			//Position									Normal								Tangent							Binormal							TexCoord						
			SimpleVertex{ XMFLOAT3(sin(theta), -0.5f, cos(theta)),	XMFLOAT3(0.0f, -1.0f, 0.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, -1.0f),		XMFLOAT2((sin(theta) + 1) / 2, (cos(theta) + 1) / 2) });

	}

	//Cone circle center
	mVertices.emplace_back(
		//Position							Normal								Tangent								Binormal							TexCoord						
		SimpleVertex{ XMFLOAT3(0.0f, -0.5f, 0.0f),			XMFLOAT3(0.0f, -1.0f, 0.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, -1.0f),		XMFLOAT2(0.5f, 0.5f) });

	for (auto i = 0; i < pointsOnCircumference * 3 - 3; i += 3)
	{
		mIndices.push_back(i);
		mIndices.push_back(i + 1);
		mIndices.push_back(i + 4);

		mIndices.push_back(mVertices.size() - 1);
		mIndices.push_back(i + 5);
		mIndices.push_back(i + 2);
	}
}

void Shape::SetQuad()
{
	//set vertices
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, -0.5f, 0),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 0.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(0.5f, 0.5f, 0),			DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT2(1.0f, 1.0f) }
	);
	mVertices.emplace_back(		//Position									Normal									Tangent									Binormal								TexCoord
		SimpleVertex{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0),		DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),	DirectX::XMFLOAT2(0.0f, 1.0f) }
	);

	//set indices
	mIndices.emplace_back(0);
	mIndices.emplace_back(2);
	mIndices.emplace_back(1);
	mIndices.emplace_back(0);
	mIndices.emplace_back(3);
	mIndices.emplace_back(2);
}