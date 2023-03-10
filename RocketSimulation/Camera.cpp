#include "Camera.h"

using namespace DirectX;
using namespace std;

Camera::Camera(const XMFLOAT4& pEye, const XMFLOAT4& pRotation, const float pWidth, const float pHeight, const bool& pControllable, const string& pName) : mEye(pEye), mName(pName), mControllable(pControllable)
{
	XMMATRIX transform = XMMatrixTranslationFromVector(XMLoadFloat4(&mEye));
	transform *= XMMatrixRotationX(pRotation.x) * XMMatrixRotationY(pRotation.y);
	XMStoreFloat4x4(&mTransform, transform);
	SetDirections();
	SetView();
	SetProj(pWidth, pHeight);
}

Camera::~Camera() = default;

void Camera::SetView()
{
	if (!mControllable)
	{
		XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat4(&mEye), XMLoadFloat4(&mLookAt), XMLoadFloat4(&mUp)));
	}
	else
	{
		XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat4(&mEye), XMLoadFloat4(&mEye) + XMLoadFloat4(&mForward), XMLoadFloat4(&mUp)));
	}
}

void Camera::SetDirections()
{
	const auto right = XMFLOAT4(mTransform._11, mTransform._12, mTransform._13, 1.0f);
	XMStoreFloat4(&mRight, XMVector4Normalize(XMLoadFloat4(&right)));

	const auto up = XMFLOAT4(mTransform._21, mTransform._22, mTransform._23, 1.0f);
	XMStoreFloat4(&mUp, XMVector4Normalize(XMLoadFloat4(&up)));

	const auto forward = XMFLOAT4(mTransform._31, mTransform._32, mTransform._33, 1.0f);
	XMStoreFloat4(&mForward, XMVector4Normalize(XMLoadFloat4(&forward)));
}

void Camera::SetProj(const float pWidth, const float pHeight)
{
	XMStoreFloat4x4(&mProjection, XMMatrixPerspectiveFovLH(XM_PIDIV2, pWidth / pHeight, 0.01f, 100.0f));
}

void Camera::RotateCam(const DirectX::XMFLOAT3 & pRotation)
{
	const auto rot = XMMatrixRotationAxis(XMLoadFloat4(&mRight), pRotation.x) * XMMatrixRotationY(pRotation.y);
	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * rot);

	SetDirections();
	SetView();
}

void Camera::TranslateCam(const DirectX::XMFLOAT4 & pTranslation)
{
	XMStoreFloat4(&mEye, XMLoadFloat4(&mEye));
	XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&mTransform) * XMMatrixTranslationFromVector(XMLoadFloat4(&mEye)));

	SetView();
}

void Camera::LookAt(const XMFLOAT4 & pLookAt)
{
	mLookAt = pLookAt;
	SetView();
}

const XMFLOAT4X4& Camera::View() const
{
	return mView;
}

const XMFLOAT4X4& Camera::Proj() const
{
	return mProjection;
}

const bool& Camera::Controllable() const
{
	return mControllable;
}

const std::string& Camera::Name() const
{
	return mName;
}

const XMFLOAT4& Camera::Eye() const
{
	return mEye;
}

const DirectX::XMFLOAT4& Camera::Forward() const
{
	return mForward;
}
const DirectX::XMFLOAT4& Camera::Up() const
{
	return mUp;
}

const DirectX::XMFLOAT4& Camera::Right() const
{
	return mRight;
}
