#include "OBJMesh.h"

bool	OBJMesh::LoadOBJMesh(std::string filename)	
{
	std::cout<<"OBJMesh:- Beggining File Load\n";
	std::ifstream f(filename.c_str(),std::ios::in);

	if(!f) {//Oh dear, it can't find the file :(
		std::cout<<"OBJMesh:- Bugger... \n";
		return false;
	}
	std::cout<<"OBJMesh:- File Exists\n";

	/*
	Stores the loaded in vertex attributes
	*/
	std::vector<Vector3> inputVertices;//Vec3
	std::vector<Vector3> inputTexCoords;//Vec2
	std::vector<Vector3> inputNormals;//Vec3

	/*
	SubMeshes temporarily get kept in here
	*/
	std::vector<OBJSubMesh*> inputSubMeshes;

	OBJSubMesh* currentMesh = new OBJSubMesh();
	inputSubMeshes.push_back(currentMesh);	

	while(!f.eof()) {
		std::string currentLine;
		f >> currentLine;
		float tempx, tempy, tempz;
		if(currentLine == OBJCOMMENT) {		//This line is a comment, ignore it
			continue;
		}
		else if(currentLine == OBJMESH /*|| currentLine == OBJUSEMTL*/ || currentLine == OBJOBJECT) {	//This line is a submesh!
			currentMesh = new OBJSubMesh();
			inputSubMeshes.push_back(currentMesh);
		}
		else if(currentLine == OBJVERT) {	//This line is a vertex
			
			f >> tempx;//x
			
			f >> tempy;//y
			
			f >> tempz;//z
			inputVertices.push_back(Vector3(tempx, tempy, tempz));
		}
		else if(currentLine == OBJNORM) {	//This line is a Normal!
			f >> tempx;//nx
			
			f >> tempy;//ny
			
			f >> tempz;//nz
			inputNormals.push_back(Vector3(tempx, tempy, tempz));
			
		}
		else if(currentLine == OBJTEX) {	//This line is a texture coordinate!
			f >> tempx;//u
			
			f >> tempy;//v
			inputTexCoords.push_back(Vector3(tempx, tempy, 0.0f));
		}
		else if(currentLine == OBJFACE) {	//This is an object face!
			if(!currentMesh) {
				inputSubMeshes.push_back(new OBJSubMesh());
				currentMesh = inputSubMeshes[inputSubMeshes.size()-1];
			}

			std::string			faceData;		//Keep the entire line in this!
			std::vector<int>	faceIndices;	//Keep the extracted indices in here!
		
			getline(f,faceData);		//Use a string helper function to read in the entire face line

			/*
			It's possible an OBJ might have normals defined, but not tex coords!
			Such files should then have a face which looks like this:

				f <vertex index>//<normal index>
				instead of <vertex index>/<tex coord>/<normal index>

				you can be some OBJ files will have "/ /" instead of "//" though... :(
			*/
			bool	skipTex = false;
			if(faceData.find("//") != std::string::npos) {	
				skipTex = true;
			}

			/*
			Count the number of slashes, but also convert the slashes to spaces
			so that string streaming of ints doesn't fail on the slash

				"f  0/0/0" becomes "f 0 0 0" etc
			*/
			for(size_t i = 0; i < faceData.length(); ++i) {
				if(faceData[i] == '/') {
					faceData[i] = ' ';
				}
			}

			/*
			Now string stream the indices from the string into a temporary
			vector.
			*/
			int tempIndex;
			std::stringstream	ss(faceData);
			while(ss >> tempIndex) {
				faceIndices.push_back(tempIndex);
			}

			//If the face indices vector is a multiple of 3, we're looking at triangles
			//with some combination of vertices, normals, texCoords
			if(faceIndices.size()%3 == 0) {		//This face is a triangle (probably)!
				if(faceIndices.size() == 3) {	//This face has only vertex information;
					currentMesh->vertIndices.push_back(faceIndices.at(0));
					currentMesh->vertIndices.push_back(faceIndices.at(1));
					currentMesh->vertIndices.push_back(faceIndices.at(2));
				}
				else if(faceIndices.size() == 9) {	//This face has vertex, normal and tex information!
					for(int i = 0; i < 9; i += 3) {
						currentMesh->vertIndices.push_back(faceIndices.at(i));
						currentMesh->texIndices.push_back(faceIndices.at(i+1));
						currentMesh->normIndices.push_back(faceIndices.at(i+2));
					}
				}
				else if(faceIndices.size() == 6) {	//This face has vertex, and one other index...
					for(int i = 0; i < 6; i += 2) {
						currentMesh->vertIndices.push_back(faceIndices.at(i));
						if(!skipTex) {		// a double slash means it's skipping tex info...
							currentMesh->texIndices.push_back(faceIndices.at(i+1));
						}
						else{
							currentMesh->normIndices.push_back(faceIndices.at(i+1));
						}
					}
				}
			}
			else{	
				//Uh oh! Face isn't a triangle. Have fun adding stuff to this ;)
			}
		}
		else{
			//std::cout << "OBJMesh::LoadOBJMesh Unknown file data:" << currentLine << std::endl;
		}
	}

	f.close();

	std::cout<<"OBJMesh:- Temp Data Load Successfull\n";

	//11/03/2014 - above works, future errors derrived from editing above or code bellow

	//We now have all our mesh data loaded in...Now to convert it into OpenGL vertex buffers!
	for(unsigned int i = 0; i < inputSubMeshes.size(); ++i) 
	{
		OBJSubMesh*sm = inputSubMeshes[i];

		/*
		We're going to be lazy and turn the indices into an absolute list
		of vertex attributes (it makes handling the submesh list easier)
		*/
		if(!sm->vertIndices.empty()) {
			OBJMesh*m		= new OBJMesh();
	
			m->numVertices	= sm->vertIndices.size();
			m->vertexData =  (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * m->numVertices);
			for(unsigned int j = 0; j < sm->vertIndices.size(); j++) 
			{
					m->vertexData[j].x = inputVertices[sm->vertIndices[j]-1].getX();
					m->vertexData[j].y = inputVertices[sm->vertIndices[j]-1].getY();
					m->vertexData[j].z = inputVertices[sm->vertIndices[j]-1].getZ();
			}

			if(!sm->texIndices.empty())	
			{
				
				for(unsigned int j = 0; j < sm->vertIndices.size(); j++) 
				{
					m->vertexData[j].x = inputVertices[sm->vertIndices[j]-1].getX();
					m->vertexData[j].y = inputVertices[sm->vertIndices[j]-1].getY();
					m->vertexData[j].z = inputVertices[sm->vertIndices[j]-1].getZ();

					m->vertexData[j].u = inputTexCoords[sm->texIndices[j]-1].getX();
					m->vertexData[j].v = inputTexCoords[sm->texIndices[j]-1].getY();
				}
			}
			else
			{
				for(unsigned int j = 0; j < sm->vertIndices.size(); j++) 
				{
					m->vertexData[j].x = inputVertices[sm->vertIndices[j]-1].getX();
					m->vertexData[j].y = inputVertices[sm->vertIndices[j]-1].getY();
					m->vertexData[j].z = inputVertices[sm->vertIndices[j]-1].getZ();
				}
			}

			
						if(sm->normIndices.empty()) {
							m->GenerateNormals((unsigned short*)&sm->vertIndices[0]);
							printf("OBJMesh:- Normals Generated\n");
						}
						else{
							printf("OBJMesh:- Normals Loading\n");
							for(unsigned int j = 0; j < sm->normIndices.size(); j++) 
							{
								m->vertexData[j].nx = inputNormals[sm->normIndices[j]-1].getX();
								m->vertexData[j].ny = inputNormals[sm->normIndices[j]-1].getY();
								m->vertexData[j].nz = inputNormals[sm->normIndices[j]-1].getZ();
							}
							printf("OBJMesh:- Normals Loaded\n");
						}
			
			
//#ifdef OBJ_USE_TANGENTS_BUMPMAPS
//			m->GenerateTangents();
//#endif

			cellGcmAddressToOffset(&m->vertexData->x, &m->vertexOffsets[VERTEX_POSITION]);
			cellGcmAddressToOffset(&m->vertexData->rgba, &m->vertexOffsets[VERTEX_COLOUR]);
			if(!sm->texIndices.empty())
			{
				cellGcmAddressToOffset(&m->vertexData->u, &m->vertexOffsets[VERTEX_TEXCOORD]);
			}
			//cellGcmAddressToOffset(indices, &m->vertexOffsets[VERTEX_INDEX]);
			AddChild(m);
		}
		delete inputSubMeshes[i];
	}
	return true;
}

void OBJMesh::Draw(VertexShader &vertex, FragmentShader &fragment) {
	Mesh::Draw(vertex, fragment);
	for(unsigned int i = 0; i < children.size(); ++i) {
		children.at(i)->Draw(vertex, fragment);
	}
};