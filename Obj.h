#pragma once

#ifndef _OBJ_H
#define _OBJ_H

#include <glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Obj
{
public:
	Obj(string filename);

	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<unsigned short int> faces;
	vector<unsigned short int> facesNormals;

private:
	void processVertices(vector<string> data);
	void processNormals(vector<string> data);
	void processFaces(vector<string> data);	
};

#endif