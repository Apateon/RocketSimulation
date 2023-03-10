#include "GameObject.h"

using namespace DirectX;
using namespace std;

GameObject::GameObject(const XMFLOAT4& pScale, const XMFLOAT4&pRotation, const XMFLOAT4& pTranslation) : mScale(pScale), mRotation(pRotation), mTranslation(pTranslation)
{
	SetTransform();
}

GameObject::~GameObject() = default;

void GameObject::Translate(const XMFLOAT4& pTranslation)
{
	XMStoreFloat4(&mTranslation, XMLoadFloat4(&mTranslation) + XMLoadFloat4(&pTranslation));
	SetTransform();
}

void GameObject::SetTranslation(const DirectX::XMFLOAT4 & pTranslation)
{
	XMStoreFloat4(&mTranslation, XMLoadFloat4(&pTranslation));
	SetTransform();
}

const DirectX::XMFLOAT4 & GameObject::Rotation() const
{
	return mRotation;
}

const DirectX::XMFLOAT4 & GameObject::Position() const
{
	return mPosition;

}

void GameObject::Rotate(const XMFLOAT4& pRotation)
{
	XMStoreFloat4(&mRotation, XMLoadFloat4(&mRotation) + XMLoadFloat4(&pRotation));
	SetTransform();
}

void GameObject::Scale(const XMFLOAT4& pScale)
{
	XMStoreFloat4(&mScale, XMLoadFloat4(&mScale) + XMLoadFloat4(&pScale));
	SetTransform();
}

const std::vector<Shape> & GameObject::Shapes() const
{
	return mShapes;
}

void GameObject::AddShape(const vector<Instance> * const pInstances,
	const XMFLOAT4& pScale,
	const XMFLOAT4& pRotation,
	const XMFLOAT4& pTranslation,
	const wstring& pDiffuseTex,
	const wstring& pNormalMap,
	const wstring& pHeightMap,
	const wstring& pShader,
	const string& pName,
	const bool& pEnvironment,
	const bool& pBlended,
	const GeometryType& pGeometryType )
{
	mShapes.emplace_back(pInstances, pScale, pRotation, pTranslation, pDiffuseTex, pNormalMap, pHeightMap, pShader, pName, pEnvironment, pBlended, pGeometryType);
}

const XMFLOAT4X4 * const GameObject::Transform() const
{
	return &mTransform;
}

const XMFLOAT4 & GameObject::Forward() const
{
	return mForward;
}

const XMFLOAT4 & GameObject::Up() const
{
	return mUp;
}

const XMFLOAT4 & GameObject::Right() const
{
	return mRight;
}

void GameObject::ResetObject()
{
	mRotation = XMFLOAT4(0, 0, 0, 1);
	mScale = XMFLOAT4(1, 1, 1, 1);
	mTranslation = XMFLOAT4(0, 0, 0, 1);
	SetTransform();
}

void GameObject::RotateShape(const int & pIndex, const DirectX::XMFLOAT4 & pRotation)
{
	mShapes[pIndex].Rotate(pRotation);
}

void GameObject::SetShapeRotation(const int & pIndex, const DirectX::XMFLOAT4 & pRotation)
{
	mShapes[pIndex].SetRotation(pRotation);
}

void GameObject::RemoveInstancesFromShape(const int & pIndex, const std::vector<Instance>& pInstances)
{
	mShapes[pIndex].RemoveInstances(pInstances);
}

void GameObject::SetShapeInstances(const int & pIndex, const std::vector<Instance>& pInstances)
{
	mShapes[pIndex].SetInstances(pInstances);
}

void GameObject::SetTransform()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixScalingFromVector(XMLoadFloat4(&mScale)));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixTranslationFromVector(XMLoadFloat4(&mTranslation)));

	const auto right = XMFLOAT4(mTransform._11, mTransform._12, mTransform._13, 1.0f);
	XMStoreFloat4(&mRight, XMVector4Normalize(XMLoadFloat4(&right)));

	const auto up = XMFLOAT4(mTransform._21, mTransform._22, mTransform._23, 1.0f);
	XMStoreFloat4(&mUp, XMVector4Normalize(XMLoadFloat4(&up)));

	const auto forward = XMFLOAT4(mTransform._31, mTransform._32, mTransform._33, 1.0f);
	XMStoreFloat4(&mForward, XMVector4Normalize(XMLoadFloat4(&forward)));

	mPosition = XMFLOAT4(mTransform._41, mTransform._42, mTransform._43, 1);
}