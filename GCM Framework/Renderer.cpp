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

	loadFrag		= new FragmentShader("/Shaders/fragmentLoad.fpo");
	

	this->SetCurrentShader(*lightVert,*lightFrag);

	testRadius = 1000.0f;

	

	FontTex = GCMRenderer::LoadGTF("/tahoma.gtf");
	basicFont = new Font(FontTex, 16, 16);
	
	cubeMap = GCMRenderer::LoadGTF("/cubemap.gtf");
	//quad->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));
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

	for(std::vector<PhysicsNode*>::iterator i = firedSpheres.begin(); i != firedSpheres.end(); ++i) {
		(*i)->UpdatePosition(msec);
	}

}

//We should also move this!!!
void Renderer::CollisionTests() {

	//To start just check spheres against the two players, it's all we need to test sphere/sphere collision anyway.
	for(std::vector<PhysicsNode*>::iterator i = firedSpheres.begin(); i != firedSpheres.end(); ++i) {
		if (physics.SphereSphereCollision( *(*i), *playerOne) ) {
			CollisionData cData;
			std::cout << "Sphere hitting player ONE" << std::endl;
			physics.AddCollisionImpulse(*(*i), *playerOne, cData.m_point, cData.m_normal, cData.m_penetration);
		}
		if (physics.SphereSphereCollision( *(*i), *playerTwo) ) {
			CollisionData cData;
			std::cout << "Sphere hitting player TWO" << std::endl;
			physics.AddCollisionImpulse(*(*i), *playerTwo, cData.m_point, cData.m_normal, cData.m_penetration);
		}
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


	SetHalfViewport1();
	
	setCurrentCamera(camera1);
	drawSkyBox();
	DrawScene();
	DrawText("Player 1", Vector3(0, screenHeight/1.1, 0), 26.0f);
	RenderArrow(Matrix4::identity());
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SetHalfViewport2();
	
	setCurrentCamera(camera2);
	drawSkyBox();
	DrawScene();
	DrawText("Player 2", Vector3(0, screenHeight/1.1, 0), 26.0f);
	RenderArrow(Matrix4::identity());
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
	/*if(currentCamera) {
		viewMatrix = currentCamera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}*/
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

void Renderer::DrawLoading()
{
	/*printf("LOADING\n");
	ClearBuffer();
	SetViewport();
	this->SetCurrentShader(*basicVert,*loadFrag);

	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(-1.0f,1.0,-1.0, 1.0f,1.0f, -1.0f);
	
	modelMatrix = Matrix4::identity();

	tempQuad = Mesh::GenerateQuad();
	tempTex = GCMRenderer::LoadGTF("/FT_Logo2.gtf");
	tempQuad->SetDefaultTexture(*tempTex);

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	if(tempQuad->GetDefaultTexture())
	{
		printf("TEXTURE FOUND\n");
		SetTextureSampler(currentFrag->GetParameter("texture"), tempQuad->GetDefaultTexture());
	}
	tempQuad->Draw(*currentVert, *currentFrag);

	SwapBuffers();

	
	projMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	delete tempQuad;
	delete tempTex;*/
	ClearBuffer();
	SetViewport();
	
	DrawText("LOADING...", Vector3(screenWidth*0.1, screenHeight/1.9, 0), 160.0f);
	
	
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();

}


void Renderer::SetupPlayers() {

	playerOne = new PhysicsNode(25.0f);
	playerOne->GravityOff(); //Turn gravity OFF
	playerOne->SetMesh(sphereOne);
	playerOne->SetPosition(Vector3(0, 1500, 0));
	root->AddChild(*playerOne);

	playerTwo = new PhysicsNode(25.0f);
	playerTwo->GravityOff(); //Turn gravity OFF
	playerTwo->SetMesh(sphereTwo);
	playerTwo->SetPosition(Vector3(500, 1500, 0));
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
	
	newSphere->SetLinearVelocity(camera1->GetLookDirection());
	
	root->AddChild(*newSphere);
	firedSpheres.push_back(newSphere);
}

void Renderer::RenderPausedScene() {

	SetViewport();
	
	DrawText("PAUSED", Vector3(screenWidth*0.1, screenHeight/1.9, 0), 260.0f);
	DrawText("Press X to Quit", Vector3(screenWidth*0.2, screenHeight/2.4, 0), 75.0f);
	DrawText("Press O to Reset", Vector3(screenWidth*0.19, screenHeight/2.9, 0), 75.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);

	SwapBuffers();

	
}
void Renderer::RenderArrow(Matrix4 transform)
{
	this->SetCurrentShader(*basicVert,*basicFrag);

	modelMatrix = Matrix4::scale(Vector3(100,100,100))* Matrix4::translation(Vector3((float) (screenWidth/4), -50, 0));//translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
	viewMatrix=Matrix4::identity();
	projMatrix = Matrix4::orthographic(0.0f,(float)screenWidth,(float)screenHeight, 0.0f,1.0f, -1.0f);
	
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
	HeightMap* h = new HeightMap(SYS_APP_HOME "/terrain.raw");
	h->SetDefaultTexture(*GCMRenderer::LoadGTF("/Sand.gtf"));
	
	//.OBJ files are ignored by git! New OBJs will have to be shared manually between folks
	printf("Beginning OBJ Mesh Loading\n");
	Mesh* thing1 = new OBJMesh(SYS_APP_HOME "/BR_Kyogre.obj");
	thing1->SetDefaultTexture(*GCMRenderer::LoadGTF("/kyogre_0_0.gtf"));

	Mesh* thing2 = new OBJMesh(SYS_APP_HOME "/Groudon.obj");
	thing2->SetDefaultTexture(*GCMRenderer::LoadGTF("/groudon_0_0.gtf"));

	Mesh* tree = new OBJMesh(SYS_APP_HOME "/tree.obj");
	tree->SetDefaultTexture(*GCMRenderer::LoadGTF("/grass.gtf"));

	SceneNode* h_map = new SceneNode();
	h_map->SetMesh(h);
	
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
	arrow->SetDefaultTexture(*GCMRenderer::LoadGTF("/FT_Logo2.gtf"));
}