
#pragma once
#include <sysutil/sysutil_sysparam.h>	
#include <cell/gcm.h>

/*
MeshBuffer by another name. Used by the Mesh and
Shader classes to determine which vertex attribute
is which
*/
enum VertexAttributes {
	VERTEX_POSITION = 0,
	VERTEX_COLOUR	= 1,
	VERTEX_TEXCOORD = 2,
	VERTEX_INDEX	= 3,
	VERTEX_NORMAL	= 4,
	VERTEX_TANGENT	= 5,
	VERTEX_SIZE		= 6
};

#include "Shader.h"
//#include "Vector3.h"

class VertexShader;		//Compile our shader classes first, please
class FragmentShader;


//x, y, z - Position
//u, v - Tex Co-ord
//nx, ny, nz - Normal
struct	Vertex{
	float x, y, z, u, v, nx, ny, nz;
	uint32_t rgba; 
};

class Mesh	{
public:
	Mesh(void);
	virtual ~Mesh(void);

	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();

	virtual void	Draw(VertexShader &vertex, FragmentShader &fragment);

	void					SetDefaultTexture(CellGcmTexture &def)  {defaultTexture = &def;}
	const CellGcmTexture*	GetDefaultTexture()						{return defaultTexture;}

protected:
	unsigned int type;				//Primitive type (triangles, points, etc)
	unsigned int numVertices;		//Number of vertices for this mesh
	unsigned int numIndices;		//Number of indices for this mesh. 

	CellGcmTexture* defaultTexture;	//Default texture for this mesh, if any

	Vertex*		 vertexData;	//Pointer to the vertex data, in graphics memory!
	unsigned int vertexOffsets[VERTEX_SIZE]; //offsets into vertexData for each attribute


	//Bellow contains my hatred
	std::vector<Vector3> *GenerateNormals(unsigned short* indices);
	//Vector3 NormaliseVec3(Vector3& inp);
	//Vector3 CrossVec3(Vector3 a, Vector3 b);
	Vector3 MakeTempVec3(Vertex a);
	Vector3 MakeTempNormalVec3(Vertex a);
};
