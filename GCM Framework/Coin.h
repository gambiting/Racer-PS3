/*
	a simple points pickup
*/

#pragma once

#include "Player.h"

class Coin : public GameEntity{
public:
	Coin(){ 
		value = 100; //default item value
		pickedUp = false;
	}

	int			getValue(){return value;}
	void		setValue(int val){value = val;}

	bool		getPickedUp(){ return pickedUp; }
	void		setPickedUp(bool pick){ pickedUp = pick; }

protected:

	bool pickedUp;
	int value; 
};