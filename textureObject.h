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
#include <string>

using namespace std;

class texObject {

public:
	string myFileName = "";
	sf::Texture texture;
};