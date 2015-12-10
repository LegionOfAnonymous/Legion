#ifndef SCENE_INTRO_H
#define SCENE_INTRO_H

#include "SceneManager2D.h"

class CSceneIntro : public CSceneManager2D
{
public:
	CSceneIntro();
	~CSceneIntro();

	void Update(double dt);
	void Render();
};

#endif