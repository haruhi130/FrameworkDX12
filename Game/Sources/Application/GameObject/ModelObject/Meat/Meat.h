#pragma once

#include "../ModelObject.h"

class Meat : public ModelObject
{
public:
	Meat() { Init(); }
	~Meat()override{}

	void Draw()override;
	void DrawShadow()override;

private:
	void Init()override;

};