#pragma once

#include "../ModelObject.h"

class Cheese : public ModelObject
{
public:
	Cheese() { Init(); }
	~Cheese()override{}

	void Draw()override;

private:
	void Init()override;

};