#include "ItemTrap.h"

Trap::Trap(){
	itemType = TRAPITEM;
	power = 34;
	owner = NULL;
}

Trap::Trap(int power){
	itemType = TRAPITEM;
	this->power = power;
	owner = NULL;
}

void Trap::useItem(Player* player){
	//we need the vector of traps to add to for collisions to work, so this does nothing :(
	std::cout << "\n" << player->getColour() << " used a trap incorrectly...";
}

void Trap::useItem(Player* player, std::vector<Trap*>& traps){
	//set the item's position to the player's, add to vector of traps
	physicsNode->SetPosition(player->GetPhysicsNode().GetPosition());
	owner = player;
	traps.push_back(this);
}

ItemType Trap::getItemEffectType(){
	if(owner == NULL)
		return NO_ITEM;

	switch(owner->getPlayerID())
	{
	case 1:
		return TRAP1;
	case 2:
		return TRAP2;
	case 3:
		return TRAP3;
	case 4:
		return TRAP4;
	case 5:
		return TRAP5;
	case 6:
		return TRAP6;
	case 7:
		return TRAP7;
	case 8:
		return TRAP8;
	default:
		return NO_ITEM;
	}
}