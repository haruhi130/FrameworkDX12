#pragma once

#include "../ModelObject.h"

class Cheese : public ModelObject
{
public:
	Cheese() { Init(); }
	~Cheese()override{}

	void Draw()override;
	void DrawShadow()override;

	void Update()override;

private:
	void Init()override;

	std::shared_ptr<SoundInstance3D> m_se;

};