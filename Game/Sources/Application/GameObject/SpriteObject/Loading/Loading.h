#pragma once

#include "../SpriteObject.h"

class Loading : public SpriteObject
{
public:
	Loading() { Init(); }
	~Loading()override{}

	void DrawSprite()override;
	void Update()override;

private:

};