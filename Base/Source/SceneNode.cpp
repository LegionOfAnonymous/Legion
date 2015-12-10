#include "SceneNode.h"
#include "SceneManager.h"
#include "Vector3.h"

#include <cstdlib>

CSceneNode::CSceneNode(void)
: theModel( NULL )
, theChildren( NULL )
, theTransform ( NULL )
, sceneNodeID (-1)
{
}

CSceneNode::CSceneNode(const int sceneNodeID)
{
	CSceneNode();
	SetSceneNodeID( sceneNodeID );
}


CSceneNode::~CSceneNode(void)
{
	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = theChildren[i];
			delete aChild;
			aChild = NULL;
		}
		theChildren.clear();
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}

	while (!theTransform.empty())
		{
			delete theTransform.back();
			theTransform.pop_back();
		}
}

MS ModelStack;

void CSceneNode::Draw(void)
{
	if (!theTransform.empty())
	{
		ModelStack.PushMatrix();

		for (unsigned i = 0; i < theTransform.size(); ++i)
			ModelStack.MultMatrix(theTransform[i]->GetTransform());

		if (theModel)
			theModel->Draw();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i=0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				aChild->Draw();
			}
		}

		ModelStack.PopMatrix();
	}
}

void CSceneNode::Draw(CSceneManager* theSceneManager)
{
	if (!theTransform.empty())
	{
		ModelStack.PushMatrix();

		for (unsigned i = 0; i < theTransform.size(); ++i)
			ModelStack.MultMatrix(theTransform[i]->GetTransform());

		theSceneManager->PreRendering(theModel->GetMesh(), ModelStack.Top(), true);
		if (theModel)
			theModel->Draw();
		theSceneManager->PostRendering();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i=0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				((CSceneNode*)aChild)->Draw(theSceneManager);
			}
		}


		ModelStack.PopMatrix();
	}
}

void CSceneNode::AddTransformation(CTransform* aNewTransform)
{
	theTransform.push_back(aNewTransform);
}

void CSceneNode::SetModel(CModel* theModel)
{
	this->theModel = theModel;
}

int CSceneNode::SetNode(CTransform* aNewTransform, CModel* aNewModel)
{
	this->theModel = aNewModel;
	this->theTransform.push_back(aNewTransform);

	// Set sceneNodeID to 1 as this is the root node
	sceneNodeID = 1;
	return sceneNodeID;
}

void CSceneNode::SetSceneNodeID(const int sceneNodeID)
{
	this->sceneNodeID = sceneNodeID;
}

int CSceneNode::AddChild(CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode();
	aNewNode->SetNode( aNewTransform, aNewModel );
	aNewNode->SetSceneNodeID( sceneNodeID*10 + (theChildren.size()+1) );
	theChildren.push_back( aNewNode );

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode( sceneNodeID );
	aNewNode->SetNode( aNewTransform, aNewModel );
	theChildren.push_back( aNewNode );
	aNewNode->SetSceneNodeID(sceneNodeID);

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::GetSceneNodeID(void)
{
	return sceneNodeID;
}

CSceneNode* CSceneNode::GetNode(const int sceneNodeID)
{
	if (this->sceneNodeID == sceneNodeID)
		return this;

	CSceneNode* theTarget = NULL;
	if (theChildren.size() != 0)
	{
		CSceneNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = (CSceneNode*)theChildren[i];

			theTarget = ((CSceneNode*)aChild)->GetNode( sceneNodeID );
			if (theTarget != NULL)
				break;
		}
	}

	return theTarget;
}

CModel * CSceneNode::GetModel()
{
	return theModel;
}

CTransform * CSceneNode::GetTransformation(unsigned i)
{
	return theTransform[i];
}

void CSceneNode::ApplyTranslate( unsigned index, const float dx, const float dy, const float dz )
{
	if (index < theTransform.size())
		theTransform[index]->SetTranslate( dx, dy, dz );
}


void CSceneNode::ApplyRotate(unsigned index, const float angle, const float rx, const float ry, const float rz)
{
	if (index < theTransform.size())
	{
		// Then rotate the children
		theTransform[index]->ApplyRotate(angle, rx, ry, rz);
	}
}


void CSceneNode::SetRotate(unsigned index, const float angle, const float rx, const float ry, const float rz)
{
	if (index < theTransform.size())
	{
		// Then rotate the children
		theTransform[index]->SetRotate(angle, rx, ry, rz);
	}
}

// Get top left corner of the group
Vector3 CSceneNode::GetTopLeft(void)
{
	if (theTransform.empty())
		return Vector3( theModel->GetTopLeft().x, 
						 theModel->GetTopLeft().y, 
						 theModel->GetTopLeft().z);
	else
	{
	//	return (theTransform[0]->GetTransform() * theModel->GetTopLeft());
		Mtx44& mat = theTransform[0]->GetTransform();
		return Vector3(mat.a[12] + theModel->GetTopLeft().x, mat.a[13] + theModel->GetTopLeft().y, mat.a[14] + theModel->GetTopLeft().z);
	}
}

// Get bottom right corner of the group
Vector3 CSceneNode::GetBottomRight(void)
{
	if (theTransform.empty())
		return Vector3( theModel->GetBottomRight().x, 
						 theModel->GetBottomRight().y, 
						 theModel->GetBottomRight().z);
	else
	{
		//return (theTransform[0]->GetTransform() * theModel->GetBottomRight());
		Mtx44& mat = theTransform[0]->GetTransform();
		return Vector3(mat.a[12] + theModel->GetBottomRight().x, mat.a[13] + theModel->GetBottomRight().y, mat.a[14] + theModel->GetBottomRight().z);
	}
		
}

void CSceneNode::SetColor(const float red, const float green, const float blue)
{
	theModel->SetColor(red, green, blue);
}

// Return the number of children in this group
int CSceneNode::GetNumOfChild(void)
{
	return (int)theChildren.size();
}

// Get top left corner of a child
bool CSceneNode::GetTopLeft(const int m_iChildIndex, Vector3& Vector3_TopLeft)
{
	Vector3_TopLeft = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_TopLeft = theTransform[0]->GetTransform() * aChild->GetTopLeft();
			return true;
		}
	}
	return false;
}

// Get bottom right corner of a child
bool CSceneNode::GetBottomRight(const int m_iChildIndex, Vector3& Vector3_BottomRight)
{
	Vector3_BottomRight = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_BottomRight = theTransform[0]->GetTransform() * aChild->GetBottomRight();
			return true;
		}
	}
	return false;
}

void CSceneNode::SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue)
{
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			aChild->SetColor(red, green, blue);
		}
	}
}
