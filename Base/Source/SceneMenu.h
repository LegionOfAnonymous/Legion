#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "SceneManager2D.h"

class CGameState;

struct Button
{
	Vector3 position;
	Vector3 hitbox;
	string text;
	float size;
	bool hover;
};

class CSceneMenu : public CSceneManager2D
{
public:
	CSceneMenu();
	~CSceneMenu();

	vector<Button> menuButtons;
	Vector3 CursorPosition;
	CGameState* QueuedState;

	void UpdateMouseButton(int button);

	void Init();
	void Update(double dt);
	void Render();
};

#endif