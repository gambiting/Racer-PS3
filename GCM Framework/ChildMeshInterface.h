#pragma once

#include "Mesh.h"
#include <vector>

class ChildMeshInterface	{
public:
	//Adds a child mesh to this mesh (only used by OBJ and MD5Mesh)
	void AddChild(Mesh*m)	{
		children.push_back(m);
	}

	virtual ~ChildMeshInterface() {
		for(unsigned int i = 0; i < children.size(); ++i) {
			delete children.at(i);
		}
	}

protected:
	//Some Meshes have children...
	std::vector<Mesh*>children;
	ChildMeshInterface(void){};
};