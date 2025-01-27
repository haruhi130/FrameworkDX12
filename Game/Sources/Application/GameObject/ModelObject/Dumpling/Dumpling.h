#pragma once

#include "../ModelObject.h"

class Dumpling : public ModelObject
{
public:
	Dumpling() { Init(); }
	~Dumpling()override{}

private:
	void Init()override;

};