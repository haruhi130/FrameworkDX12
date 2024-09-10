#pragma once

class GameCamera : public BaseObject
{
public:
	GameCamera() { Init(); }
	~GameCamera()override{}

	void PostUpdate()override;
	void PreDraw()override;

	const Math::Matrix GetRotationMatrix() const
	{
		return Math::Matrix::CreateFromYawPitchRoll
		(
			DirectX::XMConvertToRadians(m_vec.y),
			DirectX::XMConvertToRadians(m_vec.x),
			DirectX::XMConvertToRadians(m_vec.z)
		);
	}

	const Math::Matrix GetRotationYMat()const
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_vec.y));
	}

	const std::shared_ptr<const Camera>& GetCamera()const
	{
		return m_spCamera;
	}

	std::shared_ptr<Camera> WorkCamera()const
	{
		return m_spCamera;
	}

	void SetTarget(const std::shared_ptr<BaseObject>& target);

private:
	void Init()override;

	void UpdateRotateByMouse();

	std::shared_ptr<Camera> m_spCamera = nullptr;

	std::weak_ptr<BaseObject> m_wpTarget;

	Math::Matrix m_mRot;

	Math::Vector3 m_vec = {};
	Math::Vector3 m_local = {};
	Math::Matrix m_localPos;

	POINT m_mousePos = {};
};