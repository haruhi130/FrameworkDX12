#pragma once

#include "../ModelObject.h"

class Sky : public ModelObject
{
public:
	Sky() { Init(); }
	~Sky()override{}

	void Update()override;
	void Draw()override;

	void SetTarget(const std::shared_ptr<ModelObject>& target)
	{
		if (!target)return;
		m_wpTarget = target;
	}

private:
	void Init()override;

	std::weak_ptr<ModelObject> m_wpTarget;

	Math::Matrix m_mLocal;
};