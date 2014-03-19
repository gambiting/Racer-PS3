#include "SceneNode.h"
#include "Camera.h"

#pragma once

#include <vector>
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

#define GRAVITY -0.00098f

class PhysicsNode : public SceneNode {
public:

	PhysicsNode(void);
	PhysicsNode(float r);
	PhysicsNode(float r, Vector3 p);		//Radius (scale) & position

	virtual ~PhysicsNode(void);

	void			SetScale(Vector3 s)					{ scale = s; }
	void			SetPosition(Vector3 pos)			{ 
		m_position = pos;
		SetTransform(Matrix4::translation(m_position) * Matrix4::scale(scale));
	}
	void			SetLinearVelocity(Vector3 vel)		{m_linearVelocity = vel; }

	Vector3			GetScale() const					{ return scale; }
	Vector3			GetPosition() const					{ return m_position; }

	void UpdatePosition(float msec);

	void GravityOn();
	void GravityOff();

protected:

	bool ignoringGravity;

	Vector3		scale;
	Vector3		gravity;
	float		radius;
	
	//<----LINEAR---->
	Vector3		m_position;
	Vector3		m_linearVelocity;
	Vector3		m_force;
	float		m_invMass;

	//<----ANGULAR---->
	Quat		m_orientation;
	Vector3		m_angularVelocity;
	Vector3		m_torque;
	Matrix4		m_invInertia;

	SceneNode* target;
};