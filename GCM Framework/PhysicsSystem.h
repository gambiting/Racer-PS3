#include <vectormath/cpp/vectormath_aos.h>
#include "PhysicsNode.h"

using namespace Vectormath::Aos;

class CollisionSphere {

};

class CollisionAABB {

};

class CollisionData {
public:
	Vector3 m_point;
	Vector3 m_normal;
	float m_penetration;
};


class PhysicsSystem	{
public:

	PhysicsSystem(void);
	~PhysicsSystem(void);

	void		Update(float msec);

	void		BroadPhaseCollisions();
	void		NarrowPhaseCollisions();

	//Statics
	static void Initialise() {
		instance = new PhysicsSystem();
	}

	static void Destroy() {
		delete instance;
	}

	static PhysicsSystem& GetPhysicsSystem() {
		return *instance;
	}

	void	AddNode(PhysicsNode* n);

	void	RemoveNode(PhysicsNode* n);
	
	bool SphereSphereCollision(const PhysicsNode &p0, const PhysicsNode &p1, CollisionData *collisionData = NULL) const;
	
	bool AABBCollision(const CollisionAABB &cube0, const CollisionAABB &cube1) const;

	void AddCollisionImpulse(PhysicsNode &p0, PhysicsNode &p1, const Vector3 &hitPoint, const Vector3 &normal, float penetration);

protected:

	//bool SphereAABBCollision(const CollisionSphere &sphere, const CollisionAABB &cube, CollisionData *collisionData = NULL) const; //Research!!!! :-)
	//Sphere plane collisions we started in the previous module, and expand upon via the Plane class..

	bool PointInConvexPolygon(const Vector3 testPosition, Vector3 * convexShapePoints, int numPointsL) const;

	static PhysicsSystem* instance;

	std::vector<PhysicsNode*> allNodes;

};