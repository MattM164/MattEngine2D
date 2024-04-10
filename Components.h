#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
//#include "GameObject.h"

using namespace std;

//Need findComponent function, add component

/*

//Declare before everything
class GameObject;


//Base Component Class
class Component {
protected:
	

public:
	GameObject* myObject;
	Component(GameObject* thisObject) : myObject(thisObject) {}
	virtual void Start(){}
	virtual void Update(){}
	
};



//To make a component, follow this template:

class MyComponent : public Component {
	public:
		MyComponent(GameObject* thisObject) : Component(thisObject) {}
		
		//myObject is a pointer to the object this component is attached to
		void Start() override {
			//Will run once at the start
		}
		void Update() override {
			//Will run every frame
			
			//Exmaple of how to access GameObject
			
		}

};




class BasicPlayerMovement : public Component {
public:
	BasicPlayerMovement(GameObject* thisObject) : Component(thisObject) {}


	int speed = 5;

	//myObject is a pointer to the object this component is attached to
	void Start() override {
		//Will run once at the start
	}
	void Update() override {
		//Will run every frame

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			cout << "test" << endl;
		}
		
	}

};

*/
