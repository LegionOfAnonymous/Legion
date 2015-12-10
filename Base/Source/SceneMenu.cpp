#include "SceneMenu.h"
#include "PlayState.h"
#include "GameState.h"
#include "Application.h"

#include <GLFW/glfw3.h>

CSceneMenu::CSceneMenu() : QueuedState(NULL)
{
}

CSceneMenu::~CSceneMenu()
{
}

void CSceneMenu::UpdateMouseButton(int button)
{
	if (button)
	{
		for (unsigned i = 0; i < menuButtons.size(); ++i)
		{
			if (menuButtons[i].hover)
			{
				if (menuButtons[i].text == "START")
					QueuedState = CPlayState::Instance();
				if (menuButtons[i].text == "QUIT")
					Application::active = false;
				break;
			}

		}
	}
}

void CSceneMenu::Init()
{
	CSceneManager2D::Init();

	Button b; 

	b.position.Set(Application::m_window_width * 0.5f, Application::m_window_height * 0.55f);
	b.text = "START";
	b.size = 64;
	b.hitbox.Set(b.text.length() * 64 * 0.8f + 16, 64 + 16);

	menuButtons.push_back(b);

	b.position.Set(Application::m_window_width * 0.5f, Application::m_window_height * 0.55f - 128);
	b.text = "HELP";
	b.size = 64;
	b.hitbox.Set(b.text.length() * 64 * 0.8f + 16, 64 + 16);

	menuButtons.push_back(b);

	b.position.Set(Application::m_window_width * 0.5f, Application::m_window_height * 0.55f - 256);
	b.text = "QUIT";
	b.size = 64;
	b.hitbox.Set(b.text.length() * 64 * 0.8f + 16, 64 + 16);

	menuButtons.push_back(b);
}

void CSceneMenu::Update(double dt)
{
	fps = (float)(1.f / dt);

	CursorPosition.x = Application::mouse_current_x;
	CursorPosition.y = Application::m_window_height - Application::mouse_current_y;
	
	if (CursorPosition.x > Application::m_window_width)
		CursorPosition.x = Application::m_window_width;
	else if (CursorPosition.x < 0)
		CursorPosition.x = 0;
	else if (CursorPosition.y > Application::m_window_height)
		CursorPosition.y = Application::m_window_height;
	else if (CursorPosition.y < 0)
		CursorPosition.y = 0;

	for (unsigned i = 0; i < menuButtons.size(); ++i)
	{
		if (CursorPosition.x > menuButtons[i].position.x - menuButtons[i].hitbox.x * 0.5f && CursorPosition.x < menuButtons[i].position.x + menuButtons[i].hitbox.x * 0.5f &&
			CursorPosition.y > menuButtons[i].position.y - menuButtons[i].hitbox.y * 0.5f && CursorPosition.y < menuButtons[i].position.y + menuButtons[i].hitbox.y * 0.5f)
		{
			menuButtons[i].hover = true;
		}
		else
			menuButtons[i].hover = false;
	}
}

void CSceneMenu::Render()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderTextOnScreen(meshList[GEO_TEXT], "TING YANG", Color(1, 1, 0), 128, Application::m_window_width * 0.5f, Application::m_window_height - 96, true);
	
	for (unsigned i = 0; i < menuButtons.size(); ++i)
	{
		if (!menuButtons[i].hover)
			RenderTextOnScreen(meshList[GEO_TEXT], menuButtons[i].text, Color(1.0f, 1.0f, 0.0f), menuButtons[i].size, menuButtons[i].position.x, menuButtons[i].position.y, true);
		else
			RenderTextOnScreen(meshList[GEO_TEXT], menuButtons[i].text, Color(1, 1, 1), menuButtons[i].size * 1.25f, menuButtons[i].position.x, menuButtons[i].position.y, true);
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "NUB", Color(1, 1, 0), 24, CursorPosition.x, CursorPosition.y, true);
}