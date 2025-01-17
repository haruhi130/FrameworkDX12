#pragma once

#include "../ModelObject.h"

class Pedestal : public ModelObject
{
public:
	Pedestal() { Init(); }
	~Pedestal()override{}

private:
	void Init()override;

};