/******************************************************************************
Class:Renderer
Implements:
Author:Rich Davison
Description: For consistency with the Graphics for Games module, we're deriving
a 'renderer' from the actual API specific parent class. This is the one you 
should be fiddling with! Add whatever you like, here.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <string>

#include "PhysicsSystem.h"
#include "GCMRenderer.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "TextMesh.h"
#include <time.h>   
#include "PhysicsNode.h"
#include "Item.h"
#include "Coin.h"
#include "Player.h"
#include "HeightMap.h"

class Renderer : public GCMRenderer	{
public:
	Renderer(void);
	~Renderer(void);

	virtual void RenderScene(float msec);
	virtual void UpdateScene(float msec);
	void DrawScene();
	void DrawLoading(int i = 0);
	void SetupPlayers(); //Only for testing of physics and whatnot.
	void ResetPlayers();
	void ActivatePlayers();
	
	void AddSphere(Camera* c);

	//add and remove item boxes from the game world
	void AddCoin(Coin* coin);
	void AddCoin(int x, int z);
	void RemoveCoin(Coin* coin);
	int getNumCoins(){ return coins.size();}

	void CollisionTests();
	void RenderPausedScene();
	void RenderArrow(Matrix4 transform);//TODO
	void SetupGeometry();
	
	Vector3 getClosestCoin(int pNum);

	bool PlayersActive() { return playersActive; }
	void drawMenu();
	void drawWinner(int i);
	void calcArrowOrientation(Vector3 objective, int playerID);
	void addPoint(int i);
	PhysicsNode* MakeTrophy();

	void setTimer() { timeStarted = time(NULL); }
	int  secsSinceStarted() { return difftime(time(NULL),timeStarted);}

protected:
	void DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective=false);
	void DrawSplitScreenText(const std::string &text, const Vector3 &position, const float size, const bool perspective=false);
	void drawScore(int i);
	
	Font* basicFont;
	CellGcmTexture* FontTex;
	CellGcmTexture* cubeMap;

	Vector3 trophyPosition;

	float testRadius;

	Vector4 testColour;

	void drawSkyBox();
	

	VertexShader* skyVert;
	FragmentShader* skyFrag;

	VertexShader* lightVert;
	FragmentShader* lightFrag;

	VertexShader* basicVert;
	FragmentShader* basicFrag;

	FragmentShader* loadFrag;
	FragmentShader* impFrag;

	Mesh* tempQuad;
	CellGcmTexture* tempTex;
	CellGcmTexture* bkgd;

	Mesh* quad;

	Mesh* arrow;
	Matrix4 player1Trans;
	Matrix4 player2Trans;

	Mesh* sphereOne;
	Mesh* sphereTwo;
	Mesh* android;
	
	Mesh* trophy;

	std::vector<PhysicsNode*> worldObjects;
	std::vector<Coin*> coins;
	std::vector<Player*> players;

	PhysicsSystem physics;

	PhysicsNode* playerOne;
	PhysicsNode* playerTwo;

	bool playersActive;
	float halfScreenRatio;

	SceneNode* AndroidRoot;

	time_t timeStarted;	

};
