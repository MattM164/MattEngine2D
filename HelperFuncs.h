#pragma once
#include <vector>
#include <string>
#include <cstring> // for strcpy and strcat
#include <iostream>
#include "GameObject.h"

const char vectorOfCustomClassToCharArray(const std::vector<GameObject>& customObjects) {
    // Calculate total length of the concatenated string
    size_t totalLength = 0;
    for (const GameObject& obj : customObjects) {
       totalLength += obj.name.length();
    }

    // Allocate memory for the char array
    char* charArray = new char[totalLength + 1]; // +1 for null terminator

    // Copy strings into char array
    size_t currentPosition = 0;
    for (const GameObject& obj : customObjects) {
        strcpy_s(charArray + currentPosition, totalLength - currentPosition + 1, obj.name.c_str());
        currentPosition += obj.name.length();
    }

    // Null-terminate the char array
    charArray[totalLength] = '\0';

    return *charArray;
}
