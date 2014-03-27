#pragma once

#include <stdlib.h>
#include "SceneNode.h"
#include "PhysicsNode.h"

class GameEntity	{
public:

	GameEntity(void);
	GameEntity(SceneNode* s, PhysicsNode* p);
	virtual ~GameEntity(void);

	virtual void	Update(float msec);

	SceneNode&		GetRenderNode()		{ return *renderNode;}
	PhysicsNode&	GetPhysicsNode()	{ return *physicsNode;}
	PhysicsNode*	GetPhysicsNodePtr() { return this->physicsNode; }

	void			ConnectToSystems();
	void			DisconnectFromSystems();

protected:
	SceneNode*		renderNode;
	PhysicsNode*	physicsNode;
};