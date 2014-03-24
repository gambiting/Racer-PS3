/*
	Class which extends GameEntity to give item specific parameters - 
	for example if the player is an AI and the player's state
	within the world (elements such as score, current weapon and race position etc)
*/

#pragma once

#include "Player.h"

class Item : public GameEntity{
public:
	Item(){ 
		power = 100; //default item power
		pickedUp = false;
	}
	
	ItemType			getItemType(){ return itemType; } 

	//main item function, applied in child classes
	virtual void		useItem(Player* player) = 0;

	unsigned short		getItemID(){ return itemID; }
	void				setItemID(unsigned short id) { itemID = id; }

	bool				getPickedUp(){ return pickedUp; }
	void				setPickedUp(bool pick){ pickedUp = pick; }

protected:

	ItemType itemType;
	bool pickedUp;
	int power; //the power of the item's effect (changes depending on item type)
	unsigned short itemID;
};