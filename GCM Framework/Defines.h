#pragma once
#include <string>

//AI Default Names
//------------------------
const std::string DefaultPlayerNames[] = {"Mr Nobody*",
										  "Grahams-Ghost*",
										  "Richard-Rocket*",
										  "Callam-Cannon*",
										  "Mario*",
										  "Noby Noby Boy*",
										  "Link*",
										  "Dr Nefario*" };


//Game Entities
//------------------
// Game-wide entities such as weapon's crates and checkpoints etc anything which is world-wide and/or can be added/removed mid-game
enum GameEntityType { WEAPONS_CRATE = 0, CHECKPOINT, MAX_GAME_ENTITIES, TRAPBOX };


//Item Entities
//------------------
// Game-wide item/weapon entities  contained in weapons crates
//TRAP#X is the effect of hitting a fake item box laid down by player #X, TRAPITEM is a trap that hasn't been set
enum ItemType { NO_ITEM = 0, BOOST, HP_UP, TRAPITEM, TRAP1, TRAP2, TRAP3, TRAP4, TRAP5, TRAP6, TRAP7, TRAP8 };


//Objective types
//------------------
// Game modes
enum ObjectiveTypes { P2P = 0, ASSASSIN };