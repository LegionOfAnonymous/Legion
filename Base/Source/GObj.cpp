#include "GObj.h"

GOBJ::GOBJ() : rad(1), m(0), a(true), id(g_null)
{
}

GOBJ::~GOBJ()
{
}

void GOBJ::Update(float dt)
{
	t += v * dt;

	if (t.x > 500 || t.x < -500 || t.z > 500 || t.z < -500)
		a = false;
}

Mtx44 GOBJ::GetTransform()
{
	Mtx44 T, Rx, Ry, Rz, S;

	T.SetToTranslation(t.x, t.y, t.z);
	Rx.SetToRotation(r.x, 1, 0, 0);
	Ry.SetToRotation(r.y, 0, 1, 0);
	Rz.SetToRotation(r.z, 0, 0, 1);
	S.SetToScale(s.x, s.y, s.z);

	return T * Rx * Ry * Rz * S;
}
