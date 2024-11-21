#pragma once

#include "../ModelObject.h"

class Box : public ModelObject
{
public:
	Box() { Init(); }
	~Box()override{}

	void Draw()override;
	void DrawShadow()override;

private:
	void Init()override;

};
