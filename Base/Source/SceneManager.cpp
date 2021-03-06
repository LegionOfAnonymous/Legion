#include "SceneManager.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "ai.h"
#include "GManager.h"

CSceneManager::CSceneManager(void)
	: m_cMinimap(NULL)
	, m_cAvatar(NULL)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
	, m_window_width(800)
	, m_window_height(600)
{
	showderView.Set(-10.f, 0.f, 0.f);
}

CSceneManager::CSceneManager(const int m_window_width, const int m_window_height)
	: m_cMinimap(NULL)
	, m_cAvatar(NULL)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
	showderView.Set(-10.f, 0.f, 0.f);
}

CSceneManager::~CSceneManager(void)
{
	if (m_cAvatar)
	{
		delete m_cAvatar;
		m_cAvatar = NULL;
	}
	if (m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
	if (m_cSceneGraph)
	{
		delete m_cSceneGraph;
		m_cSceneGraph = NULL;
	}
	if (m_cSpatialPartition)
	{
		delete m_cSpatialPartition;
		m_cSpatialPartition = NULL;
	}
}

void CSceneManager::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	m_parameters[U_INVERSE_VIEW] = glGetUniformLocation(m_programID, "iV");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_SPATIAL] = glGetUniformLocation(m_programID, "sp");
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0.75f, 1, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//text.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//text.tga", GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//chair.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_OBJECT]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 18, 36, 10.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureID = LoadTGA("Image//grass_darkgreen.tga", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureID = LoadTGA("Image//grass_lightgreen.tga", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

	// Load the texture for minimap
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("MINIMAP", Color(1, 1, 1), 1.f));
	m_cMinimap->GetBackground()->textureID = LoadTGA("Image//grass_darkgreen.tga");
	m_cMinimap->SetBorder( MeshBuilder::GenerateMinimapBorder("MINIMAPBORDER", Color(1, 1, 0), 1.f) );
	m_cMinimap->SetAvatar( MeshBuilder::GenerateMinimapAvatar("MINIMAPAVATAR", Color(1, 1, 0), 1.f) );

	CModel* newModel;
	CTransform* newTransform;

	GLuint texture = LoadTGA("Image//surgeon.tga", GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	Mesh* Body = MeshBuilder::GenerateOBJ("Body", "OBJ//Body.obj"); Body->textureID = texture;
	Mesh* Head = MeshBuilder::GenerateOBJ("Head", "OBJ//Head.obj"); Head->textureID = texture;
	Mesh* LeftLeg = MeshBuilder::GenerateOBJ("LeftLeg", "OBJ//LeftLeg.obj"); LeftLeg->textureID = texture;
	Mesh* RightLeg = MeshBuilder::GenerateOBJ("RightLeg", "OBJ//RightLeg.obj"); RightLeg->textureID = texture;
	Mesh* LeftArm = MeshBuilder::GenerateOBJ("LeftArm", "OBJ//LeftArm.obj"); LeftArm->textureID = texture;
	Mesh* RightArm = MeshBuilder::GenerateOBJ("RightArm", "OBJ//RightArm.obj"); RightArm->textureID = texture;

	CSceneNode* sNode = new CSceneNode();
	newModel = new CModel();
	newModel->Init(Body);
	newTransform = new CTransform();
	newTransform->SetTranslate(0, 0, 0);
	cout << sNode->SetNode(newTransform, newModel) << endl;

	newTransform = new CTransform();
	newTransform->SetRotate(0, 0, 1, 0);
	sNode->AddTransformation(newTransform);

	newTransform = new CTransform();
	newTransform->SetRotate(0, 0, 1, 0);
	sNode->AddTransformation(newTransform);

	newTransform = new CTransform();
	newTransform->SetScale(15, 15, 15);
	sNode->AddTransformation(newTransform);

	newModel = new CModel();
	newModel->Init(Head);
	newTransform = new CTransform();
	newTransform->ApplyRotate(0, 0, 1, 0);
	sNode->AddChild(2, newTransform, newModel);
	newTransform = new CTransform(0, 1.5f, 0);
	sNode->GetNode(2)->AddTransformation(newTransform);
	newTransform = new CTransform();
	newTransform->SetRotate(0,1,0,0);
	sNode->GetNode(2)->AddTransformation(newTransform);
	newTransform = new CTransform(0, -1.5f, 0);
	sNode->GetNode(2)->AddTransformation(newTransform);

	newModel = new CModel();
	newModel->Init(LeftLeg);
	sNode->AddChild(3, new CTransform(0, 0.75f, 0), newModel);
	newTransform = new CTransform();
	newTransform->SetRotate(0, 1, 0, 0);
	sNode->GetNode(3)->AddTransformation(newTransform);
	newTransform = new CTransform(0, -0.75f, 0);
	sNode->GetNode(3)->AddTransformation(newTransform);

	newModel = new CModel();
	newModel->Init(RightLeg);
	sNode->AddChild(4, new CTransform(0, 0.75f, 0), newModel);
	newTransform = new CTransform();
	newTransform->SetRotate(0, 1, 0, 0);
	sNode->GetNode(4)->AddTransformation(newTransform);
	newTransform = new CTransform(0, -0.75f, 0);
	sNode->GetNode(4)->AddTransformation(newTransform);

	newModel = new CModel();
	newModel->Init(LeftArm);
	sNode->AddChild(5, new CTransform(0, 1.5f, 0), newModel);
	newTransform = new CTransform();
	newTransform->SetRotate(0, 1, 0, 0);
	sNode->GetNode(5)->AddTransformation(newTransform);
	newTransform = new CTransform(0, -1.5f, 0);
	sNode->GetNode(5)->AddTransformation(newTransform);

	newModel = new CModel();
	newModel->Init(RightArm);
	sNode->AddChild(6, new CTransform(0, 1.5f, 0), newModel);
	newTransform = new CTransform();
	newTransform->SetRotate(0, 1, 0, 0);
	sNode->GetNode(6)->AddTransformation(newTransform);
	newTransform = new CTransform(0, -1.5f, 0);
	sNode->GetNode(6)->AddTransformation(newTransform);

	m_cAvatar = new CPlayInfo3PV();
	m_cAvatar->SetModel(sNode);

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(70.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
	elapsedTime = 0;

	bLightEnabled = true;

	GM = new GManager();

	for (int i = 0; i < 500; ++i)
	{
		GOBJ* g = new GOBJ();
		g->t.Set(rand() % 801 - 400, 0, rand() % 801 - 400);
		g->s.Set(3, 3, 3);
		g->rad = 3 * 10;
		g->m = GEO_SPHERE;
		GM->G.push_back(g);
	}

	/*for (int i = 0; i < 500; ++i)
	{ 
		ai* g = new ai();
		g->t.Set(0, 0, 0);
		g->s.Set(3, 3, 3);
		g->rad = 3 * 10;
		g->m = GEO_CONE;
		GM->G.push_back(g);
	}*/

	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->player = m_cAvatar;
	m_cSpatialPartition->GM = GM;
	m_cSpatialPartition->Init(100, 100, 3, 3);
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); ++i)
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); ++j)
		{
			m_cSpatialPartition->SetGridMesh(i, j, MeshBuilder::GenerateQuad("Grid", Color(1.0f / i, 0, 1.0f / j), 100));
		}
	m_cSpatialPartition->PrintSelf();
	m_cSpatialPartition->AddObject(m_cSceneGraph);
}

void CSceneManager::Update(double dt)
{
	if(Application::IsKeyPressed('R'))
		glUniform1i(m_parameters[U_SPATIAL], true);
	else
		glUniform1i(m_parameters[U_SPATIAL], false);

	static bool switchV = false;
	static bool switchV2 = false;
	if (Application::IsKeyPressed('1') && switchV == false)
		switchV = true;
	if (Application::IsKeyPressed('2') && switchV2 == false)
		switchV2 = true;

	if (switchV)
	{
		showderView.x += dt * 200.f;

		if (showderView.x  > 0.f)
		{
			showderView.x = 0.f;
			switchV = false;
		}
	}

	if (switchV2)
	{
		showderView.x -= dt * 200.f;

		if (showderView.x  < -10.f)
		{
			showderView.x = -10.f;
			switchV2 = false;
		}
	}

	rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

	m_cAvatar->Update(dt);
	camera.UpdatePosition(Vector3(m_cAvatar->GetPos_x(), m_cAvatar->GetPos_y() + 25, m_cAvatar->GetPos_z()), m_cAvatar->GetCamDirection());
	camera.Zoom(-Application::mouseScroll * 200 * dt);
	//camera.Update(dt);

	fps = (float)(1.f / dt);
	
	static bool q = false;

	if (!q && Application::IsKeyPressed('Q'))
	{
		q = true;

		GOBJ* g = GM->Fetch();
		g->t.Set(m_cAvatar->GetPos_x(), m_cAvatar->GetPos_y() + 25, m_cAvatar->GetPos_z());
		g->v = m_cAvatar->GetDirection() * 1000;
		g->s.Set(0.5f, 0.5f, 0.5f);
		g->rad = 0.5f * 10;
		g->m = GEO_SPHERE;
		g->id = GOBJ::g_proj;
	}
	else if (q && !Application::IsKeyPressed('Q'))
		q = false;

	GM->Update(dt);
	m_cSpatialPartition->Update();

	elapsedTime += dt;
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void CSceneManager::UpdateCameraStatus(const unsigned char key, const bool status)
{
	camera.UpdateStatus(key, status);
}

void CSceneManager::UpdateAvatarStatus(const unsigned char key, const bool status)
{
	m_cAvatar->UpdateMovement(key, status);
}


/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void CSceneManager::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		// Add a bullet object which starts at the camera position and moves in the camera's direction
	}
}

/********************************************************************************
 Render text onto the screen with reference position in the middle of the image
 ********************************************************************************/
void CSceneManager::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation( (i*0.8f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
 Render a mesh in 2D
 ********************************************************************************/
void CSceneManager::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);

	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void CSceneManager::PreRendering(Mesh* mesh, Mtx44 Mtx, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.MultMatrix(Mtx);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	modelStack.PopMatrix();	

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
}

void CSceneManager::PostRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0); 
}

/********************************************************************************
 Render a mesh
 ********************************************************************************/
void CSceneManager::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderGUI()
{
	// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	// Render the crosshair
	// Note that Ortho is set to this size -> 	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	RenderMeshIn2D( m_cMinimap->GetAvatar(), false, 20.0f, 68, -48, true);
	RenderMeshIn2D( m_cMinimap->GetBorder(), false, 20.0f, 68, -48);
	RenderMeshIn2D( m_cMinimap->GetBackground(), false, 20.0f, 68, -48);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);


	ostringstream s;
	s << GM->point;
	RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 32, 0, 60);
}

/********************************************************************************
 Render mobile objects
 ********************************************************************************/
void CSceneManager::RenderMobileObjects()
{
	// Render LightBall
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0,0,0);
	//RenderMesh(m_cAvatar->theAvatarMesh->GetModel()->GetMesh(), false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 1.5f, 0);
	//modelStack.Rotate(45, 1, 0, 0);
	//modelStack.Translate(0, -1.5f, 0);
	//RenderMesh(m_cAvatar->theAvatarMesh->GetNode(2)->GetModel()->GetMesh(), false);
	//modelStack.PopMatrix();
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderFixedObjects()
{
	RenderMesh(meshList[GEO_AXES], false);
}

/********************************************************************************
 Render the lights in this scene
 ********************************************************************************/
void CSceneManager::RenderLights()
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
}

/********************************************************************************
 Render the ground in this scene
 ********************************************************************************/
void CSceneManager::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, 0.5f);
	modelStack.Rotate(-90, 0, 0, 1);

	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); ++i)
	for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); ++j)
	{
		//if (m_cSpatialPartition->GetGrid(i, j).ListOfObjects.empty())
			//continue;

		modelStack.PushMatrix();
		modelStack.Translate(m_cSpatialPartition->xGridSize	* i, m_cSpatialPartition->yGridSize * j, 0);
		RenderMesh(m_cSpatialPartition->GetGridMesh(i, j), false);
		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();
}

/********************************************************************************
 Render the skybox in this scene
 ********************************************************************************/
void CSceneManager::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective, translation;
	translation.SetToIdentity(); translation.SetToTranslation(showderView.x, showderView.y, showderView.z);
	perspective.SetToPerspective(70.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective * translation);
	
	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(	camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z );
	glUniformMatrix4fv(m_parameters[U_INVERSE_VIEW], 1, GL_FALSE, &viewStack.Top().GetInverse().a[0]);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLights();
	//RenderGround();
	//RenderSkybox();
	RenderFixedObjects();
	//RenderMobileObjects();

	if (m_cSceneGraph)
		m_cSceneGraph->Draw(this);
	m_cAvatar->theAvatarMesh->Draw(this);

	for (auto& g : GM->G)
	{
		if (g->a)
		{
			modelStack.LoadMatrix(g->GetTransform());
			RenderMesh(meshList[g->m], true);
		}
	}
	modelStack.LoadIdentity();

	RenderGUI();
}

/********************************************************************************
 Exit process for this scene
 ********************************************************************************/
void CSceneManager::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
