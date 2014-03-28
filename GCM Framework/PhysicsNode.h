#pragma once

#include "Defines.h"
#include "SceneNode.h"
#include "common.h"

#include <vector>
#include <vectormath/cpp/vectormath_aos.h>
using namespace Vectormath::Aos;
#define GRAVITY -0.00048f
#define DAMPING_FACTOR		0.97005f
#define ANGULAR_DAMPING_FACTOR 0.99999f

class PhysicsNode : public SceneNode {
public:

	PhysicsNode(void);
	PhysicsNode(float r);
	PhysicsNode(float r, Vector3 p);		//Radius (scale) & position

	virtual ~PhysicsNode(void);

	void			SetScale(Vector3 s)					{ scale = s; }
	void			SetPosition(Vector3 pos)			{ 
		m_position = pos;
		SetTransform( Matrix4::translation(m_position) * Matrix4::scale(scale) );
	}


	void			SetOrientation()			{ 
		//std::cout << "Rot: (" << m_position.getX() << ", " << m_position.getY() << ", " << m_position.getZ() << ")" << std::endl;

		SetTransform(  Matrix4::translation(m_position)* Matrix4::scale(scale) * Matrix4::rotationZYX(m_rot)) ; 
	}
	void			SetLinearVelocity(Vector3 vel)		{ m_linearVelocity = vel; }
	void			SetAngularVelocity(Vector3 ang)		{ m_angularVelocity = ang; }

	Vector3&		getForwardVec()						{ return forwardVec; }
	void			setForwardVec(Vector3& vec)			{ forwardVec = vec; }

	Vector3			GetScale() const					{ return scale; }
	Vector3			GetPosition() const					{ return m_position; }
	float			GetRadius() const					{ return radius; }
	void			setRadius(float f)					{ radius = f; }
	Matrix4			GetInvInertia() const				{ return m_invInertia; }
	Vector3			GetLinearVelocity() const			{ return m_linearVelocity; }
	Vector3			GetAngularVelocity() const			{ return m_angularVelocity; }
	float			GetInverseMass() const				{ return m_invMass; }
	Vector3			GetmRot() const						{ return m_rot;}
	Quat			QuatByVector3(const Quat &b, const Vector3 &v);

	//record a collision with a sphere for use by Objective logic
	void			SetCollidedWith(PhysicsNode* sphere){ collidedWith = sphere; }
	PhysicsNode*	GetCollidedWith()					{ return collidedWith; }

	Vector3&		GetStoredVelocity()					{return stored_Velocity;}

	bool			getAtRest()							{return atRest;}
	void			SetAtRest(bool b)					{atRest = b;}

	bool			isInAir()							{ return inAir; }
	void			SetInAir(bool a)					{ inAir = a; }

	bool			isCollidable()						{ return collidable;}
	void			setCollidable(bool b)				{ collidable = b; }

	void			AddForce(Vector3 f, Vector3 d)		{
															m_force = f;
															//distance = d;
															//m_torque = CalculateTorque(distance, m_force);
														}

	void UpdatePosition(float msec);

	void GravityOn();
	void GravityOff();

protected:

	bool ignoringGravity;
	bool collidable;

	Vector3		scale;
	Vector3		gravity;
	float		radius;
	
	//<----LINEAR---->
	Vector3		m_position;
	Vector3		m_linearVelocity;
	Vector3		m_force;
	float		m_invMass;
	Vector3		stored_Velocity;

	//<----ANGULAR---->
	Quat		m_orientation;
	Vector3		m_angularVelocity;
	Vector3		m_torque;
	Matrix4		m_invInertia;
	Vector3		forwardVec;

	Vector3		m_rot;
	
	SceneNode*	target;
	bool		atRest;
	bool		inAir;

	//for assassination game mode
	PhysicsNode* collidedWith;

};