#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(void) {
}

PhysicsSystem::~PhysicsSystem(void) {
}

bool PhysicsSystem::TerrainCollision(const PhysicsNode &p0,  CollisionData *d) const {
	//finds the lowest point of the object being compared to the highest point on the terrain and sees if they collide

	//if(p0)
	//{
	//	std::cout << "null node" << std::endl;
	//}


	Vector3 position = p0.GetPosition();
	Vector3 velocity = p0.GetLinearVelocity();
	float radius = p0.GetRadius();
	Vector3 lowestPoint = Vector3(position.getX(), position.getY() - radius, position.getZ());
	Vector3 tempVel = velocity;
	tempVel = normalize(tempVel);
	Vector3 forwardPoint = Vector3(position.getX() + (tempVel.getX() * radius), position.getY() + (tempVel.getY() * radius), position.getZ() + (tempVel.getZ() * radius));

	float position_y = lowestPoint.getY();
	Vector3 normal = Vector3(0, 1, 0);
	Vector3 normalForward = Vector3(0, 1, 0);

	//MapChunk* chunk = n1.GetMapTarget();
	//bottom point
	float low_obj_x = lowestPoint.getX() / (float)(HEIGHTMAP_X*RAW_WIDTH) ;
	float low_obj_z = lowestPoint.getZ() / (float)(HEIGHTMAP_Z*RAW_HEIGHT);
	//forward point
	float for_obj_x = forwardPoint.getX() / (float)(HEIGHTMAP_X*RAW_WIDTH) ;
	float for_obj_z = forwardPoint.getZ() / (float)(HEIGHTMAP_Z*RAW_HEIGHT) ;
//std::cout<< "low_obj_x: " << low_obj_x << ", low_obj_z: " << low_obj_z << std::endl;

	if(low_obj_x < 0.f || low_obj_x > 1.f || low_obj_z < 0.f || low_obj_z > 1.f )
		{
			return false;

	}
	if(for_obj_x < 0.f || for_obj_x > 1.f || for_obj_z < 0.f || for_obj_z > 1.f )
		{
			return false;

	}
	float terrain_y = HeightMap::getHeightAt(low_obj_x, low_obj_z, &normal); //TODO remember the normal was requested here
	float forward_y = HeightMap::getHeightAt(for_obj_x, for_obj_z, &normalForward); //TODO add forwardNormal request here
	position_y = terrain_y;
	if(position_y > lowestPoint.getY()){
		if(forward_y > forwardPoint.getY()){
			normal = Vector3(0,1,0) + normalForward;


		}
		if(d){

			normal = normalize(normal);
			d->m_penetration = position_y - lowestPoint.getY();
			d->m_normal = normal;
			d->m_point = position - normal * radius;
		}
		return true;
	}else if (forward_y > forwardPoint.getY()){
		if(d){
			normalForward += Vector3(0,1,0);
			normalForward =normalize(normalForward);

			d->m_penetration = forward_y - lowestPoint.getY();
			d->m_normal = normalForward;
			d->m_point = lowestPoint - normalForward * radius;
		}
		return true;
	}
	return false;
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
		std::cout << "Collision!"<< std::endl;
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
		jn = jn + (penetration * .05f);

		p0.SetLinearVelocity(p0.GetLinearVelocity() + (normal * p0.GetInverseMass() * jn));
		//p0.SetAngularVelocity(p0.GetAngularVelocity() + (worldInvInertia0 * cross(r0, normal * jn)).getXYZ() );


	
		p1.SetLinearVelocity(p1.GetLinearVelocity() + (normal * p1.GetInverseMass() * jn));
		//p1.SetAngularVelocity(p1.GetAngularVelocity() + (worldInvInertia1 * cross(r1, normal * jn)).getXYZ() );
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