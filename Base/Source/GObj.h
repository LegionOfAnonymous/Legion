#ifndef GOBJ_H
#define GOBJ_H

#include "Mtx44.h"

class GOBJ
{
public:
	GOBJ();
	virtual ~GOBJ();

	enum gT
	{
		g_null,
		g_proj,
		g_ai,
		g_g,
	};
	gT id;

	Vector3 t;
	Vector3 r;
	Vector3 s;
	Vector3 v;

	virtual void Update(float dt);
	Mtx44 GetTransform();

	int m;
	float rad;
	bool a;
};

#endif