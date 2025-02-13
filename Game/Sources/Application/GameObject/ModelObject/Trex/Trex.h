#pragma once

#include "../ModelObject.h"

class Trex : public ModelObject
{
public:
	Trex() { Init(); }
	~Trex()override{}

private:
	void Init()override;
};