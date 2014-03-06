#pragma once

#include <string>
#include <iostream>
#include <fstream>

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
	HeightMap(std::string name);
	~HeightMap(void){};
};

