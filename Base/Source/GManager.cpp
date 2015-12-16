#include "GManager.h"
#include "Mesh.h"
#include "GObj.h"

GManager::GManager() : point(0)
{
}

GManager::~GManager()
{
}

GOBJ * GManager::Fetch()
{
	for (auto& g : G)
	{
		if (!g->a)
		{
			g->a = true;
			return g;
		}
	}

	GOBJ* g = new GOBJ();
	G.push_back(g);
	return g;
}

void GManager::Update(float dt)
{
	for (int i = 0; i < G.size(); ++i)
	{
		if (G[i]->a)
		{
			G[i]->Update(dt);

			if (G[i]->id == GOBJ::g_proj) for (int j = 0; j < G.size(); ++j)
			{
				if (G[j]->a && G[j]->id != GOBJ::g_proj)
				{
					if (Test(G[i], G[j]))
					{
						G[i]->a = false;
						G[j]->a = false;
						point++;
						break;
					}
				}
			}
		}
	}
}

bool GManager::Test(GOBJ * a, GOBJ * b)
{
	float cR = a->rad + b->rad;

	if (Vector3(a->t - b->t).LengthSquared() < cR*cR)
		return true;

	return false;
}
