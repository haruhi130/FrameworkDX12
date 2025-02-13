#pragma once

#include "../SpriteObject.h"

class Loading : public SpriteObject
{
public:
	Loading() { Init(); }
	~Loading()override{}

	void DrawSprite()override;
	void Update()override;

	void SetRotation(bool isRot)
	{
		m_isRot = isRot;
	}

private:

	bool m_isRot = false;
	Math::Vector3 m_rotateVec;
};