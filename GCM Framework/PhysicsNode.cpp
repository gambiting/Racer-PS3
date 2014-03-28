#include "PhysicsNode.h"

PhysicsNode::PhysicsNode(void)	{

	float r = 15.0f;
	radius = r;
	scale = Vector3(r,r,r);
	m_position = Vector3(0,0,0);
	VectorToZero(m_rot);

	ignoringGravity = false;

	target			= NULL;
	collidedWith	= NULL;

	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_angularVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / r;

	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	atRest = false;

	VectorToZero(m_force);
	VectorToZero(m_rot);
	m_orientation.identity();

}

PhysicsNode::PhysicsNode(float r) {
	
	radius = r;
	scale = Vector3(r,r,r);
	m_position = Vector3(0,0,0);

	ignoringGravity = false;

	target			= NULL;
	collidedWith	= NULL;

	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;

	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_angularVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / r;
	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	atRest = false;

	m_orientation.identity();
	VectorToZero(m_rot);
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
	m_angularVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 1.0f / r;
	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	atRest = false;

	m_orientation.identity();
	VectorToZero(m_rot);
	VectorToZero(m_force);

	m_rot = Vector3(0,0,0);


}

PhysicsNode::~PhysicsNode(void) {
}

void PhysicsNode::UpdatePosition(float msec) {

	if (!atRest) {

		//Linear movement
		Vector3 acceleration;

		if (ignoringGravity) {
			acceleration = m_force * m_invMass;
		} else {
			acceleration = m_force * m_invMass + gravity;
		}

		m_linearVelocity = m_linearVelocity + acceleration * msec;
		m_linearVelocity = m_linearVelocity * DAMPING_FACTOR;
		m_position = m_position + m_linearVelocity * msec;

		float totalV = m_linearVelocity.getX() + m_linearVelocity.getY() + m_linearVelocity.getZ();
		
		if (totalV > 0.005f) {
			//atRest = true;
		} 

	}

	if (m_position.getX() < radius) {
		m_position.setX(radius) ;
	}
	if (m_position.getX() > 4096 - (radius * 2)) {
		m_position.setX(4096 - (radius * 2)) ;
	}
	if (m_position.getZ() < radius) {
		m_position.setZ(radius) ;
	}
	if (m_position.getZ() > 4096 - (radius * 2)) {
		m_position.setZ(4096 - (radius * 2)) ;
	}

	/*	 Angle Fun    */
	Vector3 angularAcceleration = (m_invInertia * m_torque).getXYZ();
	m_angularVelocity = m_angularVelocity + angularAcceleration * msec;
	m_angularVelocity = m_angularVelocity * DAMPING_FACTOR;

	m_rot.setX(m_rot.getX() + m_angularVelocity.getX());
	m_rot.setY(m_rot.getY() + m_angularVelocity.getY());
	m_rot.setZ(m_rot.getZ() + m_angularVelocity.getZ());

	if(m_rot.getX() > 360) m_rot.setX(m_rot.getX() - 360);
	if(m_rot.getY() > 360) m_rot.setX(m_rot.getY() - 360);
	if(m_rot.getZ() > 360) m_rot.setX(m_rot.getZ() - 360);
	/* No Fun Allowed */

	SetPosition(m_position);
	SetOrientation();
	VectorToZero(m_force);
	VectorToZero(m_torque);

}

void PhysicsNode::GravityOn() {
	ignoringGravity = false;
}

void PhysicsNode::GravityOff() {
	ignoringGravity = true;
}

Quat PhysicsNode::QuatByVector3(const Quat &q, const Vector3 &v) {
	Quat ans;

	ans.setW( -(q.getX() * v.getX()) - (q.getY() * v.getY()) - (q.getZ() * v.getZ()) );
	ans.setX(  (q.getW() * v.getX()) + (q.getY() * v.getZ()) - (q.getZ() * v.getY()) );
	ans.setY(  (q.getW() * v.getY()) + (q.getZ() * v.getX()) - (q.getX() * v.getZ()) );
	ans.setZ(  (q.getW() * v.getZ()) + (q.getX() * v.getY()) - (q.getY() * v.getX()) );

	return ans;
}

bool PhysicsNode::isInAir() {
	
}