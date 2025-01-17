#pragma once

#include "../ModelObject.h"

class CirclePedestal : public ModelObject
{
public:
	CirclePedestal() { Init(); }
	~CirclePedestal()override {}

private:
	void Init()override;

};