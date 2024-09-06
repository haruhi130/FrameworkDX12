#pragma once

class GameCamera : public BaseObject
{
public:
	GameCamera(){}
	~GameCamera()override{}

	void PostUpdate()override;
	void PreDraw()override;

	void SetTarget(const std::shared_ptr<BaseObject>& target);

private:
	void Init()override;

	std::shared_ptr<Camera> m_spCamera = nullptr;

	std::weak_ptr<BaseObject> m_wpTarget;

	Math::Vector3 m_vec = {};
};