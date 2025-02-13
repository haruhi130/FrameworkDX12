#pragma once

#include "../ModelObject.h"

class Cow : public ModelObject
{
public:
	Cow() { Init(); }
	~Cow()override{}

	void Update()override;

private:
	void Init()override;
};