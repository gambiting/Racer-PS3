#include "Renderer.h"

Renderer::Renderer(void)	{
	/*
	You're provided with a very basic vertex / fragment shader, to get you started
	with Cg, and drawing textured objects. 
	*/

	VertexShader* basicVert		= new VertexShader("/vertex.vpo");
	FragmentShader* basicFrag	= new FragmentShader("/fragment.fpo");

	//VertexShader* lightVert = new VertexShader("/TerrainVertex.cg");
	//FragmentShader* lightFrag = new FragmentShader("/TerrainFragment.cg");

	this->SetCurrentShader(*basicVert,*basicFrag);

	CellGcmTexture* g = LoadGTF("/OutputCube.gtf");
	testRadius = 25.0f;

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
	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!
}

Renderer::~Renderer(void)	{
	delete basicFont;
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

/*
Main rendering function. Note how it's essentially the same as the
ones you were writing in OpenGL! We start by clearing the buffer,
render some stuff, then swap the buffers. All that's different is
some slightly different matrix access.

*/
void Renderer::RenderScene(float msec) {

	SetViewport();
	ClearBuffer();
	this->SetCurrentShader(*currentVert,*currentFrag);

	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);

	modelMatrix = Matrix4::identity(); 

	if(camera) {
		viewMatrix = camera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);

	//currentFrag->SetParameter("lightPosition", (float*)&camera->GetPosition());//.getX());
	//currentFrag->SetParameter("cameraPos", (float*)&camera->GetPosition());
	//currentFrag->SetParameter("lightRadius", &testRadius);
	//currentFrag->SetParameter("lightColour", &float{1.0,1.0,1.0});

	if(root) {
		DrawNode(root);
	}
	//triangle->Draw(&currentVert, &currentFrag);
	DrawText("BLOODY PS3", Vector3(0, screenHeight/1.1, 0), 16.0f);
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!

	SwapBuffers();
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective)
{
	
	TextMesh* mesh = new TextMesh(text,*basicFont);

	
	if(perspective) {
		modelMatrix = Matrix4::translation(position) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);
	}
	else{	
		
		modelMatrix = Matrix4::translation(Vector3(position.getX(),screenHeight-position.getY(), position.getZ())) * Matrix4::scale(Vector3(size,size,1));
		viewMatrix=Matrix4::identity();
		projMatrix = Matrix4::orthographic(-1.0f,1.0f,(float)screenWidth, 0.0f,(float)screenHeight, 0.0f);
	}
	
	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	SetTextureSampler(currentFrag->GetParameter("texture"),mesh->GetDefaultTexture());

	mesh->Draw(*currentVert, *currentFrag);

	delete mesh; 
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
	newSphere->SetPosition(camera->GetPosition());
	root->AddChild(*newSphere);
	firedSpheres.push_back(newSphere);
}