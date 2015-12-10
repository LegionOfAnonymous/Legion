#ifndef ai_H
#define ai_H

#include "GObj.h"

class ai : public GOBJ
{
public:
	ai();
	~ai();

	void Update(float dt);
};

#endif