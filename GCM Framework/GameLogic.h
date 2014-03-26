/*
	Main gameplay logic!
	Gameplay objects such as players and objectives are all created, altered and disposed of in this class
*/

#include "ObjectiveAssassination.h"
#include "ObjectivePointToPoint.h"
#include "ItemTrap.h"
#include "Player.h"

//defines for how long processes should run
#define PHYSICS_HZ	120
#define PHYSICS_TIMESTEP (1000.0f / (float)PHYSICS_HZ)
#define AI_HZ       60
#define AI_TIMESTEP      (1000.0f / (float)AI_HZ)

class GameLogic{
public:
	GameLogic();
	~GameLogic();

	void initiatePlayers();
	void updateWorld(float dt);
	void updatePlayerPositions(float dt);

private:
	
	//keep track of time spent on physics & AI calcs
	float mPhysicsAccumulator;
	float mAIAccumulator;
	bool gameOver;

	//number of players to create, including AI
	int numPlayers;

	/* ~~~~~~~~~~~~~~~~GAME OBJECTIVES */
	//number of objectives that the server will generate over the course of the game 
	int numObjectives;
	//objectives already completed
	std::vector<Objective*> objectivesCompleted;
	//the objective the players are currently attempting to complete
	Objective* currentObjective;
	//the last objective's location
	Vector3 objectiveLastLocation;
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

	
	/* ~~~~~~~~~~~~~~~~~~GAME POWERUPS */
	//max number of items in the world as boxes
	int maxItems;
	//a container of all powerups in play
	std::vector<Item*> gamePowerUps;
	//all traps that have been laid in the game world
	std::vector<Trap*> laidTraps;
	//powerups that need removing (have been used or dropped)
	std::vector<Item*> powerupsToRemove;
	//traps that have been hit
	std::vector<Trap*> trapsToRemove;
	//container for player physics nodes who have hit boxes or traps
	std::vector<PhysicsNode*> boxCollisions;
	std::vector<PhysicsNode*> trapCollisions;
	//method to remove an item from all structs and delete it
	void dropAndDeleteItem(int playerIndex, bool deleteItem);
	//laid traps have been kept in separate vectors as they need different logic when removed!
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


	/* ~~~~~~~~~~~~~~~~~~~~~~~~PLAYERS */
	//array to hold all 4 players (including AIs)
	//player objects also hold physics entities
	Player* allPlayers[4];
	/* number of players that should be an AI 
	  (on PC, means that player count = 8 - aiCount) */
	int aiCount;
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

};