#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Mesh.h"

#define RAW_WIDTH	256
#define RAW_HEIGHT	256

#define HEIGHTMAP_X	16.0f
#define HEIGHTMAP_Z	16.0f
#define HEIGHTMAP_Y	2.0f//1.25f
#define HEIGHTMAP_TEX_X	1.0f/16.0f
#define HEIGHTMAP_TEX_Z	1.0f/16.0f

class HeightMap : public Mesh
{
public:
	static unsigned char *heightData;
	static std::vector<Vector3> *normals;
	HeightMap(std::string name);
	~HeightMap(void){};

	static const float getHeightAt(float X, float Y, Vector3 *normal) 
	{

		int x = X*RAW_WIDTH;
		int y = Y*RAW_HEIGHT;
		if(heightData)
		{
			
			//std::cout<< "getting the normal now!" << std::endl;
			int offset = (x * RAW_WIDTH) + y;
			//std::cout<< "size of the normal vector: " << normals->size() << std::endl;
			if(offset<0 || offset>(256*256))
			{
				std::cout<< "Wrong coorinate requested! x: " << x << ", y: " << y << ", X:" << X << ", Y:" << Y<< std::endl;
				return 0.0f;
			}
			*normal = (normals->at(offset));
			//std::cout<< "getting the height data now!" << std::endl;
			return heightData[offset]*HEIGHTMAP_Y;
		}

		return 0.0f;
	}
};


