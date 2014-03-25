/*
	One player becomes a target that the others must 'assassinate' by colliding with. 
	The target gets more points the longer he survives, the successful assassin gets a set number
*/
#pragma once

#include <time.h>
#include "Objective.h"

class ObjectiveAssassination : public Objective {

public:
	//default constructor puts goal on origin
	ObjectiveAssassination();
	ObjectiveAssassination(Player* allPlayers[], int arrayLength);

	Player* getTarget(){ return target; }

	//overrides parent class' versions
	virtual bool checkIfPlayerCompleted(Player* player, Player* allPlayers[], int arrayLength);
	virtual bool checkIfObjectiveCompleted(Player* allPlayers[], int arrayLength);
	virtual Vector3 getTargetLoc();

private:

	//the assassination target
	Player* target;

	//has the assassination taken place?
	bool finished;

	//used to maintain interval between points being awarded
	time_t timeLastAwarded;
	time_t timeLastHit;
	time_t timeStarted;
};