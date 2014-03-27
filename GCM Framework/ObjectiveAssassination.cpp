#include "ObjectiveAssassination.h"

ObjectiveAssassination::ObjectiveAssassination(){
	target = NULL;
	objectiveType = ASSASSIN;
	finished = false;
}

ObjectiveAssassination::ObjectiveAssassination(Player* allPlayers[], int arrayLength){
	/*
		choose a target! As a way of balancing the game, choose the player with the least amount of
		points to be the target to give them a chance to catch up
	*/

	Player* lowest = allPlayers[0];

	//don't bother if there is only one player
	if(arrayLength > 1){		
		for(int i = 1; i < arrayLength ; i++){
			if(allPlayers[i]->getScore() < lowest->getScore()){
				lowest = allPlayers[i];
			}
		}
	}
	target = lowest;
	target->setHP(100);

	//start the clock!
	timeLastAwarded = time(0);
	timeLastHit = time(0);
	timeStarted = time(0);

	finished = false;
	objectiveType = ASSASSIN;
}

/*
	check if the player has collided with the target OR
	if the player is the target, apply points if enough time has passed
*/

bool ObjectiveAssassination::checkIfPlayerCompleted(Player* player, Player* allPlayers[], int arrayLength){
	if(player == target){
		//check if 1 second has passed yet, if so apply points
		double timeDiff = difftime( time(0), timeLastAwarded);
		if(!finished && timeDiff > 0){
			target->addScore(basePoints/25);
			timeLastAwarded = time(0);
		}
	}else{
		//check if collision with target has ocurred
		double timeDiff = difftime( time(0), timeLastHit);
		if(player->GetPhysicsNode().GetCollidedWith()!= NULL &&  timeDiff > 2 
			&& player->GetPhysicsNode().GetCollidedWith() == target->GetPhysicsNodePtr()){

			//apply damage and points
			target->modifyHP(-34); //dead in 3 hits
			player->addScore(basePoints/3);
			player->resetPlayerCollision();
			player->GetPhysicsNode().SetCollidedWith(NULL);
			std::cout<<"\nHit! Target score = " << target->getScore();
			timeLastHit = time(0);

			//the player to take the target under HP is recorded as the winner
			if(target->getHP() <=0 && winner == NULL){
				winner = player; 
				return true;
			}
		}else{
			player->GetPhysicsNode().SetCollidedWith(NULL); //hit during invincibility grace period
		}

	}	
	return false;
}


bool ObjectiveAssassination::checkIfObjectiveCompleted(Player* allPlayers[], int arrayLength){
	//check if total time limit has elapsed
	double timeDiff = difftime( time(0), timeStarted);

	if(target->getHP() <= 0){
		return true;
	}else if(timeDiff > 35){
		winner = target;
		std::cout << "\nMax time elapsed! Target survived.";
		return true;
	}
	return false;	
}

Vector3 ObjectiveAssassination::getTargetLoc(){
	return target->GetPhysicsNode().GetPosition();
}