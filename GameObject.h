#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
//#include "Components.h"
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

using namespace std;
vector<sf::Texture> worldGameTex;
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
};

class GameObject {

public:
	//Basic
	string name = "";
	sf::Sprite Transform;
	sf::Texture texture;
	int renderLayer = 0;
	int numInWorldObjects = 0;
	vector<Component*> components; //List of all attached Components
	vector<GameObject>* worldObjects;

	//Properties - Only for initiation, do not change at runtime
	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::Vector2f scale = sf::Vector2f(1,1);
	sf::Vector2f origin = sf::Vector2f(0,0);
	sf::Color objColor = sf::Color();
	float rotation = 0;
	string myTexture = "";

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

		LoadNewTexture(myTexture);

		//Set Origin to middle of texture (by default...)
		//Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		//hitBox = Transform.getGlobalBounds();
	}

	void LoadNewTexture(string newtex) {
		//Add In Texture
		//sf::Texture loadedtexture;
		if (!texture.loadFromFile(newtex))
		{
			// error
			cout << "Error loading new Texture for: " << name << " Object" << endl;
		}
		//sf::IntRect newrect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y)); //NEEDS WORK
		//Transform.setTextureRect(newrect);
		//Need to fix how it places the texture/set the rect///////////////
		//Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		//texture = loadedtexture;

		worldGameTex.push_back(texture);
		
		Transform.setTexture(worldGameTex.back());
		Transform.setOrigin((sf::Vector2f)texture.getSize() / 2.f);
		hitBox = Transform.getGlobalBounds();
		//Transform.setTextureRect(texture.);
	}

	void AddComponent(Component *mycomponent, GameObject *tempObj) {
		components.push_back(mycomponent);
		components.back()->setGameObject(tempObj);
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


//Template
class MyComponent : public Component {
public:
	MyComponent() : Component() {}
	string compName = "PlayerMoveTest";

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		//Exmaple of how to access GameObject
		myObject->Transform.move(7, 0);
	}

};

class PlayerMoveTest : public Component {
public:
	PlayerMoveTest() : Component() {}
	string compName = "PlayerMoveTest";

	float speed = 5;

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
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			myObject->xVelocity += speed;
		}

	}

	void setSpeed(float newSpeed) {
		speed = newSpeed;
	}

	//Used for adding info to Inspector window in editor mode
	void EditorUI() override {
		ImGui::InputFloat("Speed", &speed, 0.05, 0.3, "%.2f");

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