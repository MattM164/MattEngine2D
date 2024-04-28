#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <sstream>


using namespace std;

string currentScene = "";

void SaveScene(std::vector<GameObject>& objects, string filename) {
    //filename = filename + ".scene";
    ofstream outputFile(filename, ios::binary);
    if (outputFile.is_open()) {
        //Meta Data
        //Scene Name
        outputFile << filename << endl;
		//Num of Objects
		outputFile << objects.size() << endl;

        //Objects
        for (size_t i = 0; i < objects.size(); i++)
        {
			if (objects[i].saveToFile) {
				//Name
				outputFile << objects[i].name << "*"; \
				//Position
				outputFile << objects[i].Transform.getPosition().x << "*" << objects[i].Transform.getPosition().y << "*";
				//Scale
				outputFile << objects[i].Transform.getScale().x << "*" << objects[i].Transform.getScale().y << "*";
				//Rotation
				outputFile << objects[i].Transform.getRotation() << "*";
				//Texture
				outputFile << objects[i].myTexture << "*";
				//RenderLayer
				outputFile << objects[i].renderLayer << "*";
				//Physics Layer
				outputFile << objects[i].physicsLayer << "*";
				//Texture index
				outputFile << objects[i].textureIndex << "*";



				//Save Components
				//Num of components
				outputFile << objects[i].components.size() << "*";
				//Save names that we will compare to map later
				for (size_t k = 0; k < objects[i].components.size(); k++)
				{
					outputFile << objects[i].components[k]->returnName() << " ";
					//cout << objects[i].components[k]->returnName() << endl;
				}
				
				//outputFile << "*";

				if (i != objects.size() - 1) {
					outputFile << endl;
				}
			}
            
        }


		//Save all Textures and their indecies!
		outputFile << endl;
		//How many textures
		outputFile << worldGameTex.size() << "%";
		for (size_t i = 0; i < worldGameTex.size(); i++)
		{
			outputFile << worldGameTex[i].myFileName << "%";
		}


		//outputFile << endl;
		outputFile.close();
    }
}

//vector<GameObject> LoadScene(string filename) {
void LoadScene(string filename, vector<GameObject> &gameObjects) {
	//filename = filename + ".scene";

	//Get Just the File Name, in assets folder
	reverse(filename.begin(), filename.end());
	string s = filename;
	string delimiter = "\\";
	string newName = s.substr(0, s.find(delimiter));
	reverse(newName.begin(), newName.end());
	cout << newName << endl;
	string finalName = "Assets\\" + newName;
	cout << finalName << endl;
	currentScene = newName;
	//getline(reverse(filename.begin(), filename.end()), newName);

	worldGameTex.clear();
	string sceneData = "";
	int objectnum = 0;
	int numOfObjects = 0;
    ifstream inputFile(finalName);
    if (inputFile.is_open()) {
		//GetSceneName
		getline(inputFile, sceneData);
		//Get Object Count
		getline(inputFile, sceneData);
		string lobjectscount = sceneData;
		numOfObjects = stoi(lobjectscount);
		cout << "NUMBER OF OBJECTS: " << numOfObjects << endl;
		for (size_t i = 0; i < numOfObjects; i++)
		{

		
		//while (inputFile) {
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
			string lrot;
			getline(inputFile, sceneData, '*');
			lrot = sceneData;
			string ltexture;
			getline(inputFile, sceneData, '*');
			ltexture = sceneData;
			string lrenderlayer;
			getline(inputFile, sceneData, '*');
			lrenderlayer = sceneData;
			string lphysicslayer;
			getline(inputFile, sceneData, '*');
			lphysicslayer = sceneData;
			string ltextureindex;
			getline(inputFile, sceneData, '*');
			ltextureindex = sceneData;



			GameObject loadedGameObject(ltexture);

			//Components////////////////////////////////////////////

			//Get number of components
			string numComponents;
			getline(inputFile, sceneData, '*');
			numComponents = sceneData; //stoi
			//get the names into a vector
			vector<string> compNames;
			for (size_t i = 0; i < stoi(numComponents); i++)
			{
				getline(inputFile, sceneData, ' ');
				compNames.push_back(sceneData);
				cout << sceneData << endl;
			}

			
			//getline(inputFile, sceneData, '\n');




			
			//WorldObjects[0].AddComponent(CreateComponent("PlayerMoveTest"), &WorldObjects[0]);
			//////////////////////////////
			
			loadedGameObject.name = lname;
			loadedGameObject.position.x = stof(lposx);
			loadedGameObject.position.y = stof(lposy);
			loadedGameObject.scale.x = stof(lscalex);
			loadedGameObject.scale.y = stof(lscaley);
			loadedGameObject.rotation = stof(lrot);
			loadedGameObject.myTexture = ltexture;
			loadedGameObject.numInWorldObjects = objectnum;
			loadedGameObject.renderLayer = stoi(lrenderlayer);
			loadedGameObject.physicsLayer = stoi(lphysicslayer);
			loadedGameObject.textureIndex = stoi(ltextureindex);

			loadedGameObject.worldObjects = &gameObjects;
			//loadedGameObject.Setup();


			gameObjects.push_back(loadedGameObject);

			if (compNames.size() > 0) {
				for (size_t i = 0; i < compNames.size(); i++)
				{
					gameObjects.back().AddComponent(CreateComponent(compNames[i]), &gameObjects.back());
					cout << gameObjects.back().components[i]->returnName() << endl;
				}
			}
			else {
				//getline(inputFile, sceneData, '*');
			}


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
			//getline(inputFile, sceneData, '\n');
			

			
		}


		//Textures/////////////////////////////
	//num of textures
		int numTex;
		getline(inputFile, sceneData, '%');
		numTex = stoi(sceneData); //stoi
		vector<string> texNames;
		cout << numTex << endl;
		//get file names
		for (size_t i = 0; i < numTex; i++)
		{
			getline(inputFile, sceneData, '%');
			texNames.push_back(sceneData);
		}

		//get textures in list
		for (size_t i = 0; i < texNames.size(); i++)
		{
			

			reverse(texNames[i].begin(), texNames[i].end());
			string s = texNames[i];
			string delimiter = "\\";
			string newName = s.substr(0, s.find(delimiter));
			reverse(newName.begin(), newName.end());
			cout << newName << endl;
			string finalName = "Assets\\" + newName;
			//currentScene = newName;


			CreateTexture(finalName);
		}
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i].Setup();
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