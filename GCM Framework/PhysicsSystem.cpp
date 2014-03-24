#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(void) {
}

PhysicsSystem::~PhysicsSystem(void) {
}

bool PhysicsSystem::SphereSphereCollision(const PhysicsNode &p0, const PhysicsNode &p1, CollisionData *collisionData) const {

	Vector3 pos0 = p0.GetPosition();
	Vector3 pos1 = p1.GetPosition();

	float xSq = (pos1.getX() - pos0.getX()) * (pos1.getX() - pos0.getX());
	float ySq = (pos1.getY() - pos0.getY()) * (pos1.getY() - pos0.getY());
	float zSq = (pos1.getZ() - pos0.getZ()) * (pos1.getZ() - pos0.getZ());

	float distSq = (xSq + ySq + zSq);

	float sumRadius = (p0.GetRadius() + p1.GetRadius());

	if (distSq < sumRadius * sumRadius) {
		if (collisionData) {
			
			collisionData->m_penetration = sumRadius - sqrtf(distSq);
			
			collisionData->m_normal = pos0 - pos1;
			normalize(collisionData->m_normal);

			collisionData->m_point = pos0 - collisionData->m_normal * (p0.GetRadius() - collisionData->m_penetration * 0.5f);

		}
		return true; //Collision happened
	}
	return false; //Nah It didn't.

}

bool PhysicsSystem::PointInConvexPolygon(const Vector3 testPosition, Vector3 *convexShapePoints, int numPointsL) const {
	return false;
}

void PhysicsSystem::AddCollisionImpulse(PhysicsNode &p0, PhysicsNode &p1, const Vector3 &hitPoint, const Vector3 &normal, float penetration) {

	Matrix4 worldInvInertia0 = p0.GetInvInertia();
	Matrix4 worldInvInertia1 = p1.GetInvInertia();

	Vector3 r0 = hitPoint - p0.GetPosition();
	Vector3 r1 = hitPoint - p1.GetPosition();

	Vector3 v0 = p0.GetLinearVelocity() + cross(p0.GetAngularVelocity(), r0);
	Vector3 v1 = p1.GetLinearVelocity() + cross(p1.GetAngularVelocity(), r1);

	//Relative Velocity
	Vector3 dv = v0 - v1;

	float relativeMovement = -dot(dv, normal);
	if (relativeMovement < -0.01f) {
		return;
	}

	//Normal impulse
	{
		float e = 0.0f;

		float normDiv = dot(normal, normal) * ( (p0.GetInverseMass() + p1.GetInverseMass())
						+ dot(normal,
							cross( (worldInvInertia0 * cross(r0, normal)).getXYZ(), r0)
							+ cross( (worldInvInertia1 * cross(r1, normal)).getXYZ(), r1) ) );

		float jn = -1*(1+e)*dot(dv, normal) / normDiv;

		p0.SetLinearVelocity(p0.GetLinearVelocity() + (normal * p0.GetInverseMass() * jn));
		p0.SetAngularVelocity(p0.GetAngularVelocity() + (worldInvInertia0 * cross(r0, normal * jn)).getXYZ() );
		
		p1.SetLinearVelocity(p1.GetLinearVelocity() + (normal * p1.GetInverseMass() * jn));
		p1.SetAngularVelocity(p1.GetAngularVelocity() + (worldInvInertia1 * cross(r1, normal * jn)).getXYZ() );
	}

	//Tangent impulse
	{
		Vector3 tangent = normalize(dv - (normal * dot(dv, normal)));
		
		float tangDiv = p0.GetInverseMass() + p1.GetInverseMass()
						+ dot(tangent , (p0.GetInvInertia() 
						* cross((r0, tangent), r0)).getXYZ()
						+ cross( (p1.GetInvInertia()*cross(r1, tangent)).getXYZ() , r1));

		float jt = -1 * dot (dv, tangent) / tangDiv;

		p0.SetLinearVelocity(p0.GetLinearVelocity() + (tangent * p0.GetInverseMass() * jt));
		p0.SetAngularVelocity(p0.GetAngularVelocity() + (worldInvInertia0 * cross(r0, tangent * jt)).getXYZ());

		p1.SetLinearVelocity(p1.GetLinearVelocity() + (tangent * p1.GetInverseMass() * jt));
		p1.SetAngularVelocity(p1.GetAngularVelocity() + (worldInvInertia1 * cross(r1, tangent * jt)).getXYZ());
	}

}