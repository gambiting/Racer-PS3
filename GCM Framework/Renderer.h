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
#include "GCMRenderer.h"
#include "Mesh.h"
#include "TextMesh.h"

class Renderer : public GCMRenderer	{
public:
	Renderer(void);
	~Renderer(void);

	virtual void RenderScene();
	void DrawScene();

protected:
	void DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective=false);
	Font* basicFont;
	CellGcmTexture* FontTex;
	CellGcmTexture* cubeMap;
	float testRadius;
	void drawSkyBox();

	VertexShader* skyVert;
	FragmentShader* skyFrag;

	VertexShader* basicVert;
	FragmentShader* basicFrag;

	Mesh* quad;

};
