
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneManager.h"
#include "SceneManager2D.h"

#include "GameStateManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;
bool Application::m_window_deadzone = true;
bool Application::active = true;
int Application::mouseScroll = 0;

/********************************************************************************
 Define an error callback
 ********************************************************************************/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/********************************************************************************
 Define the key input callback
 ********************************************************************************/
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
	Application::mouseScroll = int(y);
}

/********************************************************************************
 Callback function when the window is resized
 ********************************************************************************/
void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/********************************************************************************
 Get keyboard's key states
 ********************************************************************************/
bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

/********************************************************************************
 Get mouse updates
 ********************************************************************************/
bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	if (m_window_deadzone)
	{
		// Calculate the difference in positions
		mouse_diff_x = mouse_current_x - m_window_width * 0.5f;
		mouse_diff_y = mouse_current_y - m_window_height * 0.5f;

		//Calculate the yaw and pitch
		camera_yaw = (float)mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
		camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

		glfwSetCursorPos(m_window, m_window_width * 0.5f, m_window_height * 0.5f);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	int Button_Left = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
	int Button_Middle = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
	int ButtonRight = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);

	// Get the mouse button status
	theGSM->HandleEvents(mouse_current_x, mouse_current_y, Button_Left, Button_Middle, ButtonRight);
	
    return false;
}

/********************************************************************************
 Get keyboard updates
 ********************************************************************************/
bool Application::GetKeyboardUpdate()
{
	if (IsKeyPressed(VK_RETURN))
	{
		theGSM->HandleEvents(VK_RETURN);
	}
	else
	{
		theGSM->HandleEvents(VK_RETURN, false);
	}

	if (IsKeyPressed('1'))
	{
		theGSM->HandleEvents('1');
	}
	else
	{
		theGSM->HandleEvents('1', false);
	}
	if (IsKeyPressed('2'))
	{
		theGSM->HandleEvents('2');
	}
	else
	{
		theGSM->HandleEvents('2', false);
	}


	if (IsKeyPressed('A'))
	{
		theGSM->HandleEvents('a');
	}
	else
	{
		theGSM->HandleEvents('a', false);
	}
	if (IsKeyPressed('D'))
	{
		theGSM->HandleEvents('d');
	}
	else
	{
		theGSM->HandleEvents('d', false);
	}
	if (IsKeyPressed('W'))
	{
		theGSM->HandleEvents('w');
	}
	else
	{
		theGSM->HandleEvents('w', false);
	}
	if (IsKeyPressed('S'))
	{
		theGSM->HandleEvents('s');
	}
	else
	{
		theGSM->HandleEvents('s', false);
	}
	// Jump
	if (IsKeyPressed(32))
	{
		theGSM->HandleEvents(32);
	}
	else
	{
		theGSM->HandleEvents(32, false);
	}

	if (IsKeyPressed(VK_ESCAPE))
	{
		theGSM->HandleEvents(VK_ESCAPE);
	}
	else
	{
		theGSM->HandleEvents(VK_ESCAPE, false);
	}
	// Rotate camera
	/*if (IsKeyPressed(VK_LEFT))
	{
		scene->UpdateCameraStatus(VK_LEFT);
	}
	else
	{
		scene->UpdateCameraStatus(VK_LEFT, false);
	}
	if (IsKeyPressed(VK_RIGHT))
	{
		scene->UpdateCameraStatus(VK_RIGHT);
	}
	else
	{
		scene->UpdateCameraStatus(VK_RIGHT, false);
	}
	if (IsKeyPressed(VK_UP))
	{
		scene->UpdateCameraStatus(VK_UP);
	}
	else
	{
		scene->UpdateCameraStatus(VK_UP, false);
	}
	if (IsKeyPressed(VK_DOWN))
	{
		scene->UpdateCameraStatus(VK_DOWN);
	}
	else
	{
		scene->UpdateCameraStatus(VK_DOWN, false);
	}*/
    return true;
}

/********************************************************************************
 Constructor
 ********************************************************************************/
Application::Application()
	: scene(NULL),
	theGSM(NULL)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
Application::~Application()
{
	if (theGSM)
	{
		delete theGSM;
		theGSM = NULL;
	}
}

/********************************************************************************
 Initialise this program
 ********************************************************************************/
void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "Y2S2_Framework", NULL, NULL);
	glfwSetWindowPos(m_window, 100, 8);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set these 2 variables to zero
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	theGSM = new CGameStateManager();
	theGSM->Init("DM2240 with GSM", 800, 600);
	theGSM->ChangeState(CPlayState::Instance());
}

/********************************************************************************
 Run this program
 ********************************************************************************/
void Application::Run()
{
	//#if TYPE_OF_VIEW == 3
	//	scene = new CSceneManager(m_window_width, m_window_height);	// Use this for 3D gameplay
	//#else
	//	scene = new CSceneManager2D(m_window_width, m_window_height);	// Use this for 2D gameplay
	//#endif
	//scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && active)
	{
		// Get the elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		if (m_dAccumulatedTime_ThreadOne > 0.03)
		{
			GetMouseUpdate();
			GetKeyboardUpdate();
			theGSM->HandleEvents();
			theGSM->Update(m_dElapsedTime);
			//scene->Update(m_dElapsedTime);
			Application::mouseScroll = 0;

			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 1.0)
		{
			//UpdateAI();
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		// Render the scene
		theGSM->Draw();
		//scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed

	theGSM->Cleanup();
	// Delete the scene
	//scene->Exit();
	delete scene;
}

/********************************************************************************
 Exit this program
 ********************************************************************************/
void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}