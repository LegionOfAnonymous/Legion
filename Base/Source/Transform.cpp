#include "Transform.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

CTransform::CTransform(void)
{
	Mtx.SetToZero();
	Mtx.SetToIdentity();
	Update_Mtx.SetToZero();
	Update_Mtx.SetToIdentity();
}

CTransform::CTransform( const float dx, const float dy, const float dz )
{
	Mtx.SetToTranslation( dx, dy, dz );
}

CTransform::~CTransform(void)
{
}

void CTransform::SetTranslate( const float dx, const float dy, const float dz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation( dx, dy, dz );

	Mtx = Mtx * TempMtx;
}

void CTransform::ApplyRotate( const float angle, const float rx, const float ry, const float rz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation( angle, rx, ry, rz );
	Mtx = Mtx * TempMtx;
}

void CTransform::SetRotate(const float angle, const float rx, const float ry, const float rz)
{
	Mtx.SetToRotation(angle, rx, ry, rz);
}


void CTransform::SetScale( const float sx, const float sy, const float sz  )
{
	Mtx.SetToScale( sx, sy, sz );
}


// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}