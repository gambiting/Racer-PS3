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

Camera* camera1;
Camera* camera2;

void start_button()		{
	done = true;
	std::cout << "Pressed start button!" << std::endl;
}

void select_button()		{
	camera1->SetPosition(Vector3(0,0,10));
	camera1->SetPitch(0.0f);
	camera1->SetYaw(0.0f);
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
	Input::SetPadFunction(INPUT_START,	select_button);

	//Make a new quad mesh, and set its texture to a newcastle logo
	//Mesh* m = Mesh::GenerateQuad();
	//m->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));

	HeightMap* h = new HeightMap(SYS_APP_HOME "/terrain.raw");
	h->SetDefaultTexture(*GCMRenderer::LoadGTF("/Sand.gtf"));
	
	printf("Beginning OBJ Mesh Loading\n");
	Mesh* thing1 = new OBJMesh(SYS_APP_HOME "/BR_Kyogre.obj");
	thing1->SetDefaultTexture(*GCMRenderer::LoadGTF("/kyogre_0_0.gtf"));

	Mesh* thing2 = new OBJMesh(SYS_APP_HOME "/Groudon.obj");
	thing2->SetDefaultTexture(*GCMRenderer::LoadGTF("/groudon_0_0.gtf"));

	Mesh* tree = new OBJMesh(SYS_APP_HOME "/tree.obj");
	tree->SetDefaultTexture(*GCMRenderer::LoadGTF("/grass.gtf"));

	printf("OBJ Mesh Loading Complete\n");
	//Create a new scenenode
	root = new SceneNode();

	SceneNode* h_map = new SceneNode();
	h_map->SetMesh(h);
	//h_map->SetTransform(/*Matrix4::rotationX(DegToRad(-spin)) */ Matrix4::scale(Vector3(10,10,10)));

	
	SceneNode* thing_node1 = new SceneNode();
	thing_node1->SetMesh(thing1);
	thing_node1->SetTransform(Matrix4::translation(Vector3(700,300,500)) * Matrix4::scale(Vector3(10,10,10)) * Matrix4::rotationX(DegToRad(90)));
	
	SceneNode* thing_node2 = new SceneNode();
	thing_node2->SetMesh(thing2);
	thing_node2->SetTransform(Matrix4::translation(Vector3(700,300,1500)) * Matrix4::scale(Vector3(10,10,10)) * Matrix4::rotationY(DegToRad(180)));
	
	SceneNode* tree_node1 = new SceneNode();
	tree_node1->SetMesh(tree);
	tree_node1->SetTransform(Matrix4::translation(Vector3(200,400,200)) * Matrix4::scale(Vector3(30,30,30)));

	SceneNode* tree_node2 = new SceneNode();
	tree_node2->SetMesh(tree);
	tree_node2->SetTransform(Matrix4::translation(Vector3(1000,50 ,600)) * Matrix4::scale(Vector3(30,90,30)));

	SceneNode* tree_node3 = new SceneNode();
	tree_node3->SetMesh(tree);
	tree_node3->SetTransform(Matrix4::translation(Vector3(750, 50 ,550)) * Matrix4::scale(Vector3(40,30,40)));


	root->AddChild(*h_map);
	root->AddChild(*thing_node1);
	root->AddChild(*thing_node2);
	root->AddChild(*tree_node1);
	root->AddChild(*tree_node2);
	root->AddChild(*tree_node3);


	renderer.SetRootNode(root); //Set our new SceneNode as the root for our Renderer

	//We need a new camera!
	camera1 = new Camera();	
	camera1->SetControllingPad(JOYPAD_A);	//Controlled via joypad A
	camera1->SetPosition(Vector3(700, 450, 1200)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera1(camera1);	//Set the current renderer camera

	//We need a new camera!
	camera2 = new Camera();	
	camera2->SetControllingPad(JOYPAD_B);	//Controlled via joypad A
	camera2->SetPosition(Vector3(700, 450, 1200)); //And set back slightly so we can see the node at the origin

	renderer.SetCamera2(camera2);	//Set the current renderer camera

	Timer gameTime;

	while(!done) {
		Input::UpdateJoypad();	//Receive latest joypad input for all joypads

		//logo->SetTransform(/*Matrix4::rotationX(DegToRad(-spin)) */ Matrix4::scale(Vector3(10,10,10)));

		float msec = (float)gameTime.GetTimedMS();

		camera1->Update(msec);
		camera2->Update(msec);

		root->Update(msec);	//Update our scene hierarchy. This bit is new (previously the renderer handled it)

		renderer.RenderScene();	//Render the scene

		
	}
	//If we get here, joypad A has had its start button pressed

	std::cout << "Quitting..." << std::endl;

	delete h->GetDefaultTexture();
	delete h;
	delete root;
	Input::Destroy();

	return 0;
}