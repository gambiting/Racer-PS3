#include "PhysicsNode.h"

PhysicsNode::PhysicsNode(void)	{

	target			= NULL;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / 20.0f;
	m_invInertia		= Matrix4();

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	radius			= 1.0f;

}

PhysicsNode::PhysicsNode(Vector3 s, Vector3 p) {
	scale = s;
	position = p;
	SetPosition(position);
}

PhysicsNode::~PhysicsNode(void) {
}

void PhysicsNode::UpdatePosition(float msec) {

	//Linear movement
	Vector3 acceleration;

	acceleration = m_force * m_invMass  + gravity;


}