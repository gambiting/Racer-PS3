#include "Renderer.h"

Renderer::Renderer(void)	{
	/*
	You're provided with a very basic vertex / fragment shader, to get you started
	with Cg, and drawing textured objects. 
	*/

	quad = Mesh::GenerateQuad();
	testColour = Vector4(1.0,1.0,1.0,1.0);

	skyVert			= new VertexShader("/Shaders/skyBoxVert.vpo");
	skyFrag			= new FragmentShader("/Shaders/skyBoxFrag.fpo");

	lightVert		= new VertexShader("/Shaders/TerrainVert.vpo");
	lightFrag		= new FragmentShader("/Shaders/TerrainFrag.fpo");

	basicVert		= new VertexShader("/Shaders/vertex.vpo");
	basicFrag		= new FragmentShader("/Shaders/fragment.fpo");

	this->SetCurrentShader(*lightVert,*lightFrag);

	//DrawLoading();

	CellGcmTexture* g = LoadGTF("/OutputCube.gtf");
	testRadius = 1000.0f;

	//Sphere One
	std::cout << "Loading sphere ONE in renderer" << std::endl;
		sphereOne = new OBJMesh(SYS_APP_HOME "/sphere.obj");
	std::cout << "Renderer sphere ONE load success!" << std::endl;
	sphereOne->SetDefaultTexture(*GCMRenderer::LoadGTF("/Textures/checkerboard.gtf"));

	//SphereTwo
	std::cout << "Loading sphere TWO in renderer" << std::endl;
		sphereTwo = new OBJMesh(SYS_APP_HOME "/sphere.obj");
	std::cout << "Renderer sphere TWO load success!" << std::endl;
	sphereTwo->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));

	FontTex = GCMRenderer::LoadGTF("/tahoma.gtf");
	basicFont = new Font(FontTex, 16, 16);
	
	cubeMap = GCMRenderer::LoadGTF("/cubemap.gtf");
	quad->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));
	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!

}

Renderer::~Renderer(void)	{
	delete basicFont;
	
	delete tempQuad;
	delete tempTex;
}

/*
Physics stuff, positions and whatnot, happen here. 
If possible it will get moved out of the renderer at some point.
*/
void Renderer::UpdateScene(float msec) {
	playerOne->UpdatePosition(msec);
	playerTwo->UpdatePosition(msec);

	for(std::vector<PhysicsNode*>::iterator i = worldObjects.begin(); i != worldObjects.end(); ++i) {
		(*i)->UpdatePosition(msec);
	}

}

//We should also move this!!!
void Renderer::CollisionTests() {

	/*players will be the only objects that move, so just need to check if they collide with other objects
	for(int i = 0; i < players.size(); i++){
		//check against all world objects
		for(int j = 0; j < worldObjects.size(); j++{
			if(physics.SphereSphereCollision(players[i]->GetPhysicsNode(), worldObjects[j]){
				CollisionData cData;
				physics.AddCollisionImpulse(
			}
		}
		//check against item boxes
		//check against players (except THIS player)

	}*/

	for(int i=0;i<worldObjects.size();i++)
	{
		CollisionData* cData = new CollisionData();
		//std::cout << "worldObjects.size = " << worldObjects.size() << std::endl;
		if(physics.TerrainCollision( *worldObjects.at(i), cData))
		{	
			PhysicsNode *temp = new PhysicsNode();
			worldObjects.at(i)->SetInAir(false);
			physics.AddCollisionImpulse(*worldObjects.at(i), (*temp), cData->m_point, cData->m_normal, cData->m_penetration);
			delete temp;
		}
	}

	//To start just check spheres against the two players, it's all we need to test sphere/sphere collision anyway.
	/*for(std::vector<PhysicsNode*>::iterator i = worldObjects.begin(); i != worldObjects.end(); ++i) {
		CollisionData* cData = new CollisionData();
		if (physics.SphereSphereCollision( *(*i), *playerOne, cData) ) {
			//playerOne->GravityOn();
			std::cout << "cdata mpoint: " << 
				cData->m_penetration << std::endl;
			physics.AddCollisionImpulse(*(*i), *playerOne, cData->m_point, cData->m_normal, cData->m_penetration);
		}

		std::cout << "X position of the node I am about to check: " << (*i)->GetPosition().getX() << std::endl;
		if(physics.TerrainCollision( *(*i), cData))
		{
			std::cout << "collided with terrain" << std::endl;
			PhysicsNode *temp = new PhysicsNode();
			physics.AddCollisionImpulse(*(*i), (*temp), cData->m_point, cData->m_normal, cData->m_penetration);
			delete temp;
		}

		

		if (physics.SphereSphereCollision( *(*i), *playerTwo, cData) ) {
			
			//playerTwo->GravityOn();
			//physics.AddCollisionImpulse(*(*i), *playerTwo, cData->m_point, cData->m_normal, cData->m_penetration);
		}
	}*/

}

/*
Main rendering function. Note how it's essentially the same as the
ones you were writing in OpenGL! We start by clearing the buffer,
render some stuff, then swap the buffers. All that's different is
some slightly different matrix access.

*/

void Renderer::RenderScene(float msec) {

	ClearBuffer();

	SetViewport();
	//SetHalfViewport1();
	float fps = floor(1000.0f/msec);
	std::stringstream ss (std::stringstream::in | std::stringstream::out);
	ss << fps << " fps";
	std::string fpsText = ss.str();
	
	setCurrentCamera(camera1);
	drawSkyBox();
	DrawScene();
	DrawText("Player 1", Vector3(0, screenHeight/1.1, 0), 26.0f);
	DrawText(fpsText, Vector3(0, screenHeight/1.1 + 50, 0), 26.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	/*SetHalfViewport2();
	
	setCurrentCamera(camera2);
	drawSkyBox();
	DrawScene();
	DrawText("Player 2", Vector3(0, screenHeight/1.1, 0), 26.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	if(root) {
		DrawNode(root);
	}*/
	
	SwapBuffers();

	
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective)
{
	this->SetCurrentShader(*basicVert,*basicFrag);

	TextMesh* mesh = new TextMesh(text,*basicFont);

	
	if(perspective) {
		modelMatrix = Matrix4::translation(position) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix = currentCamera->BuildViewMatrix();
		projMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	}
	else{	
		
		modelMatrix = Matrix4::translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix=Matrix4::identity();
		projMatrix = Matrix4::orthographic(0.0f,(float)screenWidth,(float)screenHeight, 0.0f,1.0f, -1.0f);
	}
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	if(mesh->GetDefaultTexture())
	{
		SetTextureSampler(currentFrag->GetParameter("texture"),mesh->GetDefaultTexture());
	}

	mesh->Draw(*currentVert, *currentFrag);

	delete mesh; 
	this->SetCurrentShader(*lightVert,*lightFrag);
}
void Renderer::drawSkyBox()
{
	
	
	cellGcmSetDepthMask(CELL_GCM_FALSE);

	this->SetCurrentShader(*skyVert,*skyFrag);
	if(currentCamera) {
		viewMatrix = currentCamera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	SetTextureSampler(currentFrag->GetParameter("cubeTex"), cubeMap);

	quad->Draw(*currentVert,*currentFrag);

	this->SetCurrentShader(*lightVert,*lightFrag);

	cellGcmSetDepthMask(CELL_GCM_TRUE);
	
}

void Renderer::DrawScene()
{
	
	this->SetCurrentShader(*currentVert,*currentFrag);
	
	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);
	
	

	modelMatrix = Matrix4::identity(); 
	
	

	if(currentCamera) {
		viewMatrix = currentCamera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	currentFrag->SetParameter("lightPosition1", (float*)&camera1->GetPosition());//.getX());
	currentFrag->SetParameter("lightPosition2", (float*)&camera2->GetPosition());//.getX());
	currentFrag->SetParameter("cameraPos", (float*)&currentCamera->GetPosition());
	currentFrag->SetParameter("lightRadius", &testRadius);
	currentFrag->SetParameter("lightColour", (float*)&testColour);

	if(root) {
		DrawNode(root);
	}
	//DrawText("BLOODY PS3", Vector3(0, screenHeight/1.1, 0), 16.0f);
	//projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!

	
}

void Renderer::DrawLoading()
{
	ClearBuffer();

	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(-1.0f,1.0f,(float)screenWidth, 0.0f,(float)screenHeight, 0.0f);
	
	modelMatrix = Matrix4::identity();

	tempQuad = Mesh::GenerateQuad();
	tempTex = GCMRenderer::LoadGTF("/loading512.gtf");
	tempQuad->SetDefaultTexture(*tempTex);

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	tempQuad->Draw(*currentVert, *currentFrag);

	SwapBuffers();


	projMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
}


void Renderer::SetupPlayers() {

	playerOne = new PhysicsNode(25.0f);
	playerOne->GravityOff(); //Turn gravity OFF
	playerOne->SetMesh(sphereOne);
	playerOne->SetPosition(Vector3(0, 500, 0));
	root->AddChild(*playerOne);

	playerTwo = new PhysicsNode(25.0f);
	playerTwo->GravityOff(); //Turn gravity OFF
	playerTwo->SetMesh(sphereTwo);
	playerTwo->SetPosition(Vector3(500, 500, 0));
	root->AddChild(*playerTwo);
}

//Something nice and basic to put the players back at the start.
void Renderer::ResetPlayers() {
	playerOne->GravityOff();
	playerOne->SetPosition(Vector3(0, 1000, 0));
	playerOne->SetLinearVelocity(Vector3(0,0,0));
	
	playerTwo->GravityOff();
	playerTwo->SetPosition(Vector3(500, 1000, 0));
	playerTwo->SetLinearVelocity(Vector3(0,0,0));
}

void Renderer::ActivatePlayers() {
	playerOne->GravityOn();
	playerTwo->GravityOn();
}

void Renderer::AddSphere() {
	PhysicsNode* newSphere = new PhysicsNode(25.0f);
	newSphere->SetMesh(sphereOne);
	newSphere->SetPosition(camera1->GetPosition());
	
	newSphere->SetLinearVelocity(camera1->GetLookDirection()/10.0f);
	
	root->AddChild(*newSphere);
	worldObjects.push_back(newSphere);
}

void Renderer::AddItemBox(Item* item){
	itemBoxes.push_back(item);
	item->GetPhysicsNode().SetMesh(sphereOne);
	item->GetPhysicsNode().SetPosition(Vector3(1000, 1000, 0));
	item->GetPhysicsNode().GravityOff();
	item->GetPhysicsNode().setRadius(25.f);
	root->AddChild(item->GetPhysicsNode());
}

void Renderer::RemoveItemBox(Item* item){
	//search through itembox vector for match, remove it if found
	//This does NOT delete the item object, only stop it from rendering and being collidable
	for(int j= 0; j < itemBoxes.size(); j++){
		if(itemBoxes[j] == item) {
			itemBoxes.erase(itemBoxes.begin() + j);
			std::cout << "Item box removed from world" << std::endl;
			return;
		}
	}
	std::cout << "Failed to remove item. Awkward..."<< std::endl;
}