#include "Light.h"

using namespace DirectX;

Light::Light(const XMFLOAT4& pScale, const XMFLOAT4& pRotation, const XMFLOAT4& pTranslation, const XMFLOAT4& pOrbit, const DirectX::XMFLOAT4& pOrbitTranslation, const XMFLOAT4& pColour) :
	mScale(pScale),
	mRotation(pRotation),
	mTranslation(pTranslation),
	mOrbit(pOrbit),
	mOrbitTranslation(pOrbitTranslation),
	mColour(pColour)
{
	SetTransform();
}

void Light::SetTransform()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixScalingFromVector(XMLoadFloat4(&mScale)));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixRotationX(mRotation.x) * XMMatrixRotationY(mRotation.y) * XMMatrixRotationZ(mRotation.z));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixTranslationFromVector(XMLoadFloat4(&mOrbitTranslation)));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixRotationX(mOrbit.x) * XMMatrixRotationY(mOrbit.y) * XMMatrixRotationZ(mOrbit.z));

	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixTranslationFromVector(XMLoadFloat4(&mTranslation)));

	mPosition = XMFLOAT4(mTransform._41, mTransform._42, mTransform._43, 1);
}

void Light::Translate(const XMFLOAT4& pTranslation)
{
	XMStoreFloat4(&mTranslation, XMLoadFloat4(&mTranslation) + XMLoadFloat4(&pTranslation));
	SetTransform();
}

void Light::Rotate(const XMFLOAT4& pRotation)
{
	XMStoreFloat4(&mRotation, XMLoadFloat4(&mRotation) + XMLoadFloat4(&pRotation));
	SetTransform();
}

void Light::Orbit(const DirectX::XMFLOAT4 & pRotation)
{
	XMStoreFloat4(&mOrbit, XMLoadFloat4(&mOrbit) + XMLoadFloat4(&pRotation));
	SetTransform();
}

void Light::OrbitTranslate(const DirectX::XMFLOAT4 & pTranslation)
{
	XMStoreFloat4(&mOrbitTranslation, XMLoadFloat4(&mOrbitTranslation) + XMLoadFloat4(&pTranslation));
	SetTransform();
}

void Light::Scale(const XMFLOAT4& pScale)
{
	XMStoreFloat4(&mScale, XMLoadFloat4(&mScale) + XMLoadFloat4(&pScale));
	SetTransform();
}

void Light::SetTranslation(const DirectX::XMFLOAT4 & pTranslation)
{
	mTranslation = pTranslation;
	SetTransform();
}

const XMFLOAT4 & Light::Position() const
{
	return mPosition;
}

const DirectX::XMFLOAT4X4 * const Light::Transform() const
{
	return &mTransform;
}

void Light::SetColour(const XMFLOAT4 & pColour)
{
	mColour = pColour;
}

const XMFLOAT4 & Light::Colour() const
{
	return mColour;
}

const DirectX::XMFLOAT4 & Light::GetOrbit() const
{
	return mOrbit;
}