#include "PlayInfo3PV.h"
#include "SceneNode.h"
#include "Application.h"

float AVATAR_SPEED = 10.f;

CPlayInfo3PV::CPlayInfo3PV(void)
	: theAvatarMesh(NULL)
	, jumpspeed(0)
	, steps(0)
{
	Init();
}


CPlayInfo3PV::~CPlayInfo3PV(void)
{
	if (theAvatarMesh)
	{
		delete theAvatarMesh;
		theAvatarMesh = NULL;
	}
}

// Initialise this class instance
void CPlayInfo3PV::Init(void)
{
	curPosition.Set( 0, 0, 0);
	curDirection.Set( 0, 0, 1 );
	camDirection.Set(0, 0, 1);
	vOrientation = 0;

	// Initialise the Avatar's movement flags
	for(int i=0; i<255; i++){
		myKeys[i] = false;
	}
}

// Set Model
bool CPlayInfo3PV::SetModel(CSceneNode* theAvatarMesh )
{
	this->theAvatarMesh = theAvatarMesh;
	if (this->theAvatarMesh == NULL)
		return false;

	return true;
}

// Returns true if the player is on ground
bool CPlayInfo3PV::isOnGround(void)
{
	return true;
}

// Returns true if the player is jumping upwards
bool CPlayInfo3PV::isJumpUpwards(void)
{
	return true;
}

// Returns true if the player is on freefall
bool CPlayInfo3PV::isFreeFall(void)
{
	return true;
}

// Set the player's status to free fall mode
void CPlayInfo3PV::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayInfo3PV::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayInfo3PV::SetPos_x(int pos_x)
{
	curPosition.x = pos_x;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_y(int pos_y)
{
	curPosition.y = pos_y;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_z(int pos_z)
{
	curPosition.z = pos_z;
}

// Set Jumpspeed of the player
void CPlayInfo3PV::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

void CPlayInfo3PV::AddPosition(Vector3 position)
{
	curPosition += position;
	theAvatarMesh->ApplyTranslate(0, position.x, position.y, position.z);
}

void CPlayInfo3PV::SetPosition(Vector3 position)
{
	Vector3 dir = curPosition - position;
	AddPosition(dir);
}

// Stop the player's movement
void CPlayInfo3PV::SetToStop(void)
{
	jumpspeed = 0;
}

/********************************************************************************
 Hero Move Up Down
 ********************************************************************************/
void CPlayInfo3PV::MoveFrontBack(const bool mode, const float timeDiff)
{
	Vector3 dir = curDirection; dir.y = 0;
	dir = dir.Normalized() * 100;
	float bodyOrientation = -Math::RadianToDegree(asin(theAvatarMesh->GetTransformation(2)->GetTransform().a[2]));
	float yaw = 300 * timeDiff;

	if (mode)
	{
		velocity -= dir;

		if (bodyOrientation > 0)
			RotateBodyRise(yaw, 45);
		else
			RotateBodyFall(-yaw, -45);
	}
	else
	{
		velocity += dir;

		if (!(bodyOrientation > -Math::EPSILON && bodyOrientation < Math::EPSILON))
		{
			if (bodyOrientation > 0)
				yaw = -yaw;
			
			if (bodyOrientation > 0 && bodyOrientation + yaw < 0)
			{
				theAvatarMesh->ApplyRotate(2, -bodyOrientation, 0, 1, 0);
				theAvatarMesh->GetNode(2)->ApplyRotate(0, bodyOrientation, 0, 1, 0);
			}
			else if (bodyOrientation < 0 && bodyOrientation + yaw > 0)
			{
				theAvatarMesh->ApplyRotate(2, -bodyOrientation, 0, 1, 0);
				theAvatarMesh->GetNode(2)->ApplyRotate(0, bodyOrientation, 0, 1, 0);
			}
			else
			{
				theAvatarMesh->ApplyRotate(2, yaw, 0, 1, 0);
				theAvatarMesh->GetNode(2)->ApplyRotate(0, -yaw, 0, 1, 0);
			}
		}
	}
}

/********************************************************************************
 Hero Move Left Right
 ********************************************************************************/
void CPlayInfo3PV::MoveLeftRight(const bool mode, const float timeDiff)
{
	Vector3 right = curDirection.Cross(Vector3(0, 1, 0)); right.y = 0;
	right = right.Normalized() * 100;

	if (mode)
	{
		velocity -= right;

		if (!myKeys['s'])
			RotateBodyRise(500 * timeDiff, 45);
	}
	else
	{
		velocity += right;

		if (!myKeys['s'])
			RotateBodyFall(500 * -timeDiff, -45);
	}
}

void CPlayInfo3PV::RotateBodyRise(const float by, const float until)
{
	float bodyOrientation = -Math::RadianToDegree(asin(theAvatarMesh->GetTransformation(2)->GetTransform().a[2]));

	if (bodyOrientation + by > until)
	{
		theAvatarMesh->ApplyRotate(2, until - bodyOrientation, 0, 1, 0);
		theAvatarMesh->GetNode(2)->ApplyRotate(0, -(until - bodyOrientation), 0, 1, 0);
	}
	else
	{
		theAvatarMesh->ApplyRotate(2, by, 0, 1, 0);
		theAvatarMesh->GetNode(2)->ApplyRotate(0, -by, 0, 1, 0);
	}
}

void CPlayInfo3PV::RotateBodyFall(const float by, const float until)
{
	float bodyOrientation = -Math::RadianToDegree(asin(theAvatarMesh->GetTransformation(2)->GetTransform().a[2]));

	if (bodyOrientation + by < until)
	{
		theAvatarMesh->ApplyRotate(2, until - bodyOrientation, 0, 1, 0);
		theAvatarMesh->GetNode(2)->ApplyRotate(0, -(until - bodyOrientation), 0, 1, 0);
	}
	else
	{
		theAvatarMesh->ApplyRotate(2, by, 0, 1, 0);
		theAvatarMesh->GetNode(2)->ApplyRotate(0, -by, 0, 1, 0);
	}
}


// Get position x of the player
float CPlayInfo3PV::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 CPlayInfo3PV::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 CPlayInfo3PV::GetDirection()
{
	return curDirection;
}

Vector3 CPlayInfo3PV::GetCamDirection()
{
	return camDirection;
}

// Get Jumpspeed of the player
int CPlayInfo3PV::GetJumpspeed(void)
{
	return jumpspeed;
}

// Update Jump Upwards
void CPlayInfo3PV::UpdateJumpUpwards()
{
	curPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
	}
}

// Update FreeFall
void CPlayInfo3PV::UpdateFreeFall()
{
	curPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void CPlayInfo3PV::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff)
{
	if (curPosition.x < leftBorder)
	{
		curPosition.x = leftBorder;
	}
	else if (curPosition.x > rightBorder)
	{
		curPosition.x = rightBorder;
	}

	if (curPosition.y < topBorder)
		curPosition.y = topBorder;
	else if (curPosition.y > bottomBorder)
		curPosition.y = bottomBorder;
}


/********************************************************************************
 Update Movement
 ********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CPlayInfo3PV::Update(double dt)
{
	// WASD movement
	if (myKeys['w'])
		MoveFrontBack( false, dt );
	if (myKeys['s'])
		MoveFrontBack( true, dt );
	if (myKeys['a'])
		MoveLeftRight( true, dt );
	if (myKeys['d'])
		MoveLeftRight( false, dt );
	// Jump
	if (myKeys[32] && velocity.y == 0)
	{
		velocity.y = 300;
	}

	Vector3 HorizontalVelocity = Vector3(velocity.x, 0, velocity.z);
	if (HorizontalVelocity.LengthSquared() > 100 * 100)
	{
		HorizontalVelocity.Normalize() *= 100;
		velocity.Set(HorizontalVelocity.x, velocity.y, HorizontalVelocity.z);
	}

	curPosition += velocity * dt;
	theAvatarMesh->ApplyTranslate(0, velocity.x * dt, velocity.y * dt, velocity.z * dt);

	curPosition += knockback * dt;
	theAvatarMesh->ApplyTranslate(0, knockback.x * dt, knockback.y * dt, knockback.z * dt);
	knockback += -knockback  * dt * 10;

	if (HorizontalVelocity.LengthSquared() > 10 * 10)
		velocity += -HorizontalVelocity.Normalized() * 1200 * dt;
	else
		velocity.Set(0, velocity.y, 0);

	velocity.y += -9.8f * 150 * dt;

	if (curPosition.y < 0)
		velocity.y = 0;
	
	if ((myKeys['w'] || myKeys['a'] || myKeys['s'] || myKeys['d']) && (velocity.x != 0 || velocity.z != 0))
	{
		steps += 900 * dt;

		float yaw = camDirection.Cross(curDirection.Normalized()).y * 75 * (float)(-AVATAR_SPEED * (float)dt);

		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		curDirection = rotation * curDirection;

		theAvatarMesh->ApplyRotate(1, yaw, 0, 1, 0);
	}
	else if (velocity.y != 0)
		steps += velocity.y * dt;
	else
		ResetLimbs(8 * dt);

	AnimateLimbs(45, 30);

	//Update the camera direction based on mouse move
	if ( Application::camera_yaw != 0 )
		Yaw( dt );
	if (Application::camera_pitch != 0)
		Pitch(dt);
}

void CPlayInfo3PV::AnimateLimbs(const float legAngle, const float armAngle)
{
	float curStep = cos(Math::DegreeToRadian(steps + 90)) * legAngle;

	float prevStep = Math::RadianToDegree(asin(theAvatarMesh->GetNode(3)->GetTransformation(1)->GetTransform().a[6]));
	theAvatarMesh->GetNode(3)->ApplyRotate(1, curStep - prevStep, 1, 0, 0);

	prevStep = Math::RadianToDegree(asin(theAvatarMesh->GetNode(4)->GetTransformation(1)->GetTransform().a[6]));
	theAvatarMesh->GetNode(4)->ApplyRotate(1, -curStep - prevStep, 1, 0, 0);

	curStep = cos(Math::DegreeToRadian(steps + 90)) * armAngle;

	prevStep = Math::RadianToDegree(asin(theAvatarMesh->GetNode(5)->GetTransformation(1)->GetTransform().a[6]));
	theAvatarMesh->GetNode(5)->ApplyRotate(1, -curStep - prevStep, 1, 0, 0);

	prevStep = Math::RadianToDegree(asin(theAvatarMesh->GetNode(6)->GetTransformation(1)->GetTransform().a[6]));
	theAvatarMesh->GetNode(6)->ApplyRotate(1, curStep - prevStep, 1, 0, 0);
}
void CPlayInfo3PV::ResetLimbs(const double dt)
{
	if (int(steps + 90) % 180 > 90)
	{
		steps -= abs((int(steps + 90) % 180) - 90) * dt;

		if (int(steps + 90) % 180 < 90)
			steps = 0;
	}
	else if (int(steps + 90) % 180 < 90)
	{
		steps += abs((int(steps + 90) % 180) - 90) * dt;

		if (int(steps + 90) % 180 > 90)
			steps = 0;
	}

}
void CPlayInfo3PV::Knockback(Vector3 dir)
{
	knockback += dir;
}
/********************************************************************************
Yaw. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Yaw(const double dt)
{
	Turn(Application::camera_yaw);
}
void CPlayInfo3PV::Pitch(const double dt)
{
	Look(-Application::camera_pitch);
}

void CPlayInfo3PV::Turn(const double dt)
{
	float yaw = (float)(-AVATAR_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	camDirection = rotation * camDirection;

	//float headOrientation = -Math::RadianToDegree(asin(theAvatarMesh->GetNode(2)->GetTransformation(0)->GetTransform().a[2]));

	//if (headOrientation + yaw > 45)
	//	theAvatarMesh->GetNode(2)->ApplyRotate(0, 45 - headOrientation, 0, 1, 0);
	//else if (headOrientation + yaw < -45)
	//	theAvatarMesh->GetNode(2)->ApplyRotate(0, -45 - headOrientation, 0, 1, 0);
	//else
	//	theAvatarMesh->GetNode(2)->ApplyRotate(0, yaw, 0, 1, 0);
}

void CPlayInfo3PV::Look(const double dt)
{
	float pitch = (float)(AVATAR_SPEED * (float)dt);
	if (vOrientation + pitch > 80)
	{
		pitch = 80 - vOrientation;
		vOrientation = 80;
	}
	else if (vOrientation + pitch < -80)
	{
		pitch = -80 - vOrientation;
		vOrientation = -80;
	}
	else
		vOrientation += pitch;

	Vector3 view = camDirection;
	Vector3 right = view.Cross(Vector3(0,1,0));
	right.y = 0;
	right.Normalize();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	camDirection = rotation * camDirection;

	//theAvatarMesh->GetNode(2)->ApplyRotate(2, -pitch, 1, 0, 0);
}