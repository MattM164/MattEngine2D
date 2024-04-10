#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"


using namespace std;

void SaveScene(std::vector<GameObject>& objects, string filename) {
    filename = filename + ".scene";
    ofstream outputFile(filename, ios::binary);
    if (outputFile.is_open()) {
        //Meta Data
        //Scene Name
        outputFile << filename << endl;

        //Objects
        for (size_t i = 0; i < objects.size(); i++)
        {
            //Name
            outputFile << objects[i].name << "*";\
            //Position
            outputFile << objects[i].Transform.getPosition().x << "*" << objects[i].Transform.getPosition().y << "*";
            //Scale
            outputFile << objects[i].Transform.getScale().x << "*" << objects[i].Transform.getScale().y << "*";
            //Texture
            outputFile << objects[i].myTexture << "*";
			//RenderLayer
			outputFile << objects[i].renderLayer << "*";


			if (i != objects.size() - 1) {
				outputFile << endl;
			}
            
        }
		//outputFile << endl;
		outputFile.close();
    }
}

//vector<GameObject> LoadScene(string filename) {
void LoadScene(string filename, vector<GameObject> &gameObjects) {
	filename = filename + ".scene";
	string sceneData = "";
	int objectnum = 0;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
		//GetSceneName
		getline(inputFile, sceneData);
		while (inputFile) {
			//name
			string lname;
			getline(inputFile, sceneData, '*');
			lname = sceneData;
			string lposx;
			getline(inputFile, sceneData, '*');
			lposx = sceneData;
			string lposy;
			getline(inputFile, sceneData, '*');
			lposy = sceneData;
			string lscalex;
			getline(inputFile, sceneData, '*');
			lscalex = sceneData;
			string lscaley;
			getline(inputFile, sceneData, '*');
			lscaley = sceneData;
			string ltexture;
			getline(inputFile, sceneData, '*');
			ltexture = sceneData;
			string lrenderlayer;
			getline(inputFile, sceneData, '*');
			lrenderlayer = sceneData;

			GameObject loadedGameObject(ltexture);
			loadedGameObject.name = lname;
			loadedGameObject.position.x = stof(lposx);
			loadedGameObject.position.y = stof(lposy);
			loadedGameObject.scale.x = stof(lscalex);
			loadedGameObject.scale.y = stof(lscaley);
			loadedGameObject.myTexture = ltexture;
			loadedGameObject.numInWorldObjects = objectnum;
			loadedGameObject.renderLayer = stoi(lrenderlayer);
			//loadedGameObject.Setup();
			
			gameObjects.push_back(loadedGameObject);
			cout << "name: " << loadedGameObject.name << endl;
			cout << "x: " << loadedGameObject.position.x << endl;
			cout << "y: " << loadedGameObject.position.y << endl;
			cout << "scale x: " << loadedGameObject.scale.x << endl;
			cout << "scale y: " << loadedGameObject.scale.y << endl;
			cout << "tex name: " << loadedGameObject.myTexture << endl;
			objectnum++;
			if (getline(inputFile, sceneData, '\n')) {
				cout << "got that last backslash n" << endl;
			}
			

			
		}
	}
	else {
		
	}
	//cout << "Done reading file" << endl;
	getline(inputFile, sceneData, '\n');
	inputFile.close();
    //return gameObjects;
}



/*#pragma once
#include "raylib.h"
#include "ObjectClass.h"
#include "WorldObjectsVectorClass.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
class SceneHandler {
public:

	//WorldObjects worldObjects;

	void LoadScene(string sceneToLoad, WorldObjects* worldObjects) {

		ifstream sceneFile;
		sceneFile.open("C:/Users/Matthew Mulholland/source/repos/MattEngine/Scenes/" + sceneToLoad + ".txt");

		string sceneData = "";
		if (sceneFile.is_open()) {
			while (sceneFile) {
				//We have the line up to '#', now lets read into it
				getline(sceneFile, sceneData, '#');
				cout << sceneData << endl;
				if (sceneData == "OBJ") {
					//Scene Object
					getline(sceneFile, sceneData, '#');
					if (sceneData == "cube") {
						//Defaultcube... might get rid of this
						string cubeName;
						float posx;
						float posy;
						float posz;
						float length;
						float width;
						float hight;
						Color color;
						unsigned char rc;
						unsigned char gc;
						unsigned char bc;
						unsigned char ac;
						getline(sceneFile, sceneData, '#');
						cubeName = sceneData;
						getline(sceneFile, sceneData, '#');
						posx = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						posy = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						posz = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						length = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						width = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						hight = stof(sceneData);

						getline(sceneFile, sceneData, '#');
						cout << "actual color: " << sceneData << endl;
						rc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						gc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						bc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						ac = stoi(sceneData);
						color = {rc,gc,bc,ac};

						//Draw me?
						bool drawMe = true;
						getline(sceneFile, sceneData, '#');
						if (sceneData == "1") {
							drawMe = true;
						}
						else {
							drawMe = false;
						}

						Vector3 newPosition = {posx,posy,posz};
						myObject newObject(cubeName,newPosition, length,width,hight,color, drawMe);
						newObject.isCube = true;
						worldObjects->addObject(newObject);
						getline(sceneFile, sceneData, '\n');//To get passed the darn new line character
					}
					else if (sceneData == "model") {
						//Model to load
						string objectName;
						float posx;
						float posy;
						float posz;
						float length;
						float width;
						float hight;
						Color color;
						unsigned char rc;
						unsigned char gc;
						unsigned char bc;
						unsigned char ac;
						string modelFile;
						string textureFile;

						getline(sceneFile, sceneData, '#');
						objectName = sceneData;
						getline(sceneFile, sceneData, '#');
						posx = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						posy = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						posz = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						length = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						width = stof(sceneData);
						getline(sceneFile, sceneData, '#');
						hight = stof(sceneData);

						getline(sceneFile, sceneData, '#');
						cout << "actual color: " << sceneData << endl;
						rc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						gc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						bc = stoi(sceneData);
						getline(sceneFile, sceneData, '#');
						ac = stoi(sceneData);
						color = { rc,gc,bc,ac };

						//Draw me?
						bool drawMe = true;
						getline(sceneFile, sceneData, '#');
						if (sceneData == "1") {
							drawMe = true;
						}
						else {
							drawMe = false;
						}

						//Model and texture file paths
						getline(sceneFile, sceneData, '#');
						modelFile = sceneData;
						getline(sceneFile, sceneData, '#');
						textureFile = sceneData;

						Vector3 newPosition = { posx,posy,posz };
						myObject newObject(objectName, modelFile, textureFile, newPosition, 1.0f, drawMe);  ///1.0f is for scale, should probably add that
						/////////myObject(string name, string model, string texture, Vector3 position, float scale, bool drawMe2)
						newObject.isCube = false;
						worldObjects->addObject(newObject);
						getline(sceneFile, sceneData, '\n');//To get passed the darn new line character
					}


				}
			}
			
		}
		
		//cout << sceneData << endl;
		//return worldObjects;
		sceneFile.close();
		
	}

	void SaveScene(string sceneToSave, WorldObjects* worldObjects) {

		ofstream sceneFile;
		sceneFile.open("C:/Users/Matthew Mulholland/source/repos/MattEngine/Scenes/" + sceneToSave + ".txt");

		if (sceneFile.is_open()) {
			for (int i = 0; i < worldObjects->WorldObjects.size(); i++)
			{
				if (worldObjects->WorldObjects[i].isCube == true) {
					//We are a cube, save as cube.  I really need to get rid of this
					myObject object = worldObjects->WorldObjects[i];
					//Get color wow this is bad
					Color color;
					string objectData = "OBJ#cube#" + object.myName + "#" + to_string(object.myPosition.x) + "#" + to_string(object.myPosition.y) + "#" + to_string(object.myPosition.z) + "#" + to_string(object.length) + "#" + to_string(object.width) + "#" + to_string(object.hight) + "#" + to_string(object.color.r) + "#" + to_string(object.color.g) + "#" + to_string(object.color.b) + "#" + to_string(object.color.a) + "#" + to_string(object.drawMe) + "#" + '\n';
					//Add to the File
					sceneFile << objectData;
				}
				else if (worldObjects->WorldObjects[i].isCube == false) {
					//Model to save
					myObject object = worldObjects->WorldObjects[i];
					Color color;
					string objectData = "OBJ#model#" + object.myName + "#" + to_string(object.myPosition.x) + "#" + to_string(object.myPosition.y) + "#" + to_string(object.myPosition.z) + "#" + to_string(object.length) + "#" + to_string(object.width) + "#" + to_string(object.hight) + "#" + to_string(object.color.r) + "#" + to_string(object.color.g) + "#" + to_string(object.color.b) + "#" + to_string(object.color.a) + "#" + to_string(object.drawMe) + "#" + object.myModelFile + "#" + object.myTextureFile + "#" + '\n';
					//Add to the File
					sceneFile << objectData;
				}
			}
		}
		else {
			//File doesn't exist
		}
	}


private:


};
		*/