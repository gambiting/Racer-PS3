#include "GameLogic.h"

GameLogic::GameLogic(Renderer* passedRenderer){
	
	renderer = passedRenderer;

	//Lowered the player and item counts compared to PC
	//as the area will likely be smaller
	numPlayers = (sizeof(allPlayers)/sizeof(Player*));
	maxItems = 20;
	numObjectives = 5;

	//only one human player for the moment...
	aiCount = numPlayers - 1;
	
	//set the players up
	initiatePlayers();

	gameOver = false;

	//set the first objective
	currentObjective = new ObjectivePointToPoint();
	objectiveLastLocation = currentObjective->getTargetLoc();
	std::cout << "\nIT'S A RACE! Get to the start line!";
}

void GameLogic::updateWorld(float dt){

	mPhysicsAccumulator += dt;
	
	//update the physics system
	while(mPhysicsAccumulator >= PHYSICS_TIMESTEP){
		mPhysicsAccumulator -= PHYSICS_TIMESTEP;
		
		//TODO pass in item collision checks...
		//update physics & collisions
		renderer->UpdateScene(dt);
		renderer->CollisionTests();

		//remove item boxes that have been collided with
		for(unsigned int i = 0; i < powerupsToRemove.size(); i++){
			for(int j = 0; j < (sizeof(allPlayers)/sizeof(Player*)); j++){
				//loop through players, checking if this is their node and that they don't already have an item. If able, assign them the item
				if(allPlayers[j]->GetPhysicsNodePtr() == boxCollisions[i] && allPlayers[j]->getHeldItem() == NULL){
					allPlayers[j]->setHeldItem(powerupsToRemove[i]);
					powerupsToRemove[i]->setPickedUp(true);
					//TODO remove from game world
				}
			}
		}

		bool trapRemoved = false;
		//loop through trap collisions, check if players collided with anoher player's traps
		for(int i = 0; i < trapsToRemove.size(); i++){
			for(int j = 0; j < (sizeof(allPlayers)/sizeof(Player*)); j++){
				trapRemoved = false;
				if(allPlayers[j]->GetPhysicsNodePtr() == trapCollisions[i] && allPlayers[j] != trapsToRemove[i]->getOwner()){
					//player hit enemy trap! Reduce health and slow speed!
					std::cout <<"\n Trap hit!";
					allPlayers[j]->modifyHP(-34);
					//TODO slow player speed temporarily
					allPlayers[j]->setAffliction(trapsToRemove[i]->getItemEffectType());
					//TODO remove from game world

					//remove it from the vector of laid traps and delete
					for(int k= 0; k < laidTraps.size(); k++){
						if(laidTraps[k] == trapsToRemove[i]) {							
							laidTraps.erase(laidTraps.begin() + k);		
							delete trapsToRemove[i];
							trapRemoved = true;
							break;
						}
					}

					if(trapRemoved)	break;

				}
			}
		}
		powerupsToRemove.clear();
		boxCollisions.clear();
		trapsToRemove.clear();
		trapCollisions.clear();
	}// end physics while loop

	
	//Update current objective if needed!
	if(!gameOver && currentObjective->checkIfObjectiveCompleted(allPlayers, (sizeof(allPlayers)/sizeof(Player*)) ) ){

		//The objective says we're finished. Add current objective to completed vector
		objectivesCompleted.push_back(currentObjective);

		//create new objective if old one is finished and the game isn't over	
		if(objectivesCompleted.size() < (unsigned int)numObjectives){

			//select game mode randomly
			//0 = point to point race
			//1 = assassination event
			int mode = rand() %2;

			if(mode == P2P){				
				currentObjective = new ObjectivePointToPoint(allPlayers, sizeof(allPlayers)/sizeof(Player*));	
				objectiveLastLocation = currentObjective->getTargetLoc();
				std::cout << "\nIT'S A RACE! Get to the start line!";

			}else{
				currentObjective = new ObjectiveAssassination(allPlayers, sizeof(allPlayers)/sizeof(Player*));
				objectiveLastLocation = currentObjective->getTargetLoc();
				std::cout << "\nASSASSINATION!\nTarget player: " << static_cast<ObjectiveAssassination*>(currentObjective)->getTarget()->getColour();
			}
		}else{
			gameOver = true;
			std::cout << "\n Game Over!";
		}
	}// end current objective

	//point to point race exclusive logic
	if(!gameOver && currentObjective->getObjectiveType() == P2P ){
		if(!vector3Equal(objectiveLastLocation,currentObjective->getTargetLoc())){
			std::cout << "\nAll players are ready. RACE!";
			//for when P2P points change objective location
			objectiveLastLocation = currentObjective->getTargetLoc();
			std::cout << "\nNew objective location! " << "X: " << objectiveLastLocation.getX() << " Y: " << objectiveLastLocation.getY() << " Z: " << objectiveLastLocation.getZ();
		}
	}
	
	//Update AI and human positions
	while (mAIAccumulator >= AI_TIMESTEP) {
		mAIAccumulator -= AI_TIMESTEP;
		updatePlayerPositions(AI_TIMESTEP);
	}
}//end update world


void GameLogic::initiatePlayers() {

	//Randomly place players and set all input states to NULL
	for(int i = 0; i < numPlayers; ++i){

		allPlayers[i] = new Player(i+1);

		if(i+1 <= numPlayers - aiCount){
			//initialise as human (AI by default)
			allPlayers[i]->setIsHuman(true);
		}

		allPlayers[i]->setPlayerRotation((float)(rand() % 360) / 360.f);

		/*TODO NEED CHUNK/TERRAIN INFO!
		//random starting point
		Vector3 player_pos = Vector3((float)(rand() % (TERRAIN_RAW_WIDTH)) * TERRAIN_X * 0.25f,
									(float)(rand() % 50) + 200.f,
									(float)(rand() % (TERRAIN_RAW_WIDTH)) * TERRAIN_Z * 0.25f);
		
		//set player position, orientation, force, mass, type and radius
		allPlayers[i]->GetPhysicsNode().SetPosition(player_pos);
		allPlayers[i]->GetPhysicsNode().setPreviousPos(player_pos);	*/	

		//add player node to physics system
		//TODO
	}
}


void GameLogic::updatePlayerPositions(float dt) {

	for(int i = 0; i < numPlayers; i++){

		//Check if objective has been completed
		currentObjective->checkIfPlayerCompleted(allPlayers[i], allPlayers, numPlayers);

		//Update current player state
		//allPlayers[i]->addPlayerRotation( mPlayerInputs[i].horizontal * dt * .045f);		

		//TODO need input methods.
		//if the player is holding an item and presses the button, use it and delete it
		/*if(mPlayerInputs[i].misc_flags == PLAYER_KEYFLAGS_FIREPOWERUP && allPlayers[i]->getHeldItem()!= NULL ){

			if(allPlayers[i]->getHeldItem()->getItemType() == TRAPITEM){
				//trap is a special case - we need to create a new object in the game world
				Trap* trap = static_cast<Trap*>(allPlayers[i]->getHeldItem());
				trap->useItem(allPlayers[i], laidTraps);
				allPlayers[i]->getHeldItem()->setItemID(ServerInterface::AddGameEntity(TRAPBOX, allPlayers[i]->getHeldItem()->GetPhysicsNode().GetPosition()));
				dropAndDeleteItem(i, false);

			}else{
				allPlayers[i]->getHeldItem()->useItem(allPlayers[i]);
				dropAndDeleteItem(i, true);
			}


		}else if(mPlayerInputs[i].misc_flags == PLAYER_KEYFLAGS_DROPPOWERUP  && allPlayers[i]->getHeldItem()!= NULL ){
		//if the player wants to drop an item, we need to discard it properly
			dropAndDeleteItem(i, true);
		}*/

		//Compute players forward direction		
		Vector3	forward_vec = allPlayers[i]->GetPhysicsNode().GetStoredVelocity();
		allPlayers[i]->GetPhysicsNode().SetAtRest(false);
		forward_vec.setY(0.0f);
		normalize(forward_vec);
		allPlayers[i]->GetPhysicsNode().setForwardVec(forward_vec);


		//Update AI
		//-----------------------------
		//loop through all players, have each AI do its routine
		if(!allPlayers[i]->getIsHuman()){
			bool random = false;

			if(currentObjective->getObjectiveType() == ASSASSIN){
				if(static_cast<ObjectiveAssassination*>(currentObjective)->getTarget() == allPlayers[i]){
					//the AI is the target! Move erratically
					if(rand() % 30 == 0){
						//TODO
						//mPlayerInputs[i].horizontal  = (rand() % 100) / 50.f - 1.f;
					}
					random = true;
					//target moves at normal speed so that it can be caught
					//TODO
					//mPlayerInputs[i].acceleration = .95f;
				}
				else{
					//assassins move faster to make it more challenging for human players
					//TODO
					//mPlayerInputs[i].acceleration = 1.1f;	
				}

			}
			if(!random){	
				//if the AI isn't an assassination target he should move towards the objective
				//TODO
				//allPlayers[i]->rotateTowards(currentObjective->getTargetLoc(), mPlayerInputs[i]);
			}

			if((abs(allPlayers[i]->GetPhysicsNode().GetPosition().getZ() - currentObjective->getTargetLoc().getZ()) + abs(allPlayers[i]->GetPhysicsNode().GetPosition().getX() - currentObjective->getTargetLoc().getX()))
					> 800){
				//nerf AI for racing point to point
				//TODO
				//mPlayerInputs[i].acceleration = 0.9f;
			}
			else if(currentObjective->getObjectiveType() == P2P && !static_cast<ObjectivePointToPoint*>(currentObjective)->hasStarted()){
				//slow down when near start line
				//TODO
				//mPlayerInputs[i].acceleration = 0.f;
			}

			//if the player is holding an item, give them a small chance of using it
			if(allPlayers[i]->getHeldItem()!= NULL && rand() % 100 == 0){
				if(allPlayers[i]->getHeldItem()->getItemType() == TRAPITEM){
					Trap* trap = static_cast<Trap*>(allPlayers[i]->getHeldItem());
					trap->useItem(allPlayers[i], laidTraps);
					//allPlayers[i]->getHeldItem()->setItemID(ServerInterface::AddGameEntity(TRAPBOX, allPlayers[i]->getHeldItem()->GetPhysicsNode().GetPosition()));
					dropAndDeleteItem(i, false);
				}else
					allPlayers[i]->getHeldItem()->useItem(allPlayers[i]);
					dropAndDeleteItem(i, true);
			}

		}
		//------------------------------
	}
}

//have the given player drop their item and remove it from the game if required
void GameLogic::dropAndDeleteItem(int playerIndex, bool deleteItem){

	for(int j= 0; j < gamePowerUps.size(); j++){
		//remove it from the vector of items
		if(gamePowerUps[j] == allPlayers[playerIndex]->getHeldItem()) {
			gamePowerUps.erase(gamePowerUps.begin() + j);
			break;
		}
	}
	if (deleteItem)
		delete allPlayers[playerIndex]->getHeldItem();

	allPlayers[playerIndex]->setHeldItem(NULL);
}