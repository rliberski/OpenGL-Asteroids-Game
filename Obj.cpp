#include "Obj.h"
#include <iostream>
#include <iterator>

Obj::Obj(string filename)
{
	fstream file;
    file.open(filename, std::ios::in);
    if(file.good())	//load data
    {
		///////////////
		string line;
		while(getline(file,line))
		{
			istringstream ss(line);
			istream_iterator<string> begin(ss), end;
			vector<string> data(begin, end); 

			if(data.size()>0 && data[0] == "v")
			{
				processVertices(data);
			}
			if(data.size()>0 && data[0] == "vn")
			{
				processNormals(data);
			}
			if(data.size()>0 && data[0] == "f")
			{
				processFaces(data);
			}

		}
        ///////////////
        file.close();
    }
	else
	{
		delete this;
	}
}

void Obj::processVertices(vector<string> data)
{
	std::string::size_type sz;
	double x = std::stod(data[1],&sz);
	double y = std::stod(data[2],&sz);
	double z = std::stod(data[3],&sz);

	vertices.push_back(glm::vec3(x,y,z));
}

void Obj::processNormals(vector<string> data)
{
	std::string::size_type sz;
	double x = std::stod(data[1],&sz);
	double y = std::stod(data[2],&sz);
	double z = std::stod(data[3],&sz);

	normals.push_back(glm::vec3(x,y,z));
}

void Obj::processFaces(vector<string> data)
{
	std::string::size_type sz;

	for(int i=1;i<data.size();i++)
	{
		bool vertice = true;
		bool normal = false;
		string v="";
		string vn="";
		for(int j=0;j<data[i].length();j++)
		{
			if(data[i][j]=='/' && vertice)
				vertice = false;
			else if(data[i][j]=='/' && !vertice)
				normal = true;
			else if(vertice)
				v+=data[i][j];
			else if(normal)
				vn+=data[i][j];
		}
		unsigned short int f = std::stoi(v,&sz);
		f--;
		faces.push_back(f);
		unsigned short int fn = std::stoi(v,&sz);
		fn--;
		facesNormals.push_back(fn);
	}
}
