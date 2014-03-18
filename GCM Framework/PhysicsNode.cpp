#include "PhysicsNode.h"

PhysicsNode::PhysicsNode(void)	{

	target			= NULL;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / 20.0f;
	m_invInertia		= Matrix4();

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	radius			= 1.0f;

}

PhysicsNode::~PhysicsNode(void) {
}