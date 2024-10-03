#pragma once

class SkySphere :public BaseObject
{
public:
	SkySphere() { Init(); }
	~SkySphere()override {}

	void Update()override;

	void Draw()override;

	void SetTarget(const std::shared_ptr<BaseObject>& target);

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;

	std::weak_ptr<BaseObject> m_wpTarget;

	Math::Vector3 m_localPos;
	Math::Matrix m_mLocal;

};