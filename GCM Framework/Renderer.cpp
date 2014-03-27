#include "Renderer.h"

Renderer::Renderer(void)	{

	quad = Mesh::GenerateQuad();
	halfScreenRatio = (float)((screenWidth/2)/ screenHeight);
	player1Trans= Matrix4::rotationZYX(Vector3(DegToRad(15),DegToRad(-30),DegToRad(-10)));
	player2Trans= Matrix4::rotationZYX(Vector3(0.0f,DegToRad(115),DegToRad(30)));

	tempQuad = Mesh::GenerateQuad();
	tempTex = GCMRenderer::LoadGTF("/sand.gtf");
	tempQuad->SetDefaultTexture(*tempTex);

	testColour = Vector4(1.0,1.0,1.0,1.0);
	printf("SkyBox Shader\n");
	skyVert			= new VertexShader("/Shaders/skyBoxVert.vpo");
	skyFrag			= new FragmentShader("/Shaders/skyBoxFrag.fpo");
	printf("PerPixel Shader\n");
	lightVert		= new VertexShader("/Shaders/TerrainVert.vpo");
	lightFrag		= new FragmentShader("/Shaders/TerrainFrag.fpo");
	printf("Text Shader\n");
	basicVert		= new VertexShader("/Shaders/vertex.vpo");
	basicFrag		= new FragmentShader("/Shaders/fragment.fpo");
	printf("LOADFRAG SHADER\n");
	loadFrag		= new FragmentShader("/Shaders/fragmentLoad.fpo");
	

	this->SetCurrentShader(*lightVert,*lightFrag);

	testRadius = 1000.0f;

	

	FontTex = GCMRenderer::LoadGTF("/tahoma.gtf");
	basicFont = new Font(FontTex, 16, 16);
	
	cubeMap = GCMRenderer::LoadGTF("/cubemap.gtf");
	
	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	halfScreenRatio = (float)(screenWidth / 2) / (float)screenHeight;
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
		std::cout << "worldObjects.size = " << worldObjects.size() << std::endl;
		if(physics.TerrainCollision( *worldObjects.at(i), cData))
		{
			
			PhysicsNode *temp = new PhysicsNode();
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


	float fps = floor(1000.0f/msec);
	std::stringstream ss (std::stringstream::in | std::stringstream::out);
	ss << fps << " fps";
	std::string fpsText = ss.str();

	/*Render Left Viewport*/
	SetHalfViewport1();	
	setCurrentCamera(camera1);
	drawSkyBox();
	DrawScene();

	DrawSplitScreenText("Player 1", Vector3(0, screenHeight/9, 0), 26.0f);
	RenderArrow(player1Trans);
	DrawSplitScreenText(fpsText, Vector3(0, screenHeight/1.1 + 50, 0), 26.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	/*Render Right Viewport*/
	SetHalfViewport2();
	
	setCurrentCamera(camera2);
	drawSkyBox();
	DrawScene();
	DrawSplitScreenText("Player 2", Vector3(0, screenHeight/9, 0), 26.0f);

	RenderArrow(player2Trans);

	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	/*if(root) {
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
		projMatrix = Matrix4::orthographic(0.0f,(float)screenWidth,0.0f,(float)screenHeight, 1.0f, -1.0f);
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
void Renderer::DrawSplitScreenText(const std::string &text, const Vector3 &position, const float size, const bool perspective)
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
		projMatrix = Matrix4::orthographic(0.0f,(float)screenWidth/2,0.0f,(float)screenHeight, 1.0f, -1.0f);
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
	projMatrix	= Matrix4::perspective(0.7853982, halfScreenRatio, 1.0f, 20000.0f);

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
		
}

void Renderer::DrawLoading(int i)
{
	/*printf("LOADING\n");

	ClearBuffer();
	SetViewport();
	this->SetCurrentShader(*basicVert,*loadFrag);

	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(-5.0f,5.0,-5.0, 5.0f,5.0f, -5.0f);	
	modelMatrix = Matrix4::identity();

	

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	if(tempQuad->GetDefaultTexture())
	{
		printf("TEXTURE FOUND\n");
		SetTextureSampler(currentFrag->GetParameter("texture"), tempQuad->GetDefaultTexture());
	}
	tempQuad->Draw(*currentVert, *currentFrag);

	SwapBuffers();
	
	projMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);*/
	


	ClearBuffer();
	SetViewport();
	
	DrawText("LOADING...", Vector3(screenWidth*0.1, screenHeight/2.5, 0), 160.0f);

	std::stringstream ss;
	ss<<i<<"%";

	DrawText(ss.str(), Vector3(screenWidth/1.5, screenHeight/1.5, 0), 160.0f);
	
	
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();
	//printf("LOADING DRAWN\n");

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

void Renderer::AddSphere(Camera* c) {
	PhysicsNode* newSphere = new PhysicsNode(25.0f);
	newSphere->SetMesh(sphereOne);
	newSphere->SetPosition(c->GetPosition());
	
	newSphere->SetLinearVelocity(c->GetLookDirection()/10.0f);
	
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
	//firedSpheres.push_back(newSphere);
}

void Renderer::RenderPausedScene() {

	SetViewport();
	
	DrawText("PAUSED", Vector3(screenWidth*0.1, screenHeight/4, 0), 260.0f);
	DrawText("Press X to Quit", Vector3(screenWidth*0.2, screenHeight/2.2, 0), 75.0f);
	DrawText("Press O to Reset", Vector3(screenWidth*0.19, screenHeight/1.8, 0), 75.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();

	
}
void Renderer::RenderArrow(Matrix4 transform)
{
	this->SetCurrentShader(*basicVert,*loadFrag);

	modelMatrix = Matrix4::translation(Vector3(0,4.5, 0)) * transform;//scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(-5.0f,5.0,-10.0, 10.0f,10.0f, -10.0f);
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	if(arrow->GetDefaultTexture())
	{
		SetTextureSampler(currentFrag->GetParameter("texture"),arrow->GetDefaultTexture());
	}

	arrow->Draw(*currentVert, *currentFrag);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

}
void Renderer::SetupGeometry()
{
	int percent = 0;
	HeightMap* h = new HeightMap(SYS_APP_HOME "/doodle.raw");
	h->SetDefaultTexture(*GCMRenderer::LoadGTF("/Sand.gtf"));
	percent+=10;//10
	DrawLoading(percent);

	//.OBJ files are ignored by git! New OBJs will have to be shared manually between folks
	printf("Beginning OBJ Mesh Loading\n");
	Mesh* thing1 = new OBJMesh(SYS_APP_HOME "/BR_Kyogre.obj");
	thing1->SetDefaultTexture(*GCMRenderer::LoadGTF("/kyogre_0_0.gtf"));
	percent+=10;//20
	DrawLoading(percent);

	Mesh* thing2 = new OBJMesh(SYS_APP_HOME "/Groudon.obj");
	thing2->SetDefaultTexture(*GCMRenderer::LoadGTF("/groudon_0_0.gtf"));
	percent+=10;//30
	DrawLoading(percent);

	Mesh* tree = new OBJMesh(SYS_APP_HOME "/tree.obj");// this is issue takes ages to load
	tree->SetDefaultTexture(*GCMRenderer::LoadGTF("/grass.gtf"));
	percent+=10;//40
	DrawLoading(percent);

	SceneNode* h_map = new SceneNode();
	h_map->SetMesh(h);
	
	SceneNode* thing_node1 = new SceneNode();
	thing_node1->SetMesh(thing1);
	thing_node1->SetTransform(Matrix4::translation(Vector3(700,300,500)) * Matrix4::scale(Vector3(10,10,10)) * Matrix4::rotationX(DegToRad(90)));
	
	SceneNode* thing_node2 = new SceneNode();
	thing_node2->SetMesh(thing2);
	thing_node2->SetTransform(Matrix4::translation(Vector3(700,50,1500)) * Matrix4::scale(Vector3(10,10,10)) * Matrix4::rotationY(DegToRad(180)));
	
	SceneNode* tree_node1 = new SceneNode();
	tree_node1->SetMesh(tree);
	tree_node1->SetTransform(Matrix4::translation(Vector3(200,50,200)) * Matrix4::scale(Vector3(30,30,30)));

	SceneNode* tree_node2 = new SceneNode();
	tree_node2->SetMesh(tree);
	tree_node2->SetTransform(Matrix4::translation(Vector3(1000,50 ,600)) * Matrix4::scale(Vector3(30,90,30)));

	SceneNode* tree_node3 = new SceneNode();
	tree_node3->SetMesh(tree);
	tree_node3->SetTransform(Matrix4::translation(Vector3(750, 50 ,550)) * Matrix4::scale(Vector3(40,30,40)));
	percent+=10;//50
	DrawLoading(percent);

	root->AddChild(*h_map);
	root->AddChild(*thing_node1);
	root->AddChild(*thing_node2);
	root->AddChild(*tree_node1);
	root->AddChild(*tree_node2);
	root->AddChild(*tree_node3);

	percent+=10;//60
	DrawLoading(percent);

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
	
	std::cout << "Loading arrow in renderer" << std::endl;
	arrow = new OBJMesh(SYS_APP_HOME "/arrow.obj");
	arrow->SetDefaultTexture(*GCMRenderer::LoadGTF("/rainbow.gtf"));
	percent+=10;//80
	DrawLoading(percent);
}