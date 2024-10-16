#pragma once

#include "../ModelObject.h"

class Pedestal : public ModelObject
{
public:
	Pedestal() { Init(); }
	~Pedestal()override{}

	void Draw()override;

private:
	void Init()override;

};