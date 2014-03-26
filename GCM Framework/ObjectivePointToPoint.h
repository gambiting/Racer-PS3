/*
	The most basic objective type - requires players get to a location in the game world
	The player who gets there first wins!
*/
#pragma once

#include "Objective.h"
#include "Vector2.h"
#include <time.h>

class ObjectivePointToPoint : public Objective {

public:
	//default constructor puts goal on origin
	ObjectivePointToPoint();
	ObjectivePointToPoint(Player* allPlayers[], int arrayLength);

	Vector3				getGoalLocation(){ return goalLocation; }
	Vector3				getStartLocation(){ return startLocation; }

	//overrides parent class' versions
	virtual bool		checkIfPlayerCompleted(Player* player, Player* allPlayers[], int arrayLength);
	virtual bool		checkIfObjectiveCompleted(Player* allPlayers[], int arrayLength);
	virtual Vector3		getTargetLoc();

	bool				hasStarted() {return raceStarted; }

private:

	//start and end of the race
	Vector3 startLocation;
	Vector3 goalLocation;

	//max distance between the player and the locations for them to register as being there
	int startRadius;
	int endRadius;

	//check to see which stage of the race we're in
	bool raceStarted;

	//players that have reached the finish line
	std::vector<Player*> playersFinished;

	//time that first player arrived at objective
	time_t timeArrived;

	//time to wait for other players to complete before continuing
	int waitLengthSecs;
	bool waiting;

	bool autoFinish;

	int temptime;
};