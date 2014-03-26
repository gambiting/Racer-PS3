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
			collisionData->m_normal = normalize(collisionData->m_normal);
			collisionData->m_point = pos0 - collisionData->m_normal * (p0.GetRadius() - collisionData->m_penetration * 0.5f);

		}
		std::cout << "Colliosn!"<< std::endl;
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

	float invMass0 = p0.GetInverseMass();
	float invMass1 = p1.GetInverseMass();

	Vector3 v0 = p0.GetLinearVelocity(); //+ cross(p0.GetAngularVelocity(), r0);
	Vector3 v1 = p1.GetLinearVelocity(); //+ cross(p1.GetAngularVelocity(), r1);

	//Relative Velocity
	Vector3 dv = v0 - v1;

	float relativeMovement = -dot(dv, normal);
	if (relativeMovement < -0.01f) {
		return;
	}

	//Normal impulse
	{
		float e = 0.01f;

		Vector3 crosstemp = cross(r0, normal);
		Vector4 temp = worldInvInertia0 * Vector4(crosstemp.getX(), crosstemp.getY(), crosstemp.getZ(), 1.0);
		Vector3 crosstemp2 = cross(r1, normal);
		Vector4 temp2 = worldInvInertia1 * Vector4(crosstemp2.getX(), crosstemp2.getY(), crosstemp2.getZ(), 1.0);

		
		float normDiv = dot(normal, normal) * ((invMass0 + invMass1)
			+ dot(normal, cross(Vector3(temp.getX(), temp.getY(), temp.getZ()), r0) 
			+ cross(Vector3(temp2.getX(), temp2.getY(), temp2.getZ()), r1)) );


		float jn = -1*(1+e)*dot(dv, normal) / normDiv;
		jn = jn + (penetration * 1.5f);


		std::cout << "p0 linear velocity before: " << 
				p0.GetLinearVelocity().getX() << "," <<
				p0.GetLinearVelocity().getY() << "," <<
				p0.GetLinearVelocity().getZ() <<std::endl;

		p0.SetLinearVelocity(p0.GetLinearVelocity() + (normal * p0.GetInverseMass() * jn));
		//p0.SetAngularVelocity(p0.GetAngularVelocity() + (worldInvInertia0 * cross(r0, normal * jn)).getXYZ() );
		std::cout << "p0 linear velocity after: " << 
				p0.GetLinearVelocity().getX() << "," <<
				p0.GetLinearVelocity().getY() << "," <<
				p0.GetLinearVelocity().getZ() <<std::endl;


		std::cout << "p1 linear velocity before: " << 
				p1.GetLinearVelocity().getX() << "," <<
				p1.GetLinearVelocity().getY() << "," <<
				p1.GetLinearVelocity().getZ() <<std::endl;
		p1.SetLinearVelocity(p1.GetLinearVelocity() + (normal * p1.GetInverseMass() * jn));
		//p1.SetAngularVelocity(p1.GetAngularVelocity() + (worldInvInertia1 * cross(r1, normal * jn)).getXYZ() );
		std::cout << "p1 linear velocity after: " << 
				p1.GetLinearVelocity().getX() << "," <<
				p1.GetLinearVelocity().getY() << "," <<
				p1.GetLinearVelocity().getZ() <<std::endl;
	}

	//Tangent impulse
	/*{
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
	}*/

}