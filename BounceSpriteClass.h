#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

class BounceSprite {
public:
	sf::Sprite sprite;
	float speed = 1;
	float rotationSpeed = 1;
	float xDir; //between -1 and 1
	float yDir; //between -1 and 1
	
	/*
	BounceSprite(sf::Texture myTexture, float mySpeed) {
		speed = mySpeed;
		sprite.setTexture(myTexture);
	}
	*/
	void move() {
		sprite.move(sf::Vector2f(xDir * speed, yDir * speed));
		sprite.rotate(rotationSpeed);
	}
	void windowCol(int winx, int winy) {
		float xpos = sprite.getPosition().x;
		float ypos = sprite.getPosition().y;
		if (xpos < 0) {
			sprite.setPosition(0,ypos);
			xDir = -xDir;
		}
		if (xpos > winx) {
			sprite.setPosition(winx, ypos);
			xDir = -xDir;
		}
		if (ypos < 0) {
			sprite.setPosition(xpos, 0);
			yDir = -yDir;
		}
		if (ypos > winy) {
			sprite.setPosition(xpos, winy);
			yDir = -yDir;
		}
	}

	void spriteCol() {
		
	}
	
private:

};