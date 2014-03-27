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

	/*static const float getHeightAt(float X, float Y, Vector3 *normal) 
	{

		int x = X*RAW_WIDTH;
		int y = Y*RAW_HEIGHT;
		if(heightData)
		{
			
			//std::cout<< "getting the normal now!" << std::endl;
			int offset = (x * RAW_WIDTH) + y;
			int offset2 = (y * RAW_WIDTH) + x;
			//std::cout<< "size of the normal vector: " << normals->size() << std::endl;
			if(offset<0 || offset>(256*256))
			{
				std::cout<< "Wrong coorinate requested! x: " << x << ", y: " << y << ", X:" << X << ", Y:" << Y<< std::endl;
				return 0.0f;
			}
			*normal = normalize((normals->at(offset2)));
			//std::cout<< "getting the height data now!" << std::endl;
			return heightData[offset]*HEIGHTMAP_Y;
		}

		return 0.0f;
	}*/

	static float getRawValue(int x, int y) {
		return heightData[x * RAW_WIDTH + y];
	}

	static float getHeightAt(float X, float Y, Vector3* out_normal) {
		int x = X*RAW_WIDTH;
		int y = Y*RAW_HEIGHT;
		if(heightData)
		{
			int tl_x = (int)x;
			int tl_y = (int)y;

			//A---B
			//|\  |
			//| \ |
			//|  \|
			//C---D
			unsigned char A = getRawValue(tl_x  , tl_y  );
			unsigned char B = getRawValue(tl_x+1, tl_y  );
			unsigned char C = getRawValue(tl_x  , tl_y+1);
			unsigned char D = getRawValue(tl_x+1, tl_y+1);

			//Mix horizontal
			float AB = (1.f - X) * (float)A + (X) * (float)B;
			float CD = (1.f - X) * (float)C + (X) * (float)D;

			//Mix Vertical
			float ABCD = (1.f - Y) * AB + (Y) * CD;


			//Normal calculation
			Vector3 worldScale(256.f * 16.f, 255.f * HEIGHTMAP_Y, 256.f * 16.f);
			float one_pixel = 1.f / 255.f;
			float pos_x = one_pixel * worldScale.getX();
			float pos_z = one_pixel * worldScale.getX();

			Vector3 pA = Vector3(0.f  , A * one_pixel * worldScale.getY(), 0.f);
			Vector3 pB = Vector3(pos_x, B * one_pixel * worldScale.getY(), 0.f);
			Vector3 pC = Vector3(0.f  , C * one_pixel * worldScale.getY(), pos_z);
			Vector3 pD = Vector3(pos_x, D * one_pixel * worldScale.getY(), pos_z);

			*out_normal = cross(pC - pA, pB - pA)
								+ cross(pD - pC, pB - pC);

			
			*out_normal = normalize(*out_normal);

			//Return normalised
			return ABCD * one_pixel * worldScale.getY();
		}
		return 0.f;
	}
};


