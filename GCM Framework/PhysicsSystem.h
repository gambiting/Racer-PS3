#include <vectormath/cpp/vectormath_aos.h>
#include "PhysicsNode.h"

using namespace Vectormath::Aos;

class CollisionSphere {

};

class CollisionAABB {

};

class CollisionData {

};


class PhysicsSystem	{
public:
	friend class GameClass;

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

protected:
	PhysicsSystem(void);
	~PhysicsSystem(void);

	bool SphereSphereCollision(const CollisionSphere &s0, const CollisionSphere &s1, CollisionData *collisionData = NULL) const;
	bool AABBCollision(const CollisionAABB &cube0, const CollisionAABB &cube1) const;

	//bool SphereAABBCollision(const CollisionSphere &sphere, const CollisionAABB &cube, CollisionData *collisionData = NULL) const; //Research!!!! :-)
	//Sphere plane collisions we started in the previous module, and expand upon via the Plane class..

	bool PointInConvexPolygon(const Vector3 testPosition, Vector3 * convexShapePoints, int numPointsL) const;

	static PhysicsSystem* instance;

	std::vector<PhysicsNode*> allNodes;

};