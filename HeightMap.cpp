#include "HeightMap.h"


HeightMap::HeightMap(std::string name)
{
	std::ifstream file(name.c_str(), std::ios::binary);

	if(!file)
	{
		std::cout<<"...Bugger\n";
		return;
	}

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH-1) * (RAW_HEIGHT-1) * 6;
	
	unsigned char* data = new unsigned char[numVertices];
	file.read((char*) data, numVertices * sizeof(unsigned char));
	file.close();


	vertexData =  (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * numVertices);
	short *indices = (short*)GCMRenderer::localMemoryAlign(128, sizeof(short) * numIndices);
	
	int counter=0;

	for(int x = 0; x < RAW_WIDTH; x++)
	{
		for(int z = 0; z < RAW_HEIGHT; z++)
		{
			counter++;
			int offset = (x * RAW_WIDTH) + z;
			vertexData[offset].x = x * HEIGHTMAP_X; 
			vertexData[offset].y = data[offset] * HEIGHTMAP_Y;
			//std::cout<<"Y Value :-" << vertexData[offset].y<<"\n";
			vertexData[offset].z = z * HEIGHTMAP_Z;
			//std::cout<<" Vertex "<<counter<<" X Value :-" << vertexData[offset].x<<", Y Value :-" << vertexData[offset].y<<", Z Value :-" << vertexData[offset].z<<"\n";
			vertexData[offset].u = x * HEIGHTMAP_TEX_X;
			vertexData[offset].v = z * HEIGHTMAP_TEX_Z;
			vertexData[offset].rgba = 0xff0000ff;// Vector4(data[offset] * HEIGHTMAP_Y/256, data[offset] * HEIGHTMAP_Y/256, data[offset] * HEIGHTMAP_Y/256, 1.0f);
		}
	}
	delete data;
	numIndices = 0;
	
	for(int x = 0; x < RAW_WIDTH-1; x++)
	{
		for(int z = 0; z < RAW_HEIGHT-1; z++)
		{
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x+1) * (RAW_WIDTH)) + z;
			int c = ((x+1) * (RAW_WIDTH)) + (z+1);
			int d = (x * (RAW_WIDTH)) + (z+1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}
	//GenerateNormals();
	//GenerateTangents();
	
	cellGcmAddressToOffset(&vertexData->x, &vertexOffsets[VERTEX_POSITION]);
	cellGcmAddressToOffset(&vertexData->rgba, &vertexOffsets[VERTEX_COLOUR]);
	cellGcmAddressToOffset(&vertexData->u, &vertexOffsets[VERTEX_TEXCOORD]);
	cellGcmAddressToOffset(indices, &vertexOffsets[VERTEX_INDEX]);

}


