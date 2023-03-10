#pragma once
#include <directxmath.h>
#include <string>

class Camera
{
	DirectX::XMFLOAT4X4 mView{};
	DirectX::XMFLOAT4X4 mProjection{};
	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT4 mEye;
	DirectX::XMFLOAT4 mForward;
	DirectX::XMFLOAT4 mUp;
	DirectX::XMFLOAT4 mRight;
	DirectX::XMFLOAT4 mLookAt{};
	std::string mName;
	bool mControllable;
	void SetView();
	void SetDirections();
	void SetProj(const float pWidth, const float pHeight);

public:
	Camera(const DirectX::XMFLOAT4& pEye, const DirectX::XMFLOAT4& pRotation, const float pWidth, const float pHeight, const bool& pControllable, const std::string& pName);
	~Camera();

	void RotateCam(const DirectX::XMFLOAT3& pRotation);
	void TranslateCam(const DirectX::XMFLOAT4& pTranslation);
	void LookAt(const DirectX::XMFLOAT4& pLookAt);

	const DirectX::XMFLOAT4& Eye() const;
	const DirectX::XMFLOAT4& Forward() const;
	const DirectX::XMFLOAT4& Up() const;
	const DirectX::XMFLOAT4& Right() const;
	const DirectX::XMFLOAT4X4& View() const;
	const DirectX::XMFLOAT4X4& Proj() const;
	const bool& Controllable() const;
	const std::string& Name() const;

	inline void SetEye(const DirectX::XMFLOAT4& pEye)
	{
		mEye = pEye;
		SetView();
	}
};