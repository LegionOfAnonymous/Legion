#include "ai.h"

ai::ai() 
{
	id = GOBJ::g_ai;

	v.Set(rand() % 1000, 0, rand() % 1000);
	v.Normalize() *= 100;
}

ai::~ai()
{
}

void ai::Update(float dt)
{
	t += v * dt;

	if (t.x + rad > 500 || t.x - rad < -500)
		v.x = -v.x;
	if (t.z + rad > 500 || t.z - rad < -500)
		v.z = -v.z;
}
