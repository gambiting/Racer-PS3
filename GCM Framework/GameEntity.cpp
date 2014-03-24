#include "GameEntity.h"

GameEntity::GameEntity(void) {
	renderNode = new SceneNode();
	physicsNode = new PhysicsNode();
}

GameEntity::GameEntity(SceneNode* s, PhysicsNode* p){
	renderNode = s;
	physicsNode = p;
}

GameEntity::~GameEntity(void) {
	delete renderNode;
	delete physicsNode;
}

void GameEntity::Update(float msec) {
}

void GameEntity::ConnectToSystems() {
}

void GameEntity::DisconnectFromSystems() {
}