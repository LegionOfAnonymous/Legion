#pragma once
#include "Vector3.h"

class CSceneNode;

class CPlayInfo3PV
{
public:
	CPlayInfo3PV(void);
	virtual ~CPlayInfo3PV(void);

	enum GEOMETRY_TYPE
	{
		GEO_AVATAR,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);

	// Set Model
	bool SetModel(CSceneNode* theAvatarMesh );

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set position z of the player
	void SetPos_z(int pos_y);	
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveFrontBack(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	// Get position x of the player
	float GetPos_x(void);
	// Get position y of the player
	float GetPos_y(void);
	// Get position z of the player
	float GetPos_z(void);
	// Get position of the player
	Vector3 GetPosition();
	// Get direction of the player
	Vector3 GetDirection();
	Vector3 GetCamDirection();
	// Get Jumpspeed of the player
	int GetJumpspeed(void);

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Update
	void UpdateMovement(const unsigned char key, const bool status = true);
	// Update
	void Update(double dt);

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
					   const int topBorder, const int bottomBorder, 
					   float timeDiff);

	// Avatar's Mesh
	CSceneNode*	theAvatarMesh;
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);	
	virtual void Turn(const double dt);
	virtual void Look(const double dt);

	void RotateBodyRise(const float by, const float until);
	void RotateBodyFall(const float by, const float until);

	void AnimateLimbs(const float legAngle = 40, const float armAngle = 40);
	void ResetLimbs(const double dt);

private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curDirection;
	Vector3 camDirection;
	Vector3 velocity;
	int jumpspeed;
	float steps;

	float vOrientation;
	bool myKeys[255];
};

