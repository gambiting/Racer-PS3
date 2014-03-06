#include "HeightMap.h"


HeightMap::HeightMap(std::string name)
{
	std::ifstream file(name.c_str(), std::ios::binary);

	if(!file)
	{
		std::cout<<"...Bugger - No Input\n";
		return;
	}

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH-1) * (RAW_HEIGHT-1) * 6;
	

	
	unsigned char* data = new unsigned char[numVertices];
	file.read((char*) data, numVertices * sizeof(unsigned char));
	file.close();


	vertexData =  (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * numVertices);
	unsigned short* indices = (unsigned short*)GCMRenderer::localMemoryAlign(128, sizeof(unsigned short) * numIndices);
		
	for(int x = 0; x < RAW_WIDTH; x++)
	{
		for(int z = 0; z < RAW_HEIGHT; z++)
		{
			int offset = (x * RAW_WIDTH) + z;
			vertexData[offset].x = x * HEIGHTMAP_X; 
			vertexData[offset].y = data[offset] * HEIGHTMAP_Y;
			vertexData[offset].z = z * HEIGHTMAP_Z;
			vertexData[offset].u = x * HEIGHTMAP_TEX_X;
			vertexData[offset].v = z * HEIGHTMAP_TEX_Z;
			vertexData[offset].rgba = 0xff0000ff;// Vector4(data[offset] * HEIGHTMAP_Y/256, data[offset] * HEIGHTMAP_Y/256, data[offset] * HEIGHTMAP_Y/256, 1.0f);
		}
	}
	delete data;

	numIndices = 0;
	
	for( unsigned short x = 0; x < RAW_WIDTH-1; x++)
	{
		for( unsigned short z = 0; z < RAW_HEIGHT-1; z++)
		{
			unsigned short a = (unsigned short)(x * (RAW_WIDTH)) + z;
			unsigned short b = (unsigned short)((x+1) * (RAW_WIDTH)) + z;
			unsigned short c = (unsigned short)((x+1) * (RAW_WIDTH)) + (z+1);
			unsigned short d = (unsigned short)(x * (RAW_WIDTH)) + (z+1);
			/* //DEBUG
			if(a<0)
			{
				std::cout<<"Negative Index A: X = "<< x<<", Z = "<<z<<", Value = "<<a<<"\n";
			}
			if(b<0)
			{
				std::cout<<"Negative Index B: X = "<<x<<", Z = "<<z<<", Value = "<<b<<"\n";
			}
			if(c<0)
			{
				std::cout<<"Negative Index C: X = "<<x<<", Z = "<<z<<", Value = "<<c<<"\n";
			}
			if(d<0)
			{
				std::cout<<"Negative Index D: X = "<<x<<", Z = "<<z<<", Value = "<<d<<"\n";
			}*/
			
			
			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
			
			//std::cout<<numIndices<<"\n";
		}
	}

	/*//DEBUG
	std::ofstream out;
	out.open(SYS_APP_HOME "/indexes_PS3.txt");

	if(!out)
	{
		std::cout<<"...Bugger - No Output\n";
		return;
	}

	for(int i = 0; i<numIndices; i++)
	{
		out<<"Indices"<<i<<" :-  "<<indices[i]<<"\n\n";
	}
	out.close();*/


	GenerateNormals(indices);
	//GenerateTangents();
	std::cout<<"NumVert = "<<numVertices<<"\nNumInd = "<<numIndices<<"\n";
	
	cellGcmAddressToOffset(&vertexData->x, &vertexOffsets[VERTEX_POSITION]);
	cellGcmAddressToOffset(&vertexData->rgba, &vertexOffsets[VERTEX_COLOUR]);
	cellGcmAddressToOffset(&vertexData->u, &vertexOffsets[VERTEX_TEXCOORD]);
	cellGcmAddressToOffset(&vertexData->nx, &vertexOffsets[VERTEX_NORMAL]);
	cellGcmAddressToOffset(indices, &vertexOffsets[VERTEX_INDEX]);

}


