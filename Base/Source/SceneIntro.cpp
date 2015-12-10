#include "SceneIntro.h"
#include "Application.h"

#include <GLFW/glfw3.h>

CSceneIntro::CSceneIntro()
{
}

CSceneIntro::~CSceneIntro()
{
}

void CSceneIntro::Update(double dt)
{
	fps = (float)(1.f / dt);
}

void CSceneIntro::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
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

	RenderTextOnScreen(meshList[GEO_TEXT], "[SPACE]", Color(0, 1, 0), 30, Application::m_window_width * 0.5f, Application::m_window_height * 0.25f, true);
}