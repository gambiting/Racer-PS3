/******************************************************************************
Class:PhysicsNode
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk> and YOU!
Description: This class represents the physical properties of your game's
entities - their position, orientation, mass, collision volume, and so on.
Most of the first few tutorials will be based around adding code to this class
in order to add correct physical integration of velocity / acceleration etc to
your game objects. 


In addition to the tutorial code, this class contains a pointer to a SceneNode.
This pointer is to the 'graphical' representation of your game object, just 
like the SceneNode's used in the graphics module. However, instead of 
calculating positions etc as part of the SceneNode, it should instead be up
to your 'physics' representation to determine - so we calculate a transform
matrix for your SceneNode here, and apply it to the pointer. 

Your SceneNode should still have an Update function, though, in order to
update any graphical effects applied to your object - anything that will end
up modifying a uniform in a shader should still be the job of the SceneNode. 
fesent our
entire CubeRobot with a single PhysicsNode, and a single SceneNode 'root'.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SceneNode.h"
#include "Camera.h"
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

#define GRAVITY -0.00098f

class PhysicsNode	{
public:
	PhysicsNode(void);
	PhysicsNode(Quat orientation, Vector3 position);
	PhysicsNode(bool gravy);
	virtual ~PhysicsNode(void);

	Vector3		GetPosition()			{ return m_position;}
	Vector3		GetLinearVelocity()		{ return m_linearVelocity;}
	Matrix4		GetInverseInertia()		{ return m_invInertia; }

	void SetLinearVelocity(Vector3 velocity) { m_linearVelocity = velocity; }
	void SetAngularVelocity(Vector3 velocity) { m_angularVelocity = velocity; }

	void SetRadius(float r) {radius = r;}
	float GetRadius() {return radius;}
	
	float		GetInverseMass()		{return m_invMass;}

	Quat	GetOrientation()		{ return m_orientation;}
	Vector3		GetAngularVelocity()	{ return m_angularVelocity;}

	void SetPosition(Vector3 p) { m_position = p; }

	Matrix4		BuildTransform();

	virtual void		Update(float msec);

	virtual void UpdatePosition(float msec, Camera camera);

	void	SetTarget(SceneNode *s) { target = s;}

	Vector3	GetForce()	{ return m_force;}
	void SetInverseMass(float m) { m_invMass = m; }
	void SetForce(Vector3 f) { m_force = f; }
	void SetForce(Vector3 f, Vector3 relPos) 
	{ 
		m_torque = cross(relPos, f);
	}

	Vector3	GetTorque() { return m_torque;}
	Vector3 GetGravity() { return gravity; }

	void IgnoreGrav(bool g) { ignoreGravity = g; }
	bool IsIgnoringGravity() { return ignoreGravity; }

protected:
	Vector3 gravity;
	float radius;
	bool ignoreGravity;

	//<---------LINEAR-------------->
	Vector3		m_position;
	Vector3		m_linearVelocity;
	Vector3		m_force;
	float		m_invMass;

	//<----------ANGULAR--------------->
	Quat  m_orientation;
	Vector3		m_angularVelocity;
	Vector3		m_torque;
	Matrix4     m_invInertia;

	SceneNode*	target;  

};

