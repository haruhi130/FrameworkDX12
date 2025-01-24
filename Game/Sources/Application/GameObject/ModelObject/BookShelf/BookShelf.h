#pragma once

#include "../ModelObject.h"

class BookShelf : public ModelObject
{
public:
	BookShelf() { Init(); }
	~BookShelf()override{}

private:
	void Init()override;
};