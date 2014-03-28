#include <stdio.h>
#include <stdlib.h>	

#include <string>
#include <iostream>

#include <sys/process.h>

#include "Renderer.h"
#include "Input.h"
#include "timer.h"
#include "camera.h"
#include "HeightMap.h"
#include "OBJMesh.h"
#include "GameLogic.h"

enum GAME_STATE{GAME_LOADING, GAME_MENU, GAME_MAIN, GAME_PAUSED, GAME_OVER};
SYS_PROCESS_PARAM(1001, 0x10000)

bool done = false;
int state = GAME_LOADING;
int winner=0;

Renderer renderer;
SceneNode *root;

Camera* camera1;
Camera* camera2;


void start_button()		{
	
	
	switch(state)
	{
	
	case GAME_MAIN:
		state = GAME_PAUSED;
		renderer.RenderPausedScene();
		break;
	case GAME_PAUSED:
		state=GAME_MAIN;
		break;
	case GAME_MENU:
		state=GAME_MAIN;
		renderer.setTimer();
	case GAME_OVER:
		
		camera1->SetScore(0);
		camera2->SetScore(0);
		renderer.ResetPlayers();
		state=GAME_MENU;
		break;
	default: break;
	}
	std::cout << "Pressed start button!" << std::endl;
}

void select_button1()		{
	switch(state)
	{
	case GAME_MAIN:
		/*camera1->SetPosition(Vector3(0,0,10));
		camera1->SetPitch(0.0f);
		camera1->SetYaw(0.0f);
		break;*/
	default:
		break;
	}
		
		std::cout << "Pressed select button!" << std::endl;
	
}
void select_button2()		{
	switch(state)
	{
	case GAME_MAIN:
		/*camera2->SetPosition(Vector3(0,0,10));
		camera2->SetPitch(0.0f);
		camera2->SetYaw(0.0f);
		break;*/
		//camera2->AddPoint();
	default:
		break;
	}
		
		std::cout << "Pressed select button!" << std::endl;
	
}

void cross_button1() {
	switch(state)
	{
	case GAME_MAIN:

		renderer.AddCoin(100,100);
		break;
	case GAME_PAUSED:
		done=true;
		break;
	case GAME_OVER:
		done=true;
		break;
	default:
		break;
		 
		//it's an X button, not cross. Sony should know this, crazy bastards.
	}
	
	std::cout << "Pressed X button!" << std::endl;
}

void cross_button2() {
	switch(state)
	{
	case GAME_MAIN:

		renderer.AddCoin(100,100);
		break;
	case GAME_PAUSED:
		done=true;
		break;
	case GAME_OVER:
		done=true;
		break;
	default:
		break;
		 
		//it's an X button, not cross. Sony should know this, crazy bastards.
	}
	
	std::cout << "Pressed X button!" << std::endl;
}

void square_button() {

	switch(state)
	{
	case GAME_MAIN:
		renderer.ResetPlayers();
		break;
	case GAME_MENU: 
		state = GAME_MAIN;
		break;
		default: break;

		//std::cout << "Pressed square button!" << std::endl;
	}
}

void circle_button()
{
	std::cout << "Pressed circle button!" << std::endl;
	switch(state)
	{
	case GAME_MAIN:
		renderer.AddSphere(camera1);
		break;
	case GAME_PAUSED: 
		renderer.ResetPlayers();
		state=GAME_MAIN;
		break;
	default:
		break;
	}
		
}

void triangle_button() {

	//done = true;
	switch(state)
	{
	case GAME_MAIN:
		renderer.ActivatePlayers();
		break;
	case GAME_PAUSED: 
		state = GAME_OVER;
		renderer.drawWinner(1);
		break;
	default: break;
	}
	std::cout << "Pressed triangle button!" << std::endl;
};


int main(void)	{
	std::cout << "FG-RACER!!! :- PS3 Version\n" << std::endl;
	//Start off by initialising the Input system
	Input::Initialise();
	renderer.DrawLoading();
	//Button functions
	Input::SetPadFunction(INPUT_START,	start_button, JOYPAD_A);
	Input::SetPadFunction(INPUT_SELECT,	select_button1, JOYPAD_A);
	Input::SetPadFunction(INPUT_SQUARE, square_button, JOYPAD_A);
	Input::SetPadFunction(INPUT_CROSS, cross_button1, JOYPAD_A);
	Input::SetPadFunction(INPUT_TRIANGLE, triangle_button, JOYPAD_A);
	Input::SetPadFunction(INPUT_CIRCLE, circle_button, JOYPAD_A);

	Input::SetPadFunction(INPUT_START,	start_button, JOYPAD_B);
	Input::SetPadFunction(INPUT_SELECT,	select_button2, JOYPAD_B);
	Input::SetPadFunction(INPUT_SQUARE, square_button, JOYPAD_B);
	Input::SetPadFunction(INPUT_CROSS, cross_button2, JOYPAD_B);
	Input::SetPadFunction(INPUT_TRIANGLE, triangle_button, JOYPAD_B);
	Input::SetPadFunction(INPUT_CIRCLE, circle_button, JOYPAD_B);	

	printf("OBJ Mesh Loading Complete\n");

	//Create a new scenenode
	root = new SceneNode();

	renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer


	//We need a new camera!
	camera1 = new Camera();	
	camera1->SetControllingPad(JOYPAD_A);	//Controlled via joypad A
	//camera1->SetPosition(Vector3(700, 450, 1200)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera1(camera1);	//Set the current renderer camera

	//We need a new camera!
	camera2 = new Camera();	
	camera2->SetControllingPad(JOYPAD_B);	//Controlled via joypad A
	//camera2->SetPosition(Vector3(700, 450, 1200)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera2(camera2);	//Set the current renderer camera

	Timer gameTime;
	GameLogic* logic = new GameLogic(&renderer);

	while(!done) 
	{
		
		Input::UpdateJoypad();	//Receive latest joypad input for all joypads
		
		float msec = (float)gameTime.GetTimedMS();	

		switch(state)
		{
			case GAME_LOADING: 
				
					renderer.DrawLoading();
					root = new SceneNode();
					renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer
					renderer.SetupGeometry();
					renderer.SetupPlayers();
					renderer.DrawLoading(90);

					//renderer.SetCamera1(camera1);	//Set the current renderer camera
					renderer.DrawLoading(95);

					//renderer.SetCamera2(camera2);	//Set the current renderer camera
					renderer.DrawLoading(100);
					

					state=GAME_MENU;
					break;
				
			case GAME_MAIN:
					camera1->Update(msec);
					camera2->Update(msec);
					renderer.UpdateScene(msec);
					root->Update(msec);	//Update our scene hierarchy. This bit is new (previously the renderer handled it)
					logic->updateWorld(msec);// gameplay logic...
					renderer.RenderScene(msec);	//Render the scene
					
					renderer.CollisionTests();
					if (!renderer.PlayersActive()) {
						renderer.ActivatePlayers();
					}
					
					std::cout << renderer.secsSinceStarted() << std::endl;
					if(renderer.secsSinceStarted() > 60)
					{
						if(camera1->GetScore() > camera2->GetScore())
						{
							renderer.drawWinner(1);
							state=GAME_OVER;
						}
						else if(camera1->GetScore() < camera2->GetScore())
						{
							renderer.drawWinner(2);
							state=GAME_OVER;
						}
					}
			
					break;
				
			case GAME_PAUSED: break;
			case GAME_MENU:
				renderer.drawMenu();
				break;

			default: break;
		}
	}

	//If we get here, joypad A has had its start button pressed
	delete logic;
	std::cout << "Quitting..." << std::endl;

	delete root;
	Input::Destroy();

	return 0;
}