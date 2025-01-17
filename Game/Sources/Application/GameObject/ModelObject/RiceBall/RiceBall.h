#pragma once

#include "../ModelObject.h"

class RiceBall : public ModelObject
{
public:
	RiceBall() { Init(); }
	~RiceBall()override{}

private:
	void Init()override;

};