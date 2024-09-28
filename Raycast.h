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
#include <cmath>
#include "GameObject.h"

using namespace std;

class Raycast {

public:
	sf::Vector2f origin;
	sf::Vector2f directionVec;
	sf::Vector2f normalizedVec;
	sf::Vector2f endPoint;
	float angle = 0;

	float length = 200;
	int physicsLayer = 0;

	Raycast(sf::Vector2f position, float anglei, float lengthi) {
		origin = position;
		angle = anglei;
		length = lengthi;
		endPoint = sf::Vector2f(origin.x + length * cos(angle * (3.14159265359f / 180.0f)), origin.y + length * sin(angle * (3.14159265359f / 180.0f)));
		//directionVec = endpoint;
		//normalizedVec = normalize(directionVec);
		//cout << "Direction Vector: " << directionVec.x << ", " << directionVec.y << endl;
		//cout << "Normalized Vector: " << normalizedVec.x << ", " << normalizedVec.y << endl;
	}


	void draw(sf::RenderWindow* window) {
		/*
		sf::Vector2f templine;
		templine.x = normalizedVec.x + origin.x;
		templine.y = normalizedVec.y + origin.y;
		//normalizedVec.x *= length;
		//normalizedVec.y *= length;
		sf::Vertex line[] =
		{
			sf::Vertex(origin),
			sf::Vertex(normalizedVec + origin * 2.0f)
		};
		*/
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = origin;
		//endPoint = sf::Vector2f(origin.x + length * cos(angle * (3.14159265359f / 180.0f)), origin.y + length * sin(angle * (3.14159265359f / 180.0f)));
		line[1].position = endPoint; // Extend the ray for visualization
		line[0].color = sf::Color::Red;
		line[1].color = sf::Color::Blue;
		window->draw(line);

		//window->draw(line, 2, sf::Lines);
	}

	sf::Vector2f checkCollision(sf::Vector2f rayOrigin, sf::Vector2f rayEnd, sf::Vector2f wall1, sf::Vector2f wall2) {
		const float den = ((rayOrigin.x - rayEnd.x) * (wall1.y - wall2.y) - (rayOrigin.y - rayEnd.y) * (wall1.x - wall2.x));
		if (den == 0) {
			return endPoint;
		}
		float t = ((rayOrigin.x - wall1.x) * (wall1.y - wall2.y) - (rayOrigin.y - wall1.y) * (wall1.x - wall2.x)) / den;
		float u = -((rayOrigin.x - rayEnd.x) * (rayOrigin.y - wall1.y) - (rayOrigin.y - rayEnd.y) * (rayOrigin.x - wall1.x)) / den;
		if (t >= 0 && t <= 1) {
			if (u >= 0 && u <= 1) {
				//cout << "COLLISION!!!" << endl;
				float px = rayOrigin.x + (t * (rayEnd.x - rayOrigin.x));
				float py = rayOrigin.y + (t * (rayEnd.y - rayOrigin.y));
				sf::Vector2f interceptPoint;
				interceptPoint.x = px;
				interceptPoint.y = py;
				//cout << interceptPoint.x << endl;
				return interceptPoint;
			}
		}
		return endPoint;
	}


	float magnitude(sf::Vector2f vec){
		return std::sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	sf::Vector2f normalize(sf::Vector2f vec) {
		float mag = magnitude(directionVec);
		if (mag != 0) {
			vec.x /= mag;
			vec.y /= mag;
		}
		return vec;
	}

};

class wall {

public:
	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Color col;

	void draw(sf::RenderWindow* window) {
		sf::Vertex line[] =
		{
			sf::Vertex(p1),
			sf::Vertex(p2)
		};
		window->draw(line, 2, sf::Lines);
	}

};