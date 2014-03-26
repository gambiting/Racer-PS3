#include "PhysicsNode.h"

PhysicsNode::PhysicsNode(void)	{

	ignoringGravity = false;

	target			= NULL;
	collidedWith	= NULL;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / 20.0f;
	m_invInertia		= Matrix4();

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	radius			= 1.0f;

	VectorToZero(m_force);

}

PhysicsNode::PhysicsNode(float r) {
	
	radius = r;
	scale = Vector3(r,r,r);

	ignoringGravity = false;

	target			= NULL;
	collidedWith	= NULL;

	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / r;
	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);

	VectorToZero(m_force);
}

PhysicsNode::PhysicsNode(float r, Vector3 p) {
	radius = r;
	scale = Vector3(r,r,r);
	SetPosition(m_position);

	ignoringGravity = false;

	target			= NULL;
	
	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / r;
	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);

	VectorToZero(m_force);
}

PhysicsNode::~PhysicsNode(void) {
}

void PhysicsNode::UpdatePosition(float msec) {

	//Linear movement
	Vector3 acceleration;

	if (ignoringGravity) {
		acceleration = m_force * m_invMass;
	} else {
		acceleration = m_force * m_invMass + gravity;
	}

	m_linearVelocity = m_linearVelocity + acceleration * msec;
	m_position = m_position + m_linearVelocity * msec;
	SetPosition(m_position);

	VectorToZero(m_force);

}

void PhysicsNode::GravityOn() {
	ignoringGravity = false;
}

void PhysicsNode::GravityOff() {
	ignoringGravity = true;
}