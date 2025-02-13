#pragma once

#include "../ModelObject.h"

class CirclePedestal : public ModelObject
{
public:
	CirclePedestal() { Init(); }
	~CirclePedestal()override {}

	void Update()override;

private:
	void Init()override;

};