/******************************************************************************
Class:SceneNode
Implements:
Author:Rich Davison
Description: A subset of the SceneNode class you saw in the Graphics for Games
module. Extend this as you wish! Nothing here you haven't seen before...

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once

#include <vector>
#include <vectormath/cpp/vectormath_aos.h>
#include <iostream>

class Mesh; 

using namespace Vectormath::Aos;

class SceneNode	{
public:
	SceneNode(void);
	virtual ~SceneNode(void);

	void			SetTransform(const Matrix4 &matrix) { transform = matrix; }
	const Matrix4&	GetTransform() const				{ return transform; }
	Matrix4			GetWorldTransform() const			{ return worldTransform; }

	void			SetMesh(Mesh*m)						{ mesh = m; }
	Mesh*			GetMesh()							{ return mesh; }

	virtual void	Update(float msec);

	void VectorToZero(Vector3 &v) {
		v.setX(0.0f);
		v.setY(0.0f);
		v.setZ(0.0f);
	}

	void			AddChild(SceneNode& s);
	void			MurderChildGruesomely(SceneNode& s);

	bool			getIsPlayer()						{return isPlayer;}
	void			setIsPlayer(bool p)					{ isPlayer= p;}

	int				getNumChildren()					{return children.size();}

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{return children.begin();}
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{return children.end();}

protected:
	Mesh*		mesh;
	SceneNode*	parent;
	Matrix4		worldTransform;
	Matrix4		transform;
	
	bool		isPlayer;

	std::vector<SceneNode*>		children;
};
