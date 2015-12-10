#pragma once
#include "node.h"
#include "Mtx44.h"
#include "MatrixStack.h"
class CSceneNode;

class CTransform :
	public CNode
{
private:
	Mtx44 Mtx, Update_Mtx;

public:
	CTransform(void);
	CTransform( const float dx, const float dy, const float dz  );
	~CTransform(void);

	void SetTranslate( const float dx, const float dy, const float dz  );
	void ApplyRotate( const float angle, const float rx, const float ry, const float rz  );
	void SetRotate(const float angle, const float rx, const float ry, const float rz);
	void SetScale( const float sx, const float sy, const float sz  );

	Mtx44 GetTransform(void);
};
