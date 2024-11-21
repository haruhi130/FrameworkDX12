#pragma once

#include "../ModelObject.h"

class Pedestal : public ModelObject
{
public:
	Pedestal() { Init(); }
	~Pedestal()override{}

	void Draw()override;
	void DrawShadow()override;

private:
	void Init()override;

};