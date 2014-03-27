#include "PhysicsNode.h"

PhysicsNode::PhysicsNode(void)	{

	ignoringGravity = false;
	m_position = Vector3(0,0,0);
	target			= NULL;
	collidedWith	= NULL;
	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;
	radius			= 50.0f;
	m_linearVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_angularVelocity	= Vector3(0.0f, 0.0f, 0.0f);
	m_invMass			= 0.0f;
	m_invInertia		= Matrix4(
		Vector4(1/inertia,0.0f,0.0f,0.0f),
		Vector4(0.0f,1/inertia,0.0f,0.0f),
		Vector4(0.0f,0.0f,1/inertia,0.0f),
		Vector4(0.0f,0.0f,0.0f,1.0f));

	gravity			= Vector3(0.0f, GRAVITY, 0.0f);
	inAir = true;
	atRest = false;
	VectorToZero(m_force);

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
	inAir = true;
	atRest = false;

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
	inAir = true;
	atRest = false;

	VectorToZero(m_force);
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
	if (m_position.getX() > 4096 - radius) {
		m_position.setX(4096 - radius) ;
	}
	if (m_position.getZ() < radius) {
		m_position.setZ(radius) ;
	}
	if (m_position.getZ() > 4096 - radius) {
		m_position.setZ(4096 - radius) ;
	}

	/*	 Angle Fun    */
	//float inertia = 2 * m_invMass * (radius * radius);
	//Matrix4 inertiaMat = Matrix4::rotation(inertia, Vector3(1,1,1));

	Vector3 angularAcceleration = (m_invInertia * m_torque).getXYZ();
	m_angularVelocity = m_angularVelocity + angularAcceleration * msec;
	m_orientation = m_orientation + QuatByVector3(m_orientation, (m_angularVelocity * msec * 0.5f));
	m_orientation = normalize(m_orientation);

	/* No Fun Allowed */

	SetPosition(m_position);
	//SetOrientation(m_orientation);
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