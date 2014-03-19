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

SYS_PROCESS_PARAM(1001, 0x10000)

bool done = false;
Renderer renderer;
SceneNode *root;

Camera* camera;

void start_button()		{
	done = true;
	std::cout << "Pressed start button!" << std::endl;
}

void select_button()		{
	camera->SetPosition(Vector3(0,0,10));
	camera->SetPitch(0.0f);
	camera->SetYaw(0.0f);
}

void cross_button() {
	renderer.AddSphere();
}

void square_button() {
	renderer.ResetPlayers();
}

void triangle_button() {
	renderer.ActivatePlayers();
}

/*
Here's a quick example program. It'll load up a single SceneNode
scene, containing a textured quad, with a camera controllable by
the joypad. Pretty basic, but as with Graphics for Games, its
everything you need to get started and doing something more 
interesting with the Playstation!

*/
int main(void)	{
	std::cout << "FG-RACER!!! :- PS3 Version\n" << std::endl;
	
	//Start off by initialising the Input system
	Input::Initialise();
	//If the start button is pressed, call this function!
	Input::SetPadFunction(INPUT_START,	start_button);
	Input::SetPadFunction(INPUT_SELECT,	select_button);
	Input::SetPadFunction(INPUT_SQUARE, square_button);
	Input::SetPadFunction(INPUT_CROSS, cross_button);
	Input::SetPadFunction(INPUT_TRIANGLE, triangle_button);

	//Make a new quad mesh, and set its texture to a newcastle logo
	//Mesh* m = Mesh::GenerateQuad();
	//m->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));

	HeightMap* h = new HeightMap(SYS_APP_HOME "/terrain.raw");
	h->SetDefaultTexture(*GCMRenderer::LoadGTF("/Sand.gtf"));
	
	/*printf("Beginning OBJ Mesh Loading\n");
	Mesh* thing = new OBJMesh(SYS_APP_HOME "/sphere.obj");
	printf("OBJ Mesh Loading Complete\n");*/
	
	//Create a new scenenode
	root = new SceneNode();

	SceneNode* h_map = new SceneNode();
	h_map->SetMesh(h);
	//h_map->SetTransform(/*Matrix4::rotationX(DegToRad(-spin)) */ Matrix4::scale(Vector3(10,10,10)));

	/*SceneNode* thing_node = new SceneNode();
	thing_node->SetMesh(thing);
	thing_node->SetTransform(Matrix4::scale(Vector3(100,100,100)));*/

	root->AddChild(*h_map);
	//root->AddChild(*thing_node);

	renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer
	renderer.SetupPlayers();

	//We need a new camera!
	camera = new Camera();	
	camera->SetControllingPad(JOYPAD_A);	//Controlled via joypad A
	camera->SetPosition(Vector3(200, 500, 200)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera(camera);	//Set the current renderer camera

	Timer gameTime;

	while(!done) {
		Input::UpdateJoypad();	//Receive latest joypad input for all joypads

		//logo->SetTransform(/*Matrix4::rotationX(DegToRad(-spin)) */ Matrix4::scale(Vector3(10,10,10)));

		float msec = (float)gameTime.GetTimedMS();

		renderer.UpdateScene(msec);

		camera->Update(msec);

		root->Update(msec);	//Update our scene hierarchy. This bit is new (previously the renderer handled it)

		renderer.RenderScene(msec);	//Render the scene

		
	}
	//If we get here, joypad A has had its start button pressed

	std::cout << "Quitting..." << std::endl;

	delete h->GetDefaultTexture();
	delete h;
	delete root;
	Input::Destroy();

	return 0;
}