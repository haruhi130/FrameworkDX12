#pragma once

#include "../ModelObject.h"

class Pedestal : public ModelObject
{
public:
	Pedestal() { Init(); }
	~Pedestal()override{}

	void Update()override;

private:
	void Init()override;

};