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

class Mesh; 
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

#define GRAVITY -0.00098f

class SceneNode	{
public:
	SceneNode(void);
	virtual ~SceneNode(void);

	void			SetTransform(const Matrix4 &matrix) { transform = matrix; } 
	void			SetScale(Vector3 s)					{ scale = s; }
	void			SetPosition(Vector3 pos)			{ 
		position = pos;
		SetTransform(Matrix4::translation(position) * Matrix4::scale(scale));
	}

	const Matrix4&	GetTransform() const				{ return transform; }
	Matrix4			GetWorldTransform() const			{ return worldTransform; }
	Vector3			GetScale() const					{ return scale; }
	Vector3			GetPosition() const					{ return position; }

	void			SetMesh(Mesh*m)						{ mesh = m; }
	Mesh*			GetMesh()							{ return mesh; }

	virtual void	Update(float msec);


	void			AddChild(SceneNode& s);

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{return children.begin();}
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{return children.end();}

protected:

	Mesh*		mesh;
	SceneNode*	parent;
	Matrix4		worldTransform;
	Matrix4		transform;
	Vector3		scale;
	Vector3		position;

	std::vector<SceneNode*>		children;

	Vector3 gravity;
	float radius;
	
	//<----LINEAR---->
	Vector3 m_position;
	Vector3 m_linearVelocity;
	Vector3 m_force;
	float m_invMass;

	//<----ANGULAR---->
	Quat m_orientation;
	Vector3 m_angularVelocity;
	Vector3 m_torque;
	Matrix4 m_invInertia;

	SceneNode* target;
};
