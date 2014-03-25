#include "ObjectivePointToPoint.h"

ObjectivePointToPoint::ObjectivePointToPoint(){
	goalLocation = Vector3(5000,0,5000);
	startLocation = Vector3(-5000,0,5000);
	raceStarted = false;

	startRadius = 1200;
	endRadius = 500;

	objectiveType = P2P;
	waitLengthSecs = 15;
	waiting = false;
	autoFinish = false;
}

ObjectivePointToPoint::ObjectivePointToPoint(Player* allPlayers[]){

	//create start and end point based off of average location of all players
	Vector2 sumLoc(0,0);
	int length = sizeof(allPlayers)/sizeof(Player*);

	for(int i = 0; i < length; i++){
		sumLoc.x += allPlayers[i]->GetPhysicsNode().GetPosition().getX();
		sumLoc.y += allPlayers[i]->GetPhysicsNode().GetPosition().getY();
	}

	//(+1300 so that the new starting line isn't the previous finish line)
	startLocation = Vector3(sumLoc.x/length + 500, 0, sumLoc.y/length + 500);

	Vector3 tempLoc = startLocation;

	//make sure it's a good distance away!
	//the -35000 allows it to be in either direction
	while(abs(tempLoc.getX() - startLocation.getX()) < 20000){
		tempLoc.setX(startLocation.getX() + rand() % 70000 + (-35000));
	}
	while(abs(tempLoc.getZ() - startLocation.getZ()) < 20000){
		tempLoc.setZ(startLocation.getZ() + rand() % 70000 + (-35000));
	}

	goalLocation = tempLoc;

	raceStarted = false;

	startRadius = 1200;
	endRadius = 500;

	objectiveType = P2P;
	waitLengthSecs = 15;
	waiting = false;
	autoFinish = false;
}

/*
	check to see if the player is/has been near enough to the goal to declare a winner
	this function serves two purposes - check if all players have gotten to the starting point if the
	race has not started, or if it has started see if everyone has reached the finish line
*/
bool ObjectivePointToPoint::checkIfPlayerCompleted(Player* player, Player* allPlayers[], int arrayLength){

	if(waiting){
		//a player has arrived, see if enough time has elapsed to do something
		double timeDiff = difftime( time(0), timeArrived);

		//TEMP FOR TESTING
		if(timeDiff != temptime){
			std::cout<< "\n" << 15 - timeDiff;
			temptime = timeDiff;
		}

		if(timeDiff >= waitLengthSecs){
			if(!raceStarted){
				raceStarted = true;
				playersFinished.clear();
				std::cout << "\nToo slow! Starting race!";
			}
			else{
				autoFinish = true;				
				std::cout << "\nTime up! Race finished!";				
			}
			waiting = false;
		}
	}

	//if race hasn't started we check against the starting line location
	if(!raceStarted){
		//find the difference in distance between the player and the start line
		Vector2 distance(abs(player->GetPhysicsNode().GetPosition().getX()- startLocation.getX()),  
						 abs(player->GetPhysicsNode().GetPosition().getZ()- startLocation.getZ()));		

		//check that player wasn't already set to ready
		bool readied = false;
		for(unsigned int i = 0; i < playersFinished.size(); i++){
			if(playersFinished[i] == player){
				readied = true;
				break;
			}
		}

		//player hasn't been readied, see if that distance is within the limit
		if(readied == false && distance.x + distance.y <= startRadius){						
			playersFinished.push_back(player); 

			//give a small number of points as incentive to get to the start line
			player->addScore(100);

			if(playersFinished.size() == arrayLength){
				//all players are ready, start the race!
				raceStarted = true;
				std::cout << "\nRace START!";
				waiting = false;
				playersFinished.clear();
			}else if(playersFinished.size() == 1){
				//start the clock for other players to arrive
				timeArrived = time(0);
				waiting = true;
			}

		}		

		return false;		
	}

	else{
		//Race has started. Check to see if the player has already registered as having finished
		for(unsigned int i = 0; i < playersFinished.size(); i++){
			if(playersFinished[i] == player){
				return true;
			}
		}

		//player hasn't already been registered, we need to check his location!		
		//find the difference in distance between the player and the finish
		Vector2 distance(abs(player->GetPhysicsNode().GetPosition().getX()- goalLocation.getX()),  
						 abs(player->GetPhysicsNode().GetPosition().getZ()- goalLocation.getZ()));

		//check if that is close enough to count
		if(distance.x + distance.y <= endRadius){

			if(playersFinished.size() == 0){
				//start the clock for other players to arrive
				timeArrived = time(0);
				waiting = true;

				//assign the winner, add points, add this objective to the player
				winner = player;
				player->addScore(basePoints);
				player->addObjectiveWon(this);

				std::cout << "\n" << player->getColour() << " won!";
			}
			else if(playersFinished.size() == 1){
				//came second, give half points
				player->addScore(basePoints/2);
				std::cout << "\n" << player->getColour() << " came second!";
			}
			else if(playersFinished.size() == 2){
				//came third, give quarter points
				player->addScore(basePoints/4);
				std::cout << "\n" << player->getColour() << " came third!";
			}

			//add player to the list!
			playersFinished.push_back(player);
			return true;

		}		

		//player hasn't finished
		return false;
	}
}


bool ObjectivePointToPoint::checkIfObjectiveCompleted(Player* allPlayers[], int arrayLength){

	//if the race has started and 3 players have finished, race is over.
	if(autoFinish || (raceStarted && playersFinished.size() >= 3)){		
		return true;		
	}
	return false;
}

Vector3 ObjectivePointToPoint::getTargetLoc(){
	if(raceStarted){
		return goalLocation;
	}else{
		return startLocation;
	}
}