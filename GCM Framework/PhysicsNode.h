#pragma once

#include <vectormath/cpp/vectormath_aos.h>
#include "SceneNode.h"
#include "Camera.h"

#define GRAVITY -0.00098f

using namespace Vectormath::Aos;

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

	Quat		GetOrientation()		{ return m_orientation;}
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

protected:
	Vector3 gravity;
	float radius;

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