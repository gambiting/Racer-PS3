#include "Player.h"

Player::Player(int id){
	playerID = id;
	isHuman = false;
	score = 0;
	hp = 100;
	heldItem = NULL;

	/*from physicsNode constructor
	physicsNode->SetCollidedWith(NULL);
	physicsNode->setisStatic(false);
	physicsNode->SetAtRest(false);
	physicsNode->SetIsDead(false);
	physicsNode->SetShape(0);
	physicsNode->SetMass(10.f);
	physicsNode->setRadius(50.f);
	physicsNode->AddForce(Vector3(), Vector3());
	physicsNode->setOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), (float)(rand() % 360)));
	physicsNode->setIsPlayer(true);
	float inertia = (2 * 10.f * pow(50.f, 2)) / 5;
	physicsNode->setInertiaVals(inertia);*/

	timeLastHealed = time(0);
}

void Player::addPlayerRotation(float rotBy){
	playerRotation += rotBy;
}

void Player::addObjectiveWon(Objective* obj){
	objectivesWon.push_back(obj);
}

/*
void Player::rotateTowards(Vector3& target, Payload_ClientUpdate& commands){

	Vector3 directionToTarget = target - this->physicsNode->GetPosition();
	directionToTarget.Normalise();

	Vector3 directionVector = this->physicsNode->getForwardVec();
	directionVector.Normalise();

	float angleToTarget = atan2(directionToTarget.z, directionToTarget.x) * 180.f / PI;;
	float angleMoving = atan2(directionVector.z, directionVector.x) * 180.f / PI;;

	float nearestAngle = (angleToTarget - angleMoving);
	commands.horizontal = nearestAngle / 10.f;
	if (commands.horizontal < -1.f) {
		commands.horizontal = -1.f;
	}
	if (commands.horizontal > 1.f) {
		commands.horizontal = 1.f;
	}
}*/

std::string Player::getColour(){

	switch(playerID-1){
		case(0):
			return "Red";
		case(1):
			return "Green";
		case(2):
			return "Blue";
		case(3):
			return "Yellow";
		case(4):
			return "Purple";
		case(5):
			return "Turqouise";
		case(6):
			return "White";
		case(7):
			return "Orange";
	}

	return "Unknown";
}

//did the player recently recover HP?
bool Player::recentlyHealed(){
	double timeDiff = difftime( time(0), timeLastHealed);

	if(timeDiff <= 1)
		return true;
	else
		return false;
}

//change HP by given amount
void Player::modifyHP(int amount){ 
	hp += amount; 
	if(hp>100){
		hp = 100; 
		timeLastHealed = time(0);
	}
	else if(hp<0) hp = 0; 
}