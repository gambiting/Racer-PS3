/*
	Simple class with virtual funcs to describe game Objective objects. Could be expanded to have
	more gameplay mechanics (first to cause a certain amount of damage etc)
*/

#pragma once

#include "Player.h"

class Objective {
public:
	Objective() {
		/*
			parent constructor gets called first, so we can just add/subtract on this
			base point value in the child constructors
		*/
		basePoints = 1000;
		winner = NULL;
	}

	virtual ~Objective(void) {  }

	Player* getWinner(){ return winner; }
	void setWinner(Player* player ){  winner = player; }

	//check to see if the given player has completed their objective (whatever that might be) 
	virtual bool checkIfPlayerCompleted(Player* player, Player* allPlayers[], int arrayLength){return false; }

	//check if the objective as a whole has been achieved
	virtual bool checkIfObjectiveCompleted(Player* allPlayers[], int arrayLength){ return false; }

	//return the vector the player will be pointed towards
	virtual Vector3 getTargetLoc(){ return Vector3(0,0,0); }

	ObjectiveTypes getObjectiveType() { return objectiveType; }

protected: 	
	//we might want different objective types to be worth more/fewer points.
	//These point differences can be based on an inherited basePoints value.	
	int basePoints; 
	ObjectiveTypes objectiveType;

	//the winning player
	Player* winner;
};