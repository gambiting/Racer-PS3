/*
	Will leave a fake item box in the world which hurts players who hit it
*/
#pragma once

#include "Item.h"

class Trap : public Item{
public:
	Trap();
	Trap(int power);

	virtual void useItem(Player* player);
	void useItem(Player* player, std::vector<Trap*>& traps);
	ItemType getItemEffectType();

	Player* getOwner(){ return owner; }
private:
	Player* owner;

};