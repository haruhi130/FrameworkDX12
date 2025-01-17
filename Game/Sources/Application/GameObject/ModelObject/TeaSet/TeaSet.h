#pragma once

#include "../ModelObject.h"

class TeaSet : public ModelObject
{
public:
	TeaSet() { Init(); }
	~TeaSet()override{}

	void Update()override;

private:
	void Init()override;

	std::shared_ptr<SoundInstance3D> m_se;
};