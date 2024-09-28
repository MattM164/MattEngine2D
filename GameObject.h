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
#include <stdlib.h>

using namespace std;

sf::RenderWindow window(sf::VideoMode(1920, 1080), "PlatFormMan");

vector<texObject> worldGameTex;
class GameObject;

//The Camera
sf::View Camera;

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

class SimpleSpriteCollision;

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
	float bounciness = 0.01;
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
			if (components[i]->returnName() == name) {
				return components[i];
			}
		}
	}

	void ComponentsUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			//if (components[i] != NULL) {
				//cout << "Updating " << components[i]->returnName() << endl;
				components[i]->Update();
			//}
		}
	}

	void ComponentsStart() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components[i]->Start();
			//cout << "Started____________________________________________________" << components[i];
		}
	}

	void destroy() {
		
		components.clear();
		worldObjects->erase(worldObjects->begin() + numInWorldObjects);
		for (size_t i = 0; i < worldObjects->size(); i++)
		{
			worldObjects->at(i).numInWorldObjects = i;
		}
		
		//ObjectListUpdate(worldObjects);
		//delete this;
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



class SimpleSpriteCollision : public Component {
public:
	SimpleSpriteCollision() : Component(){}
	string compName = "SimpleSpriteCollision";

	//Variables
	bool isColliding = false;
	GameObject* collidingWith = NULL;
	bool grounded = false;

	bool usePhysics = true;

	sf::Vector2f prevPos;

	sf::Transform sprite1;
	sf::Transform sprite2;

	bool leftCol = false;
	bool rightCol = false;
	bool upCol = false;
	bool downCol = false;

	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame
		grounded = false;
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
							rightCol = true;
						}
						else {
							rightCol = false;
						}
					}
					if (directionVector.x < 0) {
						if (abs(directionVector.x) > abs(directionVector.y)) {
							//cout << "Left" << endl;
							myObject->Transform.setPosition(prevPos.x, myObject->Transform.getPosition().y);
							myObject->xVelocity = -myObject->xVelocity * myObject->bounciness;
							leftCol = true;
						}
						else {
							leftCol = false;
						}
					}
					if (directionVector.y > 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "bottom" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
							myObject->yVelocity = -myObject->yVelocity * myObject->bounciness;
							grounded = true;
							downCol = true;
						}
						else {
							downCol = false;
						}
					}

					if (directionVector.y < 0) {
						if (abs(directionVector.x) < abs(directionVector.y)) {
							//cout << "top" << endl;
							myObject->Transform.setPosition(myObject->Transform.getPosition().x, prevPos.y);
							myObject->yVelocity = -myObject->yVelocity * myObject->bounciness;
							upCol = true;
						}
						else {
							upCol = false;
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
		if (dynamic_cast<SimpleSpriteCollision*>(myObject->components[2])->grounded) {
			myObject->xVelocity = myObject->xVelocity * 0.93;
			//cout << "Colliding and slowing down" << endl;
		}
		/*
		else if(myObject->xVelocity < 0 && dynamic_cast<SimpleSpriteCollision*>(myObject->components[2])->grounded) {
			myObject->xVelocity = myObject->xVelocity * 0.98;
			cout << "Colliding and slowing down LEFT" << endl;
		}
		*/

		//Clamp Top Speed
		if (myObject->xVelocity > 40) {
			myObject->xVelocity = 40;
		}
		if (myObject->xVelocity < -40) {
			myObject->xVelocity = -40;
		}

	}
	string returnName() {
		return compName;
	}


};


class PlayerMoveTest : public Component {
public:
	PlayerMoveTest() : Component() {}
	string compName = "PlayerMoveTest";

	float speed = 1.5;
	float fallspeed = -8.1;

	//Death variables
	bool dead = false;
	sf::Clock dclock;
	float deathelapsedTime = 0.0f;
	float switchTime = 0.7f; // Time interval to switch textures (in seconds)

	//Jump variables
	sf::Clock jclock;
	bool spacebarPressed = false;
	sf::Time pressTime;
	float jumpPower = 1;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		//Death Check
		if (myObject->dragCo < 0 && dead == false) {
			dead = true;
			deathelapsedTime = 0.0;
		}
		//Do the little bounce
		deathelapsedTime += dclock.restart().asSeconds();
		if (dead == true) {
			if (deathelapsedTime >= switchTime)
			{
				myObject->yAcceleration = -5;
				myObject->physicsLayer = 20;
				myObject->yVelocity = 62;
				deathelapsedTime = -99999.0f; //Make sure it only happens once
			}
			
			
		}



		//Exmaple of how to access GameObject
		//myObject->Transform.setColor(sf::Color::Red);
		if (dead == false) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				myObject->xVelocity += -speed;
				//myObject->Transform.move(2.5, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				myObject->xVelocity += speed;
				//myObject->Transform.move(-2.5, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				//myObject->Transform.move(0, -2.5);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				myObject->Transform.move(0, 2.5);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dynamic_cast<SimpleSpriteCollision*>(myObject->components[2])->grounded) {
				myObject->Transform.setPosition(myObject->Transform.getPosition().x, myObject->Transform.getPosition().y - 1);
				myObject->yVelocity = 100;
			}
			if (dynamic_cast<SimpleSpriteCollision*>(myObject->components[2])->grounded == false) {
				myObject->Transform.move(0, 0.3);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && myObject->yVelocity <= 0) {
				if (myObject->yVelocity < fallspeed) {
					myObject->yVelocity = fallspeed;
				}
			}
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


class CameraControl : public Component {
public:
	CameraControl() : Component() {}
	string compName = "CameraControl";

	GameObject* Player;



	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t) {
		return start + t * (end - start);
	}

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
		
		int count = myObject->worldObjects->size();
		for (size_t i = 0; i < count; i++)
		{
			if (myObject->worldObjects->at(i).name == "Player") {
				Player = &myObject->worldObjects->at(i);
			}
		}
		
	}


	void Update() override {
		//Will run every frame


		//Camera.setCenter(Player->Transform.getPosition().x, 500);
		Camera.setCenter(lerp(Camera.getCenter(), Player->Transform.getPosition(), 0.07).x, 500);
		if (window.mapCoordsToPixel(Player->Transform.getPosition()).x > 1200) {
			//Camera.setCenter(lerp(Camera.getCenter(), Player->Transform.getPosition(), 0.07).x + 240, Camera.getCenter().y);
		}
		else if (window.mapCoordsToPixel(Player->Transform.getPosition()).x < 500) {
			//Camera.setCenter(lerp(Camera.getCenter(), Player->Transform.getPosition(), 0.07).x - 460, Camera.getCenter().y);
		}

	}

	string returnName() {
		return compName;
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


//Enemy Components
class Goomba : public Component {
public:
	Goomba() : Component() {}
	string compName = "Goomba";
	sf::Clock clock;
	float elapsedTime = 0.0f;
	float switchTime = 0.5f; // Time interval to switch textures (in seconds)

	//Platform 'walls' physics layer
	int pwlayer = 7;

	int usingtex = 1;
	texObject goomba1;
	texObject goomba2;

	int direction = 0;
	float speed = 1;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
		
		goomba1.myFileName = "Assets\\goomba1.png";
		goomba1.texture.loadFromFile(goomba1.myFileName);
		goomba2.myFileName = "Assets\\goomba2.png";
		goomba2.texture.loadFromFile(goomba2.myFileName);

		//Set random elapsed time so they all aren't synced up
		srand( myObject->Transform.getPosition().x + myObject->numInWorldObjects);
		elapsedTime = 0.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (switchTime - 0.0)));
		//cout << "etime:" << elapsedTime << endl;




	}
	void Update() override {
		//Will run every frame
		// Update elapsed time
		elapsedTime += clock.restart().asSeconds();

		// Check if it's time to switch textures
		if (elapsedTime >= switchTime)
		{
			// Swap textures
			if (usingtex == 1) {
				myObject->Transform.setTexture(goomba2.texture);
				usingtex = 2;
			}else {
				myObject->Transform.setTexture(goomba1.texture);
				usingtex = 1;
			}

			// Reset elapsed time
			elapsedTime = 0.0f;
		}

		//Die if jumped on
		//if (dynamic_cast<SimpleSpriteCollision*>(myObject->components[1])->upCol) {
		if (dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->upCol) {

			//Error with deleting objects
			//Probably because when I delete the object the pointer to the list of components gets shifted? 
			//But why does it wtill work in reverse order...

			//Spawn Death Sprite
			GameObject mynewobj;
			mynewobj.name = "DeadGoomba";
			mynewobj.renderLayer = 1;
			//mynewobj.myTexture = "Assets\\goombaDead.png";
			mynewobj.numInWorldObjects = myObject->worldObjects->size();
			mynewobj.position = Camera.getCenter();
			mynewobj.Transform.setPosition(myObject->Transform.getPosition());
			mynewobj.worldObjects = myObject->worldObjects;
			myObject->worldObjects->push_back(mynewobj);
			myObject->worldObjects->at(mynewobj.numInWorldObjects).Setup();
			myObject->worldObjects->at(mynewobj.numInWorldObjects).SetObjectTexture(19);
			myObject->worldObjects->at(mynewobj.numInWorldObjects).Transform.setScale(5,5);
			myObject->worldObjects->at(mynewobj.numInWorldObjects).physicsLayer = 5;
			myObject->worldObjects->at(mynewobj.numInWorldObjects).Transform.setPosition(myObject->Transform.getPosition());
			//ObjectListUpdate(worldObjects);


			cout << "Delete Me: " << myObject->name << endl;
			//myObject->destroy(); //doesnt work correctly
			//Temp way to 'delete' object. Just make it invisible and remove all components
			myObject->Transform.setScale(0, 0);
			myObject->physicsLayer = 99;
			myObject->components.clear();
			//Player Bounce
			myObject->worldObjects->at(15).yVelocity = 50;

			return;
		}

		if (dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->leftCol) {
			//myObject->worldObjects->at(15).Transform.setScale(0,0);
			myObject->worldObjects->at(15).dragCo = -1;
			//myObject->worldObjects->at(15).components.clear();

			return;
		}
		if (dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->rightCol) {
			//myObject->worldObjects->at(15).Transform.setScale(0, 0);
			myObject->worldObjects->at(15).dragCo = -1;
			//myObject->worldObjects->at(15).components.clear();

			return;
		}

		//Movement Mechanics
		for (size_t i = 0; i < myObject->worldObjects->size(); i++)
		{


			if (i != myObject->numInWorldObjects && myObject->worldObjects->at(i).physicsLayer == 7 && myObject->worldObjects->at(i).Transform.getGlobalBounds().intersects(myObject->Transform.getGlobalBounds())) {
				direction = !direction;
			}
		}

		if (direction == 0) {
			myObject->Transform.move(-speed, 0);
		}
		else if(direction == 1) {
			myObject->Transform.move(speed, 0);
		}
		

	}

	string returnName() {
		return compName;
	}

};


class DeathWall : public Component {
public:
	DeathWall() : Component() {}
	string compName = "DeathWall";

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		if (dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->upCol) {
			//myObject->worldObjects->at(15).Transform.setScale(0,0);
			myObject->worldObjects->at(15).dragCo = -1;
			//myObject->worldObjects->at(15).components.clear();
			cout << "Deaed" << endl;
			return;
		}


		//Exmaple of how to access GameObject
		//dynamic_cast<ComponentName*>(WorldObjects[ObjectIndex].components[ComponentIndex])->speed = 700;
	}

	string returnName() {
		return compName;
	}

};





class PlayerAnim : public Component {
public:
	PlayerAnim() : Component() {}
	string compName = "PlayerAnim";
	////////////////////////////////

	sf::Clock clock;
	//Walking Clock
	float walkElapsedTime = 0.0f;
	float walkSwitchTime = 0.15f; // Time interval to switch textures (in seconds)

	//Flying Clock
	sf::Clock flyclock;
	float flyElapsedTime = 0.0f;
	float flySwitchTime = 0.1f; // Time interval to switch textures (in seconds)

	//Idle Frame
	texObject idle1;

	//Walking Frames
	texObject walk1;
	texObject walk2;
	int usingwalk = 0;
	bool walking = false;

	//Flying Frames
	texObject fly1;
	texObject fly2;
	int usingfly = 0;
	bool flying = false;

	//Crouch Frame
	texObject crouch1;

	//Look Up frame
	texObject lookup1;

	//Dead Frame
	texObject dead1;


	void Start() override {
		//Will run once at the start
		
		//Idle Frame:
		idle1.myFileName = "Assets\\ChickenSprites\\ChickenIdle.png";
		idle1.texture.loadFromFile(idle1.myFileName);

		//Walking Frames:
		walk1.myFileName = "Assets\\ChickenSprites\\ChickenWalkOne.png";
		walk1.texture.loadFromFile(walk1.myFileName);
		walk2.myFileName = "Assets\\ChickenSprites\\ChickenWalkTwo.png";
		walk2.texture.loadFromFile(walk2.myFileName);

		//Flying Frames:
		fly1.myFileName = "Assets\\ChickenSprites\\ChickenFlyOne.png";
		fly1.texture.loadFromFile(fly1.myFileName);
		fly2.myFileName = "Assets\\ChickenSprites\\ChickenFlyTwo.png";
		fly2.texture.loadFromFile(fly2.myFileName);

		//Death frame:
		dead1.myFileName = "Assets\\ChickenSprites\\ChickenDed.png";
		dead1.texture.loadFromFile(dead1.myFileName);

		//Crouch frame
		crouch1.myFileName = "Assets\\ChickenSprites\\ChickenCrouch.png";
		crouch1.texture.loadFromFile(crouch1.myFileName);

		//Look Up frame
		lookup1.myFileName = "Assets\\ChickenSprites\\ChickenLoopUp.png";
		lookup1.texture.loadFromFile(lookup1.myFileName);


	}
	void Update() override {
		//Will run every frame

		//Turning
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			myObject->Transform.setScale(4, 4);
			walking = true;
		}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			myObject->Transform.setScale(-4, 4);
			walking = true;
		}
		else {
			walking = false;
		}

		
		//Walking Animation
		walkElapsedTime += clock.restart().asSeconds();
		if (walking == true && dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->grounded && walkElapsedTime >= walkSwitchTime)
		{
			// Swap textures
			if (usingwalk == 0) {
				myObject->Transform.setTexture(walk1.texture);
				usingwalk = 1;
			}
			else if(usingwalk == 1){
				myObject->Transform.setTexture(idle1.texture);
				usingwalk = 2;
			}
			else if (usingwalk == 2) {
				myObject->Transform.setTexture(walk2.texture);
				usingwalk = 3;
			}
			else if (usingwalk == 3) {
				myObject->Transform.setTexture(idle1.texture);
				usingwalk = 0;
			}

			// Reset elapsed time
			walkElapsedTime = 0.0f;
		}
		else if(walking == false && dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->grounded == true) {
			//Grounded IDLE
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				myObject->Transform.setTexture(crouch1.texture);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				myObject->Transform.setTexture(lookup1.texture);
			}
			else {
				myObject->Transform.setTexture(idle1.texture);
			}
		}

		//Flying Animation
		flyElapsedTime += flyclock.restart().asSeconds();
		if (dynamic_cast<SimpleSpriteCollision*>(myObject->GetComponent("SimpleSpriteCollision"))->grounded == false && flyElapsedTime >= flySwitchTime)
		{
			// Swap textures
			if (usingfly == 0) {
				myObject->Transform.setTexture(fly1.texture);
				usingfly = 1;
			}
			else {
				myObject->Transform.setTexture(fly2.texture);
				usingfly = 0;
			}

			// Reset elapsed time
			flyElapsedTime = 0.0f;
		}

		//Dead animation
		if (dynamic_cast<PlayerMoveTest*>(myObject->GetComponent("PlayerMoveTest"))->dead) {
			
			myObject->Transform.setTexture(dead1.texture);
		}

		
	}

	string returnName() {
		return compName;
	}

};




class Parallax1 : public Component {
public:
	Parallax1() : Component() {}
	string compName = "Parallax1";

	float length = 0;
	float startpos = 0;
	float parallaxEffect = 0.5;


	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
		startpos = myObject->Transform.getPosition().x;

		if (myObject->renderLayer == 5) {
			parallaxEffect = 0.8;
		}
		else if (myObject->renderLayer == 4) {
			parallaxEffect = 0.6;
		}else if (myObject->renderLayer == 3) {
			parallaxEffect = 0.35;
		}else if (myObject->renderLayer == 2) {
			parallaxEffect = 0.2;
		}

	}
	void Update() override {
		//Will run every frame

		float dist = (Camera.getCenter().x * parallaxEffect);
		myObject->Transform.setPosition(startpos + dist, myObject->Transform.getPosition().y);
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
	{"CameraControl", []() { return new CameraControl(); }},
	{"Goomba", []() { return new Goomba(); }},
	{"PlayerAnim", []() { return new PlayerAnim(); }},
	{"Parallax1", []() { return new Parallax1(); }},
	{"DeathWall", []() { return new DeathWall(); }},

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