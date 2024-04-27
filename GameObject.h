#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
//#include "Components.h"
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"
#include <time.h>
#include <chrono>
#include <map>
#include <functional>
#include "textureObject.h"

using namespace std;
vector<texObject> worldGameTex;
class GameObject;

class Component {
public:
	string compName = "";
	GameObject* myObject;
	bool enabled = true;
	virtual void Start() {}
	virtual void Update() {}
	virtual void setGameObject(GameObject* thisObject) {
		myObject = thisObject;
	}
	virtual void EditorUI() {}
	virtual string returnName() {
		return compName;
	}
};

class GameObject {

public:
	//Basic
	string name = "";
	sf::Sprite Transform;
	sf::Texture texture;
	int textureIndex = 0;
	int renderLayer = 0;
	int numInWorldObjects = 0;
	vector<Component*> components; //List of all attached Components
	vector<GameObject>* worldObjects;

	//Save to file
	bool saveToFile = true;

	//Properties - Only for initiation, do not change at runtime
	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::Vector2f scale = sf::Vector2f(1,1);
	sf::Vector2f origin = sf::Vector2f(0,0);
	sf::Color objColor = sf::Color();
	float rotation = 0;
	string myTexture = " ";

	//Physics
	bool usePhysics = false;
	  //should we collide with other physics enabled objects in the same layer?
	bool collisions = false;
	  //Only checks collisions between objects inside the same layer
	sf::FloatRect hitBox = Transform.getGlobalBounds();
	float mass = 1;
	float gravity = -9.81;
	int physicsLayer = 0;
	float yVelocity = 0;
	float yAcceleration = gravity;
	float xVelocity = 0;
	float xAcceleration = 0;
	float bounciness = 0.2;
	float dragCo = 0.88;

	///Constructors
	GameObject() {
		//At least add a genaric texture
		myTexture = "DefaultSprite.png";
		Setup();
	}
	GameObject(string itexture) {
		myTexture = itexture;
		Setup();
	}
	GameObject(string myname, string itexture, int mypositionx, int mypositiony, int myscalex, int myscaley) {
		myTexture = itexture;
		position.x = mypositionx;
		position.y = mypositiony;
		scale.x = myscalex;
		scale.y = myscaley;
		name = myname;
		Setup();
	}
	GameObject(sf::Vector2f myposition, sf::Vector2f scalei) {
		position = myposition;
		scale = scalei;

		Setup();
	}
	GameObject(int texindex) {
		
		textureIndex = texindex;
		Setup();
	}

	/*
	~GameObject() {
		cout << "Deleting " << name << endl;
		delete this;
	}
	*/
	
	void Setup() {
		Transform.setPosition(position);
		Transform.setOrigin(origin);
		Transform.setRotation(rotation);
		Transform.setScale(scale);
		//Add In Texture

		//LoadNewTexture(myTexture);
		SetObjectTexture(textureIndex);

		//Set Origin to middle of texture (by default...)
		//Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		//hitBox = Transform.getGlobalBounds();
	}

	void LoadNewTexture(string newtex) {
		//Add In Texture
		//sf::Texture loadedtexture;
		//if (!texture.loadFromFile(newtex))
		//{
			// error
		//	cout << "Error loading new Texture for: " << name << " Object" << endl;
		//}
		//sf::IntRect newrect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)); //NEEDS WORK
		//Transform.setTextureRect(newrect);
		//Need to fix how it places the texture/set the rect///////////////
		//Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		//texture = loadedtexture;


		texObject loadedTex;
		loadedTex.myFileName = newtex;

		if (!loadedTex.texture.loadFromFile(newtex))
		{
			// error
			cout << "Error loading new Texture for: " << name << " Object" << endl;
		}

		worldGameTex.push_back(loadedTex);
		
		Transform.setTexture(worldGameTex.back().texture);
		textureIndex = worldGameTex.size() - 1;
		sf::IntRect newrect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(worldGameTex.back().texture.getSize().x, worldGameTex.back().texture.getSize().y)));
		Transform.setTextureRect(newrect);
		Transform.setOrigin((sf::Vector2f)worldGameTex.back().texture.getSize() / 2.0f);
		//Transform.setOrigin(sf::Vector2f(worldGameTex.back().texture.getSize().x / 2.0f, worldGameTex.back().texture.getSize().y / 2.0f));
		//Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		//Transform.setTextureRect(newrect);
		hitBox = Transform.getGlobalBounds();
		//Transform.setTextureRect(texture.);
	}
	
	void SetObjectTexture(int index) {
		//Set texture from the worldtex vector
		Transform.setTexture(worldGameTex[index].texture);
		textureIndex = index;
		//Transform.setOrigin((sf::Vector2f)worldGameTex[index].texture.getSize() / 2.f);
		//hitBox = sf::Vector2f(Transform.getGlobalBounds().getSize().x / 2.0f, );
		//hitBox = Transform.getLocalBounds();

		//Transform.setOrigin(sf::Vector2f(worldGameTex[index].texture.getSize().x / 2.0f, worldGameTex[index].texture.getSize().y / 2.0f));
	
		sf::IntRect newrect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(worldGameTex[index].texture.getSize().x, worldGameTex[index].texture.getSize().y)));
		Transform.setTextureRect(newrect);
		Transform.setOrigin((sf::Vector2f)worldGameTex[index].texture.getSize() / 2.0f);
		hitBox = Transform.getGlobalBounds();
	
	}

	void AddComponent(Component *mycomponent, GameObject *tempObj) {
		components.push_back(mycomponent);
		if (tempObj != NULL) {
			components.back()->setGameObject(tempObj);
		}
	}

	Component* GetComponent(string name) {
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components[i]->compName == name) {
				return components[i];
			}
		}
	}

	void ComponentsUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components[i]->Update();
		}
	}

	void ComponentsStart() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components[i]->Start();
		}
	}



};







//To make a component, copy and paste the template below.  Change the three instances of 'MyComponent'
// to the name of your component. Then, duplicate a line from the map at the bottom of the GameObject.h file
// and replace the name of the class to your new class name.  You may need to declare the name of the component
// at the top of this file to access it from below.


//Template
class MyComponent : public Component {
public:
	MyComponent() : Component() {}
	string compName = "MyComponent";

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		//Exmaple of how to access GameObject
		//dynamic_cast<ComponentName*>(WorldObjects[ObjectIndex].components[ComponentIndex])->speed = 700;
	}

	string returnName() {
		return compName;
	}

};

class PlayerMoveTest : public Component {
public:
	PlayerMoveTest() : Component() {}
	string compName = "PlayerMoveTest";

	float speed = 1;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame
		
		//Exmaple of how to access GameObject
		//myObject->Transform.setColor(sf::Color::Red);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			myObject->xVelocity += -speed;
			//myObject->Transform.move(2.5, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			myObject->xVelocity += speed;
			//myObject->Transform.move(-2.5, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			myObject->Transform.move(0, -2.5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			myObject->Transform.move(0, 2.5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			myObject->Transform.setPosition(myObject->Transform.getPosition().x, myObject->Transform.getPosition().y - 1);
			myObject->yVelocity = 100;
		}

	}

	string returnName() {
		return compName;
	}

	//Used for adding info to Inspector window in editor mode
	void EditorUI() override {
		ImGui::InputFloat("Speed", &speed, 0.05, 0.3, "%.2f");

	}

};


class ChangeSpeed : public Component {
public:
	ChangeSpeed() : Component() {}
	string compName = "ChangeSpeed";

	float fastSpeed = 50;
	float slowSpeed = 5;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
			dynamic_cast<PlayerMoveTest*>(myObject->components[0])->speed = fastSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			dynamic_cast<PlayerMoveTest*>(myObject->components[0])->speed = slowSpeed;
		}
	}

	string returnName() {
		return compName;
	}

	void EditorUI() override {
		ImGui::InputFloat("Fast Speed", &fastSpeed, 0.5, 1, "%.2f");
		ImGui::InputFloat("Slow Speed", &slowSpeed, 0.5, 1, "%.2f");

	}


};

class SimpleSpriteCollision : public Component {
public:
	SimpleSpriteCollision() : Component(){}
	string compName = "SimpleSpriteCollision";

	//Variables
	bool isColliding = false;
	GameObject* collidingWith = NULL;

	bool usePhysics = true;

	sf::Vector2f prevPos;

	sf::Transform sprite1;
	sf::Transform sprite2;


	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame
		for (size_t i = 0; i < myObject->worldObjects->size(); i++)
		{
			

			if (i != myObject->numInWorldObjects && myObject->physicsLayer == myObject->worldObjects->at(i).physicsLayer && myObject->worldObjects->at(i).Transform.getGlobalBounds().intersects(myObject->Transform.getGlobalBounds())) {
				isColliding = true;
				sf::FloatRect overlap;
				myObject->worldObjects->at(i).Transform.getGlobalBounds().intersects(myObject->Transform.getGlobalBounds(), overlap);//    myObject->worldObjects->at(i).Transform.getGlobalBounds().intersects(myObject->Transform.getGlobalBounds());
		
				 // Calculate center of the overlap
				sf::Vector2f overlapCenter(overlap.left + overlap.width / 2, overlap.top + overlap.height / 2);

				// Calculate direction vector from sprite2 to the center of the overlap
				sf::Vector2f directionVector = overlapCenter - myObject->Transform.getPosition();

				// Normalize direction vector
				float length = std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);
				if (length != 0) {
					directionVector /= length;
				}
				
				//BASIC
				if (!usePhysics) {
					if (directionVector.x > 0) {
						if (abs(directionVector.x) > abs(directionVector.y)) {
							//cout << "Right" << endl;
							myObject->Transform.setPosition(prevPos.x, myObject->Transform.getPosition().y);
						}
					}
					if (directionVector.x < 0) {
						if (abs(directionVector.x) > abs(directionVector.y)) {
							//cout << "Left" << endl;
							myObject->Transform.setPosition(prevPos.x, myObject->Transform.getPosition().y);
						}
					}
					if (directionVector.y > 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "Top" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
						}
					}
					if (directionVector.y < 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "Bottom" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
						}
					}
				}
				//PHYSICS
				else {
					if (directionVector.x > 0) {
						if (abs(directionVector.x) > abs(directionVector.y)) {
							//cout << "Right" << endl;
							myObject->Transform.setPosition(prevPos.x, myObject->Transform.getPosition().y);
							myObject->xVelocity = -myObject->xVelocity * myObject->bounciness;
						}
					}
					if (directionVector.x < 0) {
						if (abs(directionVector.x) > abs(directionVector.y)) {
							//cout << "Left" << endl;
							myObject->Transform.setPosition(prevPos.x, myObject->Transform.getPosition().y);
							myObject->xVelocity = -myObject->xVelocity * myObject->bounciness;
						}
					}
					if (directionVector.y > 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "Top" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
							myObject->yVelocity = -myObject->yVelocity * myObject->bounciness;
						}
					}
					if (directionVector.y < 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "Bottom" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
							myObject->yVelocity = -myObject->yVelocity * myObject->bounciness;
						}
					}
				}
				collidingWith = &myObject->worldObjects->at(i);
			

				//Super Simple:
				/*
				cout << "Colliding with " << myObject->worldObjects->at(i).name << "  -  " << prevPos.x << endl;
				myObject->Transform.setPosition(prevPos);
				*/
				
			}
			else {
				//cout << "Colliding with Nothing" << endl;
				isColliding = false;
				collidingWith = NULL;
			}
		}
		prevPos = myObject->Transform.getPosition();


	}

	string returnName() {
		return compName;
	}


};

class SimplePhysics : public Component {
public:
	SimplePhysics() : Component() {}
	string compName = "SimplePhysics";

	float gravity = -9.81;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<int, std::milli> timeElapsed;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		float deltaTime = 0.16666; //Timestep, set to 60 fps.  144fps is 0.0694444
		end = std::chrono::steady_clock::now(); // Get current time
	   // Calculate time elapsed since start
		timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		if (timeElapsed >= std::chrono::milliseconds(1000 / 60)) { //Check if it's time to update physics //60fps
			float currentYpos = myObject->Transform.getPosition().y;
			myObject->yVelocity += myObject->yAcceleration * deltaTime;// *WorldObjects[i].dragCo; //Updates Velocity, including drag co: might not be working
			myObject->Transform.setPosition(myObject->Transform.getPosition().x, currentYpos -= myObject->yVelocity * deltaTime); //Updates position

			float currentXpos = myObject->Transform.getPosition().x;
			myObject->xVelocity += myObject->xAcceleration * deltaTime;// *WorldObjects[i].dragCo; //Updates Velocity, including drag co: might not be working
			myObject->Transform.setPosition(currentXpos -= myObject->xVelocity * deltaTime, myObject->Transform.getPosition().y); //Updates position
		}

	}
	string returnName() {
		return compName;
	}


};













GameObject* FindGameObject(string search, vector<GameObject>* worldObjects) {
	for (size_t i = 0; i < worldObjects->size(); i++)
	{
		if (worldObjects->at(i).name == search) {
			return &worldObjects->at(i);
		}
	}
}


std::map<std::string, std::function<Component* ()>> classFactory = {
	//{"ComponentName", []() { return new ComponentName(); }},
	{"PlayerMoveTest", []() { return new PlayerMoveTest(); }},
	{"SimplePhysics", []() { return new SimplePhysics(); }},
	{"SimpleSpriteCollision", []() { return new SimpleSpriteCollision(); }},
	{"ChangeSpeed", []() { return new ChangeSpeed(); }},
	
};

Component* CreateComponent(const std::string& className) {
	if (classFactory.find(className) != classFactory.end()) {
		return classFactory[className]();
	}
	return nullptr;
}

void CreateTexture(string newtex) {
	sf::Texture mytexture;
		if (!mytexture.loadFromFile(newtex))
		{
			// error
			cout << "Error loading new Texture" << endl;
		}
		texObject newtexture;
		newtexture.myFileName = newtex;
		newtexture.texture = mytexture;
		worldGameTex.push_back(newtexture);
}