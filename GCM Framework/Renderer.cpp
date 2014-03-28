#include "Renderer.h"

Renderer::Renderer(void)	{

	quad = Mesh::GenerateQuad();

	halfScreenRatio = (float)((screenWidth/2)/ screenHeight);
	player1Trans= Matrix4::rotationZYX(Vector3(DegToRad(15),DegToRad(-30),DegToRad(-10)));
	player2Trans= Matrix4::rotationZYX(Vector3(DegToRad(15),DegToRad(-30),DegToRad(-10)));

	tempQuad = Mesh::GenerateQuad();
	//tempTex = GCMRenderer::LoadGTF("/sand.gtf");
	//tempQuad->SetDefaultTexture(*tempTex);


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

	playersActive = false;

	bkgd = GCMRenderer::LoadGTF("/Textures/splashbg.gtf");


	FontTex = GCMRenderer::LoadGTF("/tahoma.gtf");
	basicFont = new Font(FontTex, 16, 16);
	
	cubeMap = GCMRenderer::LoadGTF("/cubemap.gtf");

	cubeMap->cubemap	= CELL_GCM_TRUE;
	
	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	halfScreenRatio = (float)(screenWidth / 2) / (float)screenHeight;
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!


	worldObjects.clear();	//WHY GAWD, WHYYYY?
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

	for(int i = 0; i < players.size(); i++){
		players.at(i)->GetPhysicsNode().UpdatePosition(msec);
	}

	for(std::vector<PhysicsNode*>::iterator i = worldObjects.begin(); i != worldObjects.end(); ++i) {
		(*i)->UpdatePosition(msec);
	}

}

//We should also move this!!!
void Renderer::CollisionTests() {
	
	CollisionData* cData = new CollisionData();
	//players will really be the only objects that move about, so need to check if they collide with other objects
	for(int i = 0; i < players.size(); i++){
		//check against terrain
		if(physics.TerrainCollision( players.at(i)->GetPhysicsNode(), cData)){
			PhysicsNode *temp = new PhysicsNode();
			physics.AddCollisionImpulse(players.at(i)->GetPhysicsNode(), (*temp), cData->m_point, cData->m_normal, cData->m_penetration);
			delete temp;
		}
		//check against item boxes
		for(int j = 0; j < itemBoxes.size(); j++){
			if(physics.SphereSphereCollision(players.at(i)->GetPhysicsNode(),itemBoxes.at(j)->GetPhysicsNode(), cData)){	
				physics.AddCollisionImpulse(players.at(i)->GetPhysicsNode(),itemBoxes.at(j)->GetPhysicsNode(), cData->m_point, cData->m_normal, cData->m_penetration);
			}
		}
		//check against players (except THIS player)
		for(int j = 0; j < players.size(); j++){
			if(j == i) continue;
			if(physics.SphereSphereCollision(players.at(i)->GetPhysicsNode(),players.at(j)->GetPhysicsNode(), cData)){	
				physics.AddCollisionImpulse(players.at(i)->GetPhysicsNode(),players.at(j)->GetPhysicsNode(), cData->m_point, cData->m_normal, cData->m_penetration);
			}
		}
	}

	//all other objects need to check terrain
	for(int i=0;i<worldObjects.size();i++)
	{
		CollisionData* cData = new CollisionData();

		if(physics.TerrainCollision( *worldObjects.at(i), cData))
		{
			worldObjects.at(i)->SetInAir(false);

			PhysicsNode *temp = new PhysicsNode();
			physics.AddCollisionImpulse(*worldObjects.at(i), (*temp), cData->m_point, cData->m_normal, cData->m_penetration);
			delete temp;
		}
		//For the ease of time and whatnot, just check against the 2 players.
		for (int j = 0; j < players.size(); ++j) {
			if (physics.SphereSphereCollision(*worldObjects.at(i), players.at(j)->GetPhysicsNode(), cData)) {
				physics.AddCollisionImpulse(*worldObjects.at(i), players.at(j)->GetPhysicsNode(), cData->m_point, cData->m_normal, cData->m_penetration);
			}
		}

		delete cData;
	}

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
void Renderer::drawMenu()
{
	ClearBuffer();
	SetViewport();
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	setCurrentCamera(camera1);
	drawSkyBox();
	DrawScene();
	DrawText("Press SQUARE to Begin",Vector3(screenWidth/10, screenHeight/2, 0), 75.0f);
	SwapBuffers();

}
void Renderer::drawSkyBox()
{
	
	
	cellGcmSetDepthMask(CELL_GCM_FALSE);

	cellGcmSetDepthTestEnable(CELL_GCM_FALSE); //rich

	cellGcmSetCullFaceEnable(CELL_GCM_FALSE); //rich

	this->SetCurrentShader(*skyVert,*skyFrag);
	if(currentCamera) {
		viewMatrix = currentCamera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	SetTextureSampler(currentFrag->GetParameter("cubeTex"), cubeMap);

	quad->Draw(*currentVert,*currentFrag);
	projMatrix	= Matrix4::perspective(0.7853982, halfScreenRatio, 1.0f, 20000.0f);

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
	
	currentFrag->SetParameter("lightPosition1", (float*)&players.at(0)->GetPhysicsNode().GetPosition());//.getX());
	currentFrag->SetParameter("lightPosition2", (float*)&players.at(1)->GetPhysicsNode().GetPosition());//.getX());
	currentFrag->SetParameter("cameraPos", (float*)&currentCamera->GetPosition());
	currentFrag->SetParameter("lightRadius", &testRadius);
	currentFrag->SetParameter("lightColour", (float*)&testColour);

	if(root) {
		DrawNode(root);
	}
		
}

void Renderer::DrawLoading(int i)
{
	ClearBuffer();
	SetViewport();

	this->SetCurrentShader(*basicVert,*loadFrag);

	modelMatrix = Matrix4::identity();//scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
	viewMatrix=Matrix4::identity();

	projMatrix = Matrix4::orthographic(-1.0f,1.0,-1.0, 1.0f,1.0f, -1.0f);
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	SetTextureSampler(currentFrag->GetParameter("texture"),bkgd);
	
	tempQuad->Draw(*currentVert, *currentFrag);

	DrawText("LOADING...", Vector3(screenWidth/2, screenHeight/1.9, 0), 75.0f);

	std::stringstream ss;
	ss<<i<<"%";

	DrawText(ss.str(), Vector3(screenWidth/1.3, screenHeight/1.2, 0), 75.0f);
	
	
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();

}


void Renderer::SetupPlayers() {

	std::cout << "num objects when adding player "<< worldObjects.size() << std::endl;
	
	Player* bob = new Player(1);
	bob->GetPhysicsNode().SetMesh(sphereOne);
	bob->GetPhysicsNode().SetPosition(Vector3(2000, 500, 2000));
	bob->GetPhysicsNode().setCollidable(true);
	bob->GetPhysicsNode().GravityOff();
	players.push_back(bob);
	camera1->SetPhysicsNode(bob->GetPhysicsNodePtr());
	root->AddChild(bob->GetPhysicsNode());

	Player* meryll = new Player(2);
	meryll->GetPhysicsNode().SetMesh(sphereTwo);
	meryll->GetPhysicsNode().SetPosition(Vector3(2200, 500, 2000));
	meryll->GetPhysicsNode().setCollidable(true);
	meryll->GetPhysicsNode().GravityOff();
	players.push_back(meryll);
	camera2->SetPhysicsNode(meryll->GetPhysicsNodePtr());
	root->AddChild(meryll->GetPhysicsNode());

	std::cout << "Finished creating players" << std::endl;
}

//Something nice and basic to put the players back at the start.
void Renderer::ResetPlayers() {

	
	/*
	playerOne->SetPosition(Vector3(0, 1000, 0));
	playerOne->SetLinearVelocity(Vector3(0,0,0));

	playerTwo->SetPosition(Vector3(500, 1000, 0));
	playerTwo->SetLinearVelocity(Vector3(0,0,0));*/
}

void Renderer::ActivatePlayers() {
	players.at(0)->GetPhysicsNode().GravityOn();
	players.at(1)->GetPhysicsNode().GravityOn();
	playersActive = true;
}

void Renderer::AddSphere(Camera* c) {
	PhysicsNode* newSphere = new PhysicsNode(15.0f);
	newSphere->SetMesh(sphereOne);
	newSphere->SetPosition(c->GetPosition());
	newSphere->SetLinearVelocity(camera1->GetLookDirection()/2.0f);
	newSphere->SetAngularVelocity(camera1->GetLookDirection() / 1000.0f);
	root->AddChild(*newSphere);
	worldObjects.push_back(newSphere);
}

void Renderer::AddItemBox(Item* item){
	itemBoxes.push_back(item);
	item->GetPhysicsNode().SetMesh(android);
	item->GetPhysicsNode().setRadius(25.0f);
	root->AddChild(item->GetPhysicsNode());
	worldObjects.push_back(item->GetPhysicsNodePtr());
	std::cout << "Added item box. Number of children in scene root node: "<< root->getNumChildren() << std::endl;
}

void Renderer::AddItemBox(Camera* c){
	Trap* item	= new Trap();		
	//set item's position 
	item->GetPhysicsNode().SetPosition(c->GetPosition());
	//item->setItemID(ServerInterface::AddGameEntity(WEAPONS_CRATE, item->GetPhysicsNode().GetPosition()));
	itemBoxes.push_back(item);
	item->GetPhysicsNode().SetMesh(android);
	item->GetPhysicsNode().setRadius(25.0f);
	root->AddChild(item->GetPhysicsNode());
	worldObjects.push_back(item->GetPhysicsNodePtr());
	std::cout << "Added item box. Number of children in scene root node: "<< root->getNumChildren() << std::endl;
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

	modelMatrix = Matrix4::translation(Vector3(0,4.5, 0))/* Matrix4::rotationZYX(Vector3(DegToRad(25), 0, DegToRad(25)))*/ * transform;//scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));

	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(-5.0f,5.0,-10.0, 10.0f,10.0f, -10.0f);
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	if(arrow->GetDefaultTexture())
	{
		SetTextureSampler(currentFrag->GetParameter("texture"),arrow->GetDefaultTexture());
	}

	arrow->Draw(*currentVert, *currentFrag);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	modelMatrix= Matrix4::identity();
	
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

	//Android mesh
	std::cout << "Loading meshes in renderer" << std::endl;
	android = new OBJMesh(SYS_APP_HOME "/android.obj");

	//Sphere One
	std::cout << "Loading sphere ONE in renderer" << std::endl;
		sphereOne = new OBJMesh(SYS_APP_HOME "/sphere.obj");
	std::cout << "Renderer sphere ONE load success!" << std::endl;
	sphereOne->SetDefaultTexture(*GCMRenderer::LoadGTF("/rainbow.gtf"));


	//SphereTwo
	
	sphereTwo = new OBJMesh(SYS_APP_HOME "/sphere.obj");
	sphereTwo->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));
	
	std::cout << "Loading arrow in renderer" << std::endl;
	arrow = new OBJMesh(SYS_APP_HOME "/arrow.obj");

	arrow->SetDefaultTexture(*GCMRenderer::LoadGTF("/rainbow.gtf"));
	percent+=10;//80
	DrawLoading(percent);
}
void Renderer::drawWinner(int i)
{
	switch(i)
	{
	case 1:
		SetHalfViewport1();	
		setCurrentCamera(camera1);
		this->SetCurrentShader(*basicVert,*basicFrag);

		modelMatrix = Matrix4::identity();//scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix=Matrix4::identity();
		projMatrix = Matrix4::orthographic(-1.0f,1.0f,-0.5f, 0.5f,0.5f, -0.5f);

		currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
		
		SetTextureSampler(currentFrag->GetParameter("texture"), GCMRenderer::LoadGTF("/Textures/Winner.gtf"));
		

		tempQuad->Draw(*currentVert, *currentFrag);


		break;
	case 2:
		SetHalfViewport2();
		setCurrentCamera(camera2);
		this->SetCurrentShader(*basicVert,*basicFrag);

		modelMatrix = Matrix4::identity();//scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix=Matrix4::identity();
		projMatrix = Matrix4::orthographic(-1.0f,1.0f,-0.5f, 0.5f,0.5f, -0.5f);
		
		currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
		
		SetTextureSampler(currentFrag->GetParameter("texture"), GCMRenderer::LoadGTF("/Textures/Winner.gtf"));
		

		tempQuad->Draw(*currentVert, *currentFrag);
		//SwapBuffers();
		break;
	default:
		break;
	}
	SetViewport();
	
	DrawText("GAME OVER", Vector3(screenWidth/20, screenHeight/8, 0), 200.0f);
	DrawText("Press X to Quit", Vector3(screenWidth*0.2, screenHeight*0.8, 0), 75.0f);
	DrawText("Press START to Play Again", Vector3(screenWidth/50, screenHeight*0.9, 0), 75.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();

}

void Renderer::calcArrowOrientation(Vector3 objective, int playerID)
{
	/*Vector3 worldNormal = Vector3(0,1,0);
	Vector3 dist = (player - objective);
	Vector3 r = cross(worldNormal, dist);
	r = normalize(r);
	Vector3 b = cross(r, worldNormal);
	b = normalize(b);
	Vector3 u = cross(b,r);

	Matrix4 orientation = Matrix4(Vector4(r.getX(), r.getY(), r.getZ(), 0.0f),
								  Vector4(u.getX(), u.getY(), u.getZ(), 0.0f), 
								  Vector4(b.getX(), b.getY(), b.getZ(), 0.0f), 
								  Vector4(-dot(r, player), -dot(u, player), -dot(b, player), 1.0f));*/

	
	switch(playerID)
	{
	case 1:
		Vector3 player1 = camera1->GetPhysicsNode()->GetPosition();
		//Matrix4 orientation1 = Matrix4::lookAt( Point3(player1), Point3(objective), Vector3(0,1,0));
		Vector3 worldNormal = Vector3(0,1,0);
		Vector3 dist = (objective - player1);
		Vector3 b = dist;
		b = normalize(b);
		Vector3 r = cross(worldNormal, b);
		r = normalize(r);
		Vector3 u = cross(b,r);

		Matrix4 orientation1 = Matrix4(Vector4(r.getX(), u.getX(), b.getX(), 0.0f),
									  Vector4(r.getY(), u.getY(), b.getY(), 0.0f), 
									  Vector4(r.getZ(), u.getZ(), b.getZ(), 0.0f), 
									  Vector4(0,0,0,1.0f));//Vector4(-dot(r, player1), -dot(u, player1), -dot(b, player1), 1.0f));
		player1Trans = orientation1;
		break;
	case 2:
		Vector3 player2 = camera2->GetPhysicsNode()->GetPosition();
		//Matrix4 orientation2 = Matrix4::lookAt( Point3(player2), Point3(objective), Vector3(0,1,0));
		Vector3 worldNormal2 = Vector3(0,1,0);
		Vector3 dist2 = (objective - player2);
		Vector3 b2 = dist2;
		b2 = normalize(b2);
		Vector3 r2 = cross(worldNormal2, b2);
		r2 = normalize(r2);
		Vector3 u2 = cross(b2,r2);

		Matrix4 orientation2 = Matrix4(Vector4(r2.getX(), u2.getX(), b2.getX(), 0.0f),
									  Vector4(r2.getY(), u2.getY(), b2.getY(), 0.0f), 
									  Vector4(r2.getZ(), u2.getZ(), b2.getZ(), 0.0f), 
									  Vector4(0,0,0,1.0f));//Vector4(-dot(r, player1), -dot(u, player1), -dot(b, player1), 1.0f));
		player2Trans = orientation2;

		break;
	default:
		break;
	}

}