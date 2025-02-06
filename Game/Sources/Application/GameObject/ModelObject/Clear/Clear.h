#pragma once

#include "../ModelObject.h"

class Clear : public ModelObject
{
public:
	Clear() { Init(); }
	~Clear()override{}

	void Update()override;

	void SetPos(const Math::Vector3& pos)override
	{
		m_pos = pos;
		m_mWorld.Translation(pos);
	}

private:
	void Init()override;

	Math::Vector3 m_pos;

	std::shared_ptr<EffekseerObject> m_spEffect = nullptr;
};