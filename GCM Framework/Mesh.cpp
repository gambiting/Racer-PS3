#include "Mesh.h"

Mesh::Mesh(void)	{
	vertexData		= 0;
	type			= CELL_GCM_PRIMITIVE_TRIANGLES;	//named constant for triangles, oddly enough
	numVertices		= 0;
	defaultTexture  = NULL;

	for(int i = 0; i < VERTEX_SIZE; ++i) {
		vertexOffsets[i] = 0;
	}
}

Mesh::~Mesh(void)	{
	//we'd delete vertex data here if the renderer allowed it...
}

/*
Returns a Mesh with vertex data to draw a triangle.

Yep, you've seen this function before! Slightly different this time, 
as we're writing directly to graphics memory, and we're using interleaved
vertices, rather than VBOs.
*/
Mesh* Mesh::GenerateTriangle() {
	Mesh*m = new Mesh();
	m->numVertices = 3;	//Triangles have 3 vertices...

	m->vertexData = (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * 3); //allocate some memory

	m->vertexData[0].x		= 0.0f;			//Top of our triangle
	m->vertexData[0].y		= 1.0f; 
	m->vertexData[0].z		= 0; 
	m->vertexData[0].rgba	= 0xff0000ff;	//Red

	m->vertexData[1].x		= -1.0f;		//Bottom Left
	m->vertexData[1].y		= -1.0f; 
	m->vertexData[1].z		= 0 ;
	m->vertexData[1].rgba	= 0x00ff00ff;	//Green

	m->vertexData[2].x		= 1;			//Bottom Right
	m->vertexData[2].y		= -1.0f; 
	m->vertexData[2].z		= 0; 
	m->vertexData[2].rgba	= 0x0000ffff;	//Blue

	//Think of this as setting the VBO for the positions and colours
	//(we're actually grabbing the position in memory for the first position and 
	//colour, so GCM knows how to stride over the data)

	cellGcmAddressToOffset(&m->vertexData->x, &m->vertexOffsets[VERTEX_POSITION]);
	cellGcmAddressToOffset(&m->vertexData->rgba, &m->vertexOffsets[VERTEX_COLOUR]);

	return m;
}

/*
Returns a Mesh with vertex data to draw an indexed quad.

Yep, you've seen this function before! Slightly different this time, 
as we're writing directly to graphics memory, and we're using interleaved
vertices, rather than VBOs.

*/
Mesh* Mesh::GenerateQuad() {
	Mesh*m = new Mesh();

	m->numIndices	 = 6;
	m->numVertices   = 4;

	short *indices = (short*)GCMRenderer::localMemoryAlign(128, sizeof(short) * 6);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	m->vertexData =  (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * 4);

	float size = 1.0f;

	m->vertexData[0].x = -size;			//Top left of our quad
	m->vertexData[0].y = size; 
	m->vertexData[0].z = 0.0f; 
	m->vertexData[0].u = 0.0f;
	m->vertexData[0].v = 1.0f;

	m->vertexData[1].x = -size;		//Bottom left
	m->vertexData[1].y = -size; 
	m->vertexData[1].z = 0.0f; 
	m->vertexData[1].u = 0.0f;
	m->vertexData[1].v = 0.0f;

	m->vertexData[2].x = size;			//Bottom Right
	m->vertexData[2].y = -size; 
	m->vertexData[2].z = 0.0f; 
	m->vertexData[2].u = 1.0f;
	m->vertexData[2].v = 0.0f;

	m->vertexData[3].x = size;			//Top Right
	m->vertexData[3].y = size; 
	m->vertexData[3].z = 0.0f; 
	m->vertexData[3].u = 1.0f;
	m->vertexData[3].v = 1.0f;

	m->vertexData[0].rgba=0xffffffff;
	m->vertexData[1].rgba=0xffffffff;
	m->vertexData[2].rgba=0xffffffff;
	m->vertexData[3].rgba=0xffffffff;

	cellGcmAddressToOffset(&m->vertexData->x, &m->vertexOffsets[VERTEX_POSITION]);
	cellGcmAddressToOffset(&m->vertexData->rgba, &m->vertexOffsets[VERTEX_COLOUR]);
	cellGcmAddressToOffset(&m->vertexData->u, &m->vertexOffsets[VERTEX_TEXCOORD]);
	cellGcmAddressToOffset(indices, &m->vertexOffsets[VERTEX_INDEX]);
	//Just like in the OpenGL framework, we're treating indices like a vertex attribute...

	return m;
}

/*
Draws a mesh, via linking its vertex attributes to the vertex shader.

This function is mostly as described in the GCM tutorial, except we have
a branch statement to draw an indexed mesh. It's very similar to how to
draw an indexed mesh with OpenGL!
*/
void	Mesh::Draw(VertexShader &vertex, FragmentShader &fragment) 
{
	if(vertexOffsets[VERTEX_POSITION])
	{
		cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_POSITION),
			0, 
			sizeof(Vertex), 
			3, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VERTEX_POSITION]
		);

		if(vertexOffsets[VERTEX_COLOUR])	{
			cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_COLOUR),
				0, 
				sizeof(Vertex), 
				4, 
				CELL_GCM_VERTEX_UB, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VERTEX_COLOUR]
			);
		}
		if(vertexOffsets[VERTEX_NORMAL])	{
			cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_NORMAL),
				0, 
				sizeof(Vertex), 
				3, 
				CELL_GCM_VERTEX_F, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VERTEX_NORMAL]
			);
		}
		//else{
		//	cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_COLOUR),0,0,0,CELL_GCM_VERTEX_UB,CELL_GCM_LOCATION_LOCAL,0);
		//}

		if(vertexOffsets[VERTEX_TEXCOORD])	{
			cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_TEXCOORD),
				0, 
				sizeof(Vertex), 
				2, 
				CELL_GCM_VERTEX_F, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VERTEX_TEXCOORD]
			);
		}
		//else{
		//	cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VERTEX_TEXCOORD),0,0,0,CELL_GCM_VERTEX_F,CELL_GCM_LOCATION_LOCAL,0);
		//}

		if(vertexOffsets[VERTEX_INDEX]) 
		{	//If we have indices
			
			cellGcmSetDrawIndexArray(type, numIndices, CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16,
				CELL_GCM_LOCATION_LOCAL, vertexOffsets[VERTEX_INDEX]);
		}
		else{
			//else just draw an ordered list of vertices
			
			cellGcmSetDrawArrays(type, 0, numVertices);
		}
	}
}


void Mesh::GenerateNormals(unsigned short* indices)
{
	for(int i = 0; i<numIndices; i+=3)
	{
		vertexData[i].nx = 0;
		vertexData[i].ny = 0;
		vertexData[i].nz = 0;
	}

	
	if(vertexOffsets[VERTEX_INDEX])
	{
		for(int i = 0; i < numIndices; i+=3)
		{
			unsigned int a = indices[i];
			unsigned int b = indices[i+1];
			unsigned int c = indices[i+2];

			Vector3 normal = CrossVec3((MakeTempVec3(vertexData[b]) - MakeTempVec3(vertexData[a])), (MakeTempVec3(vertexData[c]) - MakeTempVec3(vertexData[a])));
			normal=NormaliseVec3(normal);


			vertexData[a].nx += normal.getX();
			vertexData[a].ny += normal.getY();
			vertexData[a].nz += normal.getZ();

			vertexData[b].nx += normal.getX();
			vertexData[b].ny += normal.getY();
			vertexData[b].nz += normal.getZ();

			vertexData[c].nx += normal.getX();
			vertexData[c].ny += normal.getY();
			vertexData[c].nz += normal.getZ();

		}
	}
	else
	{
		for(int i = 0; i<numVertices ; i+=3)
		{
			Vector3 a = MakeTempVec3(vertexData[i]);
			Vector3 b = MakeTempVec3(vertexData[i+1]);
			Vector3 c = MakeTempVec3(vertexData[i+2]);

			Vector3 normal = CrossVec3(b-a, c-a);
			normal=NormaliseVec3(normal);

			vertexData[i].nx = normal.getX();
			vertexData[i].ny = normal.getY();
			vertexData[i].nz = normal.getZ();

			vertexData[i+1].nx = normal.getX();
			vertexData[i+1].ny = normal.getY();
			vertexData[i+1].nz = normal.getZ();
			
			vertexData[i+2].nx = normal.getX();
			vertexData[i+2].ny = normal.getY();
			vertexData[i+2].nz = normal.getZ();
		}
	}
	cellGcmAddressToOffset(&vertexData->nx, &vertexOffsets[VERTEX_NORMAL]);
}

Vector3 Mesh::NormaliseVec3(Vector3& inp)
{
	float x = inp.getX();
	float y = inp.getY();
	float z = inp.getZ();
	float length = sqrt((x*x)+(y*y)+(z*z));

		if(length != 0.0f)	
		{
			length = 1.0f / length;
			x = x * length;
			y = y * length;
			z = z * length;

			return Vector3(x,y,z);
		}
		return Vector3(0,0,0);
}

Vector3 Mesh::CrossVec3(Vector3 a, Vector3 b)
{
	return Vector3( (a.getY() * b.getZ()) - (a.getZ() * b.getX()),
					(a.getZ() * b.getX()) - (a.getX() * b.getZ()),
					(a.getX() * b.getY()) - (a.getY() * b.getX())
					);
}

Vector3 Mesh::MakeTempVec3(Vertex a)
{
	return Vector3(a.x, a.y, a.z);
}

