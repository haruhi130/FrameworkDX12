#pragma once

#include "../ModelObject.h"

class Cow : public ModelObject
{
public:
	Cow() { Init(); }
	~Cow()override{}

private:
	void Init()override;
};