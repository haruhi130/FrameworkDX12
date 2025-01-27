#pragma once

#include "../ModelObject.h"

class Meat : public ModelObject
{
public:
	Meat() { Init(); }
	~Meat()override {}

private:
	void Init()override;
};