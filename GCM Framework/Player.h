/*
	Class which extends GameEntity to give player specific parameters - 
	for example if the player is an AI and the player's state
	within the world (elements such as score, current weapon and race position etc)
*/

#pragma once

#include "GameEntity.h"

//forward declarations- Objectives and Items are assigned to Players
class Objective; 
class Item;

class Player : public GameEntity{

public:
	Player(int playerID);
	
	int				getPlayerID() { return playerID; }

	//for determining if player is AI or human
	bool			getIsHuman(){ return isHuman; }
	void			setIsHuman(bool turingTest) { isHuman = turingTest; }

	//getting/modifying player's current game score
	int				getScore() { return score; }
	void			setScore(unsigned int newScore) { score = newScore; }
	void			addScore(unsigned int toAdd) { score += toAdd; }

	//getting/modifying current player hit points
	int				getHP(){ return hp; }
	void			setHP(int amount){ hp = amount; }
	void			modifyHP(int amount);
	
	//check if the player was healed recently
	bool			recentlyHealed();

	//returns a string of the player's colour
	std::string		getColour();

	
	Item*			getHeldItem(){ return heldItem; }
	void			setHeldItem(Item* it){ heldItem = it; }

	void			addObjectiveWon(Objective* obj);
	std::vector<Objective*>* getObjectivesWon(){ return &objectivesWon; } 

	//TODO 
	//void resetPlayerCollision() { this->GetPhysicsNode().SetCollidedWith(NULL); }

	//TODO
	//rotate the player towards a point (for AI)
	//void			rotateTowards(Vector3& target, Payload_ClientUpdate& commands);

	//the last item type that negatively affected the player (used for graphical effects)
	ItemType		getLastAffliction() { return lastAffliction; }
	void			setAffliction(ItemType i) { lastAffliction = i; }
	
	//rotation commands from player
	void			setPlayerRotation(float rot){ playerRotation = rot; }
	void			addPlayerRotation(float rotBy);
	float			getPlayerRotation(){ return playerRotation; }



protected:
	
	//objectives that this player has won
	std::vector<Objective*> objectivesWon;

	int playerID; //player number

	int score;

	Item* heldItem;
	ItemType lastAffliction;

	//used for combat game modes
	int hp;

	//rotation commands sent from client to be processed by physics
	float playerRotation; 

	//check against being controlled by AI or human
	bool isHuman;	

	//for checking if recently recovered HP
	time_t timeLastHealed;	
};