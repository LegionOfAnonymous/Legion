#ifndef G_MANAGER_H
#define G_MANAGER_H

#include "Mtx44.h"

#include <vector>
using namespace::std;

class GOBJ;
class Mesh;
class GManager
{
public:
	GManager();
	~GManager();

	GOBJ* Fetch();

	void Update(float dt);
	vector<GOBJ*> G;

	bool Test(GOBJ* a, GOBJ* b);

	int point;
};

#endif