#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <chrono>
#include <list>
#include "GameObject.h"
#include "EditorMouse.h"
#include "HelperFuncs.h"
#include "OpenFileExplorer.h"
#include "Components.h"
#include <thread>
#include <cmath>
#include "SaveLoadScene.h"
#include "GameLoop.h"

//ImGui Libraries
#include "imgui.h"
#include "imgui-SFML.h"


using namespace std;


//Global Variables
vector<string> objectListStrings; //Used for Editor UI
void ObjectListUpdate(vector<GameObject>& worldObjects);



int main() {
  //Setup
    sf::RenderWindow window(sf::VideoMode(2000, 1200), "Game");
    window.setFramerateLimit(144);

    srand(time(NULL)); //Set random seed
    
    /*
    sf::Texture testex;
    testex.loadFromFile("ball.png");
    texObject mytester;
    mytester.myFileName = "ball.png";
    mytester.texture = testex;
    worldGameTex.push_back(mytester);

    sf::Texture testex2;
    testex2.loadFromFile("MetalGround.png");
    texObject mytester2;
    mytester2.myFileName = "MetalGround.png";
    mytester2.texture = testex2;
    worldGameTex.push_back(mytester2);

    sf::Texture testex3;
    testex3.loadFromFile("DefaultSprite.png");
    texObject mytester3;
    mytester3.myFileName = "DefaultSprite.png";
    mytester3.texture = testex3;
    worldGameTex.push_back(mytester3);

    sf::Texture testex4;
    testex4.loadFromFile("skycolor.png");
    texObject mytester4;
    mytester4.myFileName = "skycolor.png";
    mytester4.texture = testex4;
    worldGameTex.push_back(mytester4);
    */
   
    //All World Objects
    vector <GameObject> WorldObjects;
    WorldObjects.reserve(25000); //Set this to a little more than what you think your game will use at max
    worldGameTex.reserve(1000); //Set this to a little more than what you think your game will use at max
    
    bool hasRanStart = false;

    vector <GameObject> WorldParticles;
    WorldParticles.reserve(25000);

    list <GameObject> ListdWorldObjects;

        //Camera Offset (if the camera moves)
    sf::Vector2f camOffset = sf::Vector2f(0.0,0.0);
    Camera = window.getDefaultView();
    sf::Vector2f cameraOffset;
            //Any mouse position calculations need to take into account the camera offset
    Camera.setCenter(700,600);
    

        //Physics
    float gravity = -9.81;
            //Physics Timestep stuff
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;
    std::chrono::duration<int, std::milli> timeElapsed;

        //Editor
    bool editor = true;  //Change this to false when you want to build release
    GameObject mouseCollision("MouseCollisionSprite.png");
    bool holdingLeftMouse = false;
    bool objectSelected = false;
    bool gotMouseObjectOffset = false;
    int mouseLayer = 1;
    GameObject* hoveredObject = NULL;
    GameObject* selectedObject = NULL;
    float mouseOffsetx = 0;
    float mouseOffsety = 0;
    float selectionoffsetx = 0;
    float selectionoffsety = 0;
    sf::RectangleShape outline;
    sf::RectangleShape selCenter;
    float previewSpriteScale = 1;
    static int listbox_item_current = 0;
    int gridSize = 50;
    bool evntAlready = false;
    bool midMouse = false;
    sf::Vector2i midMouseInit = sf::Vector2i(0, 0);
    bool selectingNewImage = false;
    bool selectingNewComponent = false;
    string startingScene = "TestSave.scene";///////////////////////////////////////////////////////////////////////Change this to the starting scene FilePath
    LoadScene(startingScene, WorldObjects);
    currentScene = startingScene;
    bool saveAsWindow = false;
    string newFileName = "";
    bool playMode = false;

    float fps;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    
        //ImGUI
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    char buf[255]{};


  //Game object creation
    
   
    /*
    GameObject test(0);
    //GameObject test;
    //test.LoadNewTexture("ball.png");
    test.name = "PlayerBall";
    //test.hitBox = sf::FloatRect(test.Transform.getGlobalBounds().left * 0.5, test.Transform.getGlobalBounds().left * 0.5,,);
    test.usePhysics = true;
    test.collisions = true;
    test.Transform.setPosition(300,100);
    test.numInWorldObjects = 0;
    test.worldObjects = &WorldObjects;
    


    GameObject ground(1);
    ground.name = "Ground";
    ground.renderLayer = 1;
    ground.Transform.setScale(17,2);
    ground.Transform.setPosition(800, 1100);
    ground.collisions = true;
    ground.numInWorldObjects = 1;
    ground.worldObjects = &WorldObjects;

    GameObject ground2(1);
    ground2.name = "Ground2";
    //ground2.Transform.setColor(sf::Color(255,255,255,150));
    ground2.renderLayer = 1;
    ground2.Transform.setScale(17, 2);
    ground2.Transform.setPosition(1900, 700);
    ground2.collisions = true;
    ground2.numInWorldObjects = 2;
    ground2.worldObjects = &WorldObjects;


    GameObject backGround(3);
    backGround.name = "sky";
    backGround.renderLayer = 2;
    backGround.Transform.setScale(sf::Vector2f(10,10));
    backGround.physicsLayer = 1;
    backGround.numInWorldObjects = 3;
    backGround.worldObjects = &WorldObjects;

    GameObject spawner(2);
    spawner.name = "spawner";
    spawner.usePhysics = true;
    spawner.collisions = true;
    spawner.Transform.setPosition(500,500);
    spawner.worldObjects = &WorldObjects;
    spawner.numInWorldObjects = 4;



    WorldObjects.push_back(test);
    WorldObjects.push_back(ground);
    WorldObjects.push_back(ground2);
    WorldObjects.push_back(backGround);
    WorldObjects.push_back(spawner);

    
    //WorldObjects[0].AddComponent(new PlayerMoveTest(), &WorldObjects[0]);
    WorldObjects[0].AddComponent(CreateComponent("PlayerMoveTest"), &WorldObjects[0]);
    WorldObjects[0].AddComponent(new SimpleSpriteCollision(), & WorldObjects[0]);
    WorldObjects[0].AddComponent(new SimplePhysics(), &WorldObjects[0]);

    WorldObjects[0].AddComponent(new ChangeSpeed(), &WorldObjects[0]);

    WorldObjects[4].AddComponent(CreateComponent("PlayerMoveTest"), &WorldObjects[4]);
    WorldObjects[4].AddComponent(CreateComponent("SimpleSpriteCollision"), &WorldObjects[4]);
    WorldObjects[4].AddComponent(CreateComponent("SimplePhysics"), &WorldObjects[4]);
    WorldObjects[4].AddComponent(CreateComponent("ChangeSpeed"), &WorldObjects[4]);

    */


    //Large Size Testing
    /*
    for (size_t i = 0; i < 500; i++)
    {
        GameObject mytester;
        mytester.name = "wowsa";
        mytester.Transform.setPosition(rand() % 800 + 200, rand() % 800 + 200);
        WorldObjects.push_back(mytester);
    }
    */

    //WorldObjects[0].GetComponent("PlayerMoveTest").PlayerMoveTest::speed = 100;
    //WorldObjects[0].components.at(0).PlayerMoveTest::speed = 100;
    //PlayerMoveTest* hmmm = &WorldObjects[0].components[0];
    //cout << bruh.GetComponent("PlayerMoveTest")->compName << endl;;


    
    /////////////////

    Start();

    //Fonts
    sf::Font font;
    if (!font.loadFromFile("Sans.ttf"))
    {
        // error
    }
    sf::Text text;
    text.setPosition(window.getSize().x/2,window.getSize().y/2);
    text.setFont(font);
    text.setString("testing");
    text.setCharacterSize(75);
    text.setFillColor(sf::Color::White);



    ObjectListUpdate(WorldObjects);//initialize

    

    //Setup mouse collision detection sprite
    sf::Texture mousecoltex;
    mousecoltex.loadFromFile("Assets\\MouseCollisionSprite.png");
    mouseCollision.Transform.setTexture(mousecoltex);
    sf::IntRect newmouserect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(mousecoltex.getSize().x, mousecoltex.getSize().y)));
    mouseCollision.Transform.setTextureRect(newmouserect);
    mouseCollision.Transform.setOrigin((sf::Vector2f)mousecoltex.getSize() / 2.0f);
    mouseCollision.hitBox = mouseCollision.Transform.getGlobalBounds();


  // -Main loop-

    while (window.isOpen()) {
        // Clear the window
        window.clear();

        //Start function of all Components
        if (hasRanStart == false && playMode) {
            for (size_t i = 0; i < WorldObjects.size(); i++)
            {
                //cout << "starting all gameobject components++++++++++++++++++++++++++++++++++++" << endl;
                WorldObjects[i].ComponentsStart();
            }
            hasRanStart = true;
        }

        //Run all Component Updates:
        if (playMode) {
            for (size_t i = 0; i < WorldObjects.size(); i++)
            {
                WorldObjects[i].ComponentsUpdate();
            }
        }


        //Where is the camera
        //cout << "Cam Offset: " << Camera.getCenter().x << endl;
        cameraOffset = sf::Vector2f(Camera.getCenter().x, Camera.getCenter().y);

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }else if (event.type == sf::Event::MouseWheelScrolled) {  //Scroll wheel changing the layer
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    
                    if (event.mouseWheelScroll.delta > 0) {
                        // Scroll up, increase the value
                        mouseLayer++;
                        text.setString(to_string(mouseLayer));
                    }
                    else if (event.mouseWheelScroll.delta < 0) {
                        // Scroll down, decrease the value
                        mouseLayer--;
                        text.setString(to_string(mouseLayer));
                    }
                }
                break;
            }
        }
        //Update ImGUI
        ImGui::SFML::Update(window, deltaClock.restart());


        ///////FPS   From imanifacier
        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        //std::cout << "fps = " << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
        previousTime = currentTime;
        ////////////////////////////




        //Editor

        if (!sf::Mouse().isButtonPressed(sf::Mouse().Left)) { 
            holdingLeftMouse = false; 
            gotMouseObjectOffset = false;
        }else { 
            holdingLeftMouse = true; 
        }

        mouseCollision.Transform.setPosition(sf::Mouse::getPosition(window).x + cameraOffset.x - Camera.getSize().x/2, sf::Mouse::getPosition(window).y + cameraOffset.y - Camera.getSize().y/2);
        if (editor) {
            int tempobjectcounter = -1;
            if (objectSelected == false) {
                for (size_t i = 0; i < WorldObjects.size(); i++)
                {
                    if (mouseLayer == WorldObjects[i].renderLayer && mouseCollision.Transform.getGlobalBounds().intersects(WorldObjects[i].Transform.getGlobalBounds())) {
                        //cout << "Mouse is hovering over  " << WorldObjects[i].name << endl;
                        // We are hovering the mouse over something
                        hoveredObject = &WorldObjects[i];
                        tempobjectcounter = i;
                    }
                }
            }
            if (sf::Mouse().isButtonPressed(sf::Mouse().Left) && hoveredObject != NULL && objectSelected == false) {
                selectedObject = &WorldObjects[tempobjectcounter];
                selectingNewImage = false;
                selectingNewComponent = false;
                objectSelected = true;
            }
            if (selectedObject != NULL && sf::Mouse().isButtonPressed(sf::Mouse().Left) && mouseCollision.Transform.getGlobalBounds().intersects(selectedObject->Transform.getGlobalBounds())) {
                //Drag object
                //Set offset from mouse click location
                if (gotMouseObjectOffset == false) {
                    selectionoffsetx = mouseCollision.Transform.getPosition().x - selectedObject->Transform.getPosition().x;
                    selectionoffsety = mouseCollision.Transform.getPosition().y - selectedObject->Transform.getPosition().y;
                    gotMouseObjectOffset = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    selectedObject->Transform.setPosition(round((mouseCollision.Transform.getPosition().x + -selectionoffsetx)/ gridSize) * gridSize, round((mouseCollision.Transform.getPosition().y + -selectionoffsety) / gridSize) * gridSize);
                    selectedObject->yVelocity = 0;
                    selectedObject->xVelocity = 0;
                }else{
                selectedObject->Transform.setPosition(mouseCollision.Transform.getPosition().x + -selectionoffsetx, mouseCollision.Transform.getPosition().y + -selectionoffsety);
                selectedObject->yVelocity = 0;
                selectedObject->xVelocity = 0;
                }
            }
            if(selectedObject != NULL && sf::Mouse().isButtonPressed(sf::Mouse().Left) && !mouseCollision.Transform.getGlobalBounds().intersects(selectedObject->Transform.getGlobalBounds())){
                //If we click outside of the object, deselect it
                auto& io = ImGui::GetIO();
                if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
                    //We clicked on an imgui window, so don't delselect
                }
                else {
                    objectSelected = false;
                    selectedObject = NULL;
                    //listbox_item_current = 99999;
                }
            }

            //Reset hovered object pointer
            hoveredObject = NULL;
            tempobjectcounter = -1;


            if (selectedObject != NULL) {
                //Draw box around sprite
                outline.setPosition(selectedObject->Transform.getPosition().x - selectedObject->Transform.getGlobalBounds().width/2, selectedObject->Transform.getPosition().y - selectedObject->Transform.getGlobalBounds().height / 2);
                outline.setSize(sf::Vector2f((selectedObject->Transform.getGlobalBounds().width), (selectedObject->Transform.getGlobalBounds().height)));
                outline.setOutlineThickness(-4);
                outline.setFillColor(sf::Color::Transparent);
                selCenter.setPosition(selectedObject->Transform.getPosition().x, selectedObject->Transform.getPosition().y);
                selCenter.setSize(sf::Vector2f(3,3));
                selCenter.setFillColor(sf::Color::Red);

                //cout << "drawing outline" << endl;

                //Check if we are on the wrong layer, if so then deselect
                if (mouseLayer != selectedObject->renderLayer) {
                    objectSelected = false;
                    selectedObject = NULL;
                }
                
            }

            //Camera Pan with Middle Mouse button
            if (sf::Mouse().isButtonPressed(sf::Mouse().Middle)) {
                //cout << "Middle Mouse" << endl;
                sf::Vector2i midMouseOffset = sf::Vector2i(0, 0);
                if (midMouse == false) {
                    midMouseInit.x = mouseCollision.Transform.getPosition().x;
                    midMouseInit.y = mouseCollision.Transform.getPosition().y;
                    midMouse = true;
                }
                else {
                    midMouseOffset.x = midMouseInit.x - mouseCollision.Transform.getPosition().x;
                    midMouseOffset.y = midMouseInit.y - mouseCollision.Transform.getPosition().y;
                    Camera.setCenter(Camera.getCenter().x + midMouseOffset.x, Camera.getCenter().y + midMouseOffset.y);
                }
            }
            else { midMouse = false; }
            ////////////////////

            //Delete key and selected object
            if (selectedObject != NULL && sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
                WorldObjects.erase(WorldObjects.begin() + selectedObject->numInWorldObjects);// +listbox_item_current);
                // 
                //iter_swap(WorldObjects[selectedObject->numInWorldObjects], WorldObjects.end() - 1);
                //WorldObjects.pop_back();
                // 
                //remove(WorldObjects.begin(), WorldObjects.end(), &selectedObject);
                // 
                //remove(WorldObjects.begin() + selectedObject->numInWorldObjects, WorldObjects.begin() + selectedObject->numInWorldObjects, selectedObject);
                //selectedObject->Transform.setColor(sf::Color(0,0,0,0));

                //Fix objects member numbers
                for (size_t i = 0; i < WorldObjects.size(); i++)
                {
                    WorldObjects[i].numInWorldObjects = i;
                    //WorldObjects[i].LoadNewTexture(WorldObjects[i].myTexture);  //SUUUPER SLOW
                    //WorldObjects[i].texture = WorldObjects[i - 1].Transform.getTexture();
                }
                selectedObject = NULL;
                objectSelected = false;
                ObjectListUpdate(WorldObjects);
            }

            //Make sure mouseLayer doesn't go below 0
            if (mouseLayer < 0) {
                mouseLayer = 0;
            }


            //Saving scene hotkey, ctrl s
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    //Need to make it only work once instead of happening 5000 times
                    //SaveScene(WorldObjects, "SceneOne");
                }
            }

        }
        /////////////////////////

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            //WorldObjects[0].xAcceleration = 5;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //WorldObjects[0].Transform.move(2.5, 0);
           //WorldObjects[0].xAcceleration = -5;

        }
        else {
            //WorldObjects[0].xAcceleration = 0;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            //WorldObjects[0].Transform.move(0, -2.5);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            //WorldObjects[0].Transform.move(0, 2.5);
        }
        //JUMP
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            //WorldObjects[0].yVelocity = 100;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            //WorldObjects[0].Transform.setScale(WorldObjects[0].Transform.getScale().x + 0.01, WorldObjects[0].Transform.getScale().y + 0.01);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
            //WorldObjects[0].Transform.setScale(WorldObjects[0].Transform.getScale().x - 0.01, WorldObjects[0].Transform.getScale().y - 0.01);
        }


        //CAMERA MOVEMENT TEST
        /*
        float cameraSpeed = 5.0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            //camOffset = sf::Vector2f(camOffset.x - cameraSpeed, camOffset.y);
            Camera.move(cameraSpeed,0);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            //camOffset = sf::Vector2f(camOffset.x + cameraSpeed, camOffset.y);
            Camera.move(-cameraSpeed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            //camOffset = sf::Vector2f(camOffset.x, camOffset.y - cameraSpeed);
            Camera.move(0, cameraSpeed);

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
            //camOffset = sf::Vector2f(camOffset.x, camOffset.y + cameraSpeed);
            Camera.move(0, -cameraSpeed);
        }
        */



        //Update Game Loop
        Update();



        /*
        //Physics Updates
        //Just doing falling objects right now
        int physlayers = 2;
        float deltaTime = 0.16666; //Timestep, set to 60 fps.  144fps is 0.0694444
        end = std::chrono::steady_clock::now(); // Get current time
       // Calculate time elapsed since start
        timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (timeElapsed >= std::chrono::milliseconds(1000 / 60)) { //Check if it's time to update physics //60fps
            for (size_t pl = 0; pl < physlayers; pl++)
            {
                for (size_t i = 0; i < WorldObjects.size(); i++)
                {
                    //Save last position in case of collisions
                    sf::Vector2f lastPos = WorldObjects[i].Transform.getPosition();

                    if (WorldObjects[i].usePhysics == true) {       

                        /*
                        WorldObjects[i].yAcceleration = WorldObjects[i].gravity - (0.5 * 1.2 * WorldObjects[i].yVelocity * WorldObjects[i].yVelocity * WorldObjects[i].dragCo * 0.2) / WorldObjects[i].mass;
                        WorldObjects[i].yVelocity += WorldObjects[i].yAcceleration * deltaTime;
                        WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x, WorldObjects[i].Transform.getPosition().y * WorldObjects[i].yVelocity * deltaTime);// += velocity * dt;
                        




                        //Make object fall
                        
                        float currentYpos = WorldObjects[i].Transform.getPosition().y;
                        WorldObjects[i].yVelocity += WorldObjects[i].yAcceleration * deltaTime;// *WorldObjects[i].dragCo; //Updates Velocity, including drag co: might not be working
                        WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x, currentYpos -= WorldObjects[i].yVelocity * deltaTime); //Updates position

                        float currentXpos = WorldObjects[i].Transform.getPosition().x;
                        WorldObjects[i].xVelocity += WorldObjects[i].xAcceleration * deltaTime;// *WorldObjects[i].dragCo; //Updates Velocity, including drag co: might not be working
                        WorldObjects[i].Transform.setPosition(currentXpos -= WorldObjects[i].xVelocity * deltaTime, WorldObjects[i].Transform.getPosition().y); //Updates position
                        

                        /*
                        float dragForceX = -0.5f * WorldObjects[i].dragCo * WorldObjects[i].xVelocity * std::abs(WorldObjects[i].xVelocity);
                        float dragForceY = -0.5f * WorldObjects[i].dragCo * WorldObjects[i].yVelocity * std::abs(WorldObjects[i].yVelocity);

                        WorldObjects[i].xAcceleration = dragForceX;
                        WorldObjects[i].yAcceleration = WorldObjects[i].yAcceleration - dragForceY;

                        // Update velocity
                        WorldObjects[i].xVelocity += WorldObjects[i].xAcceleration * deltaTime;
                        WorldObjects[i].yVelocity += WorldObjects[i].yAcceleration * deltaTime;

                        // Update position
                        WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x + WorldObjects[i].xVelocity * deltaTime, WorldObjects[i].Transform.getPosition().y);//x += velocityX * deltaTime;
                        WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x, WorldObjects[i].Transform.getPosition().y + WorldObjects[i].yVelocity * deltaTime);//y += velocityY * deltaTime;
                                                

                        WorldObjects[i].xVelocity *= WorldObjects[i].dragCo;
                    }

                    //Check all collision enabled objects on the same layer, see if the collision boxes are overlapping
                    if (WorldObjects[i].collisions == true) {
                        //check collision

                        //loop through every other object
                        for (size_t k = 0; k < WorldObjects.size(); k++)
                        {
                            
                            if (WorldObjects[k].collisions == true && WorldObjects[i].name != WorldObjects[k].name && WorldObjects[k].physicsLayer == WorldObjects[i].physicsLayer) {
                                //on the same layer, check for collision
                               
                                if (WorldObjects[i].Transform.getGlobalBounds().intersects(WorldObjects[k].Transform.getGlobalBounds())) {
                                    //We collided, now determine which side collided

                                    //Attempt at directional collision
                                    
                                    float deltaX = WorldObjects[i].Transform.getTextureRect().left - WorldObjects[k].Transform.getTextureRect().left;
                                    float deltaY = WorldObjects[i].Transform.getTextureRect().top - WorldObjects[k].Transform.getTextureRect().top;

                                    float intersectX = WorldObjects[i].Transform.getTextureRect().width + WorldObjects[k].Transform.getTextureRect().width - std::abs(deltaX);
                                    float intersectY = WorldObjects[i].Transform.getTextureRect().height + WorldObjects[k].Transform.getTextureRect().height - std::abs(deltaY);

                                    if (intersectX < intersectY) {
                                        if (deltaX > 0) {
                                            //Right
                                            WorldObjects[i].xVelocity = -WorldObjects[i].xVelocity * WorldObjects[i].bounciness;
                                            cout << "collision RIGHT" << endl;
                                        }
                                        else
                                            //Left
                                            WorldObjects[i].xVelocity = -WorldObjects[i].xVelocity * WorldObjects[i].bounciness;
                                        cout << "collision Left" << endl;
                                    }
                                    else {
                                        if (deltaY < 0) {
                                            //Bottom
                                            WorldObjects[i].yVelocity = -WorldObjects[i].yVelocity * WorldObjects[i].bounciness;
                                            cout << "collision bottom" << endl;
                                        }
                                        else
                                            //Top
                                            WorldObjects[i].yVelocity = -WorldObjects[i].yVelocity * WorldObjects[i].bounciness;
                                       // cout << "collision top" << endl;
                                    }
                                    WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x, lastPos.y);

                                    
                                    
                                    //Check bottom of 
                                    ////////////

                                    /*
                                    if (WorldObjects[i].Transform.getGlobalBounds().top - WorldObjects[i].Transform.getGlobalBounds().height >= WorldObjects[k].Transform.getGlobalBounds().top) {
                                        // we collided on the top
                                        cout << WorldObjects[i].name << "s bottom collided with " << WorldObjects[k].name << "s top" << endl;
                                    }
                                    //cout << WorldObjects[i].name << " at " << WorldObjects[i].Transform.getPosition().y << " collided with " << WorldObjects[k].name << " at " << WorldObjects[k].Transform.getPosition().y << endl;
                                    WorldObjects[i].yVelocity = -WorldObjects[i].yVelocity * WorldObjects[i].bounciness;
                                    //WorldObjects[i].xVelocity = -WorldObjects[i].xVelocity * WorldObjects[i].bounciness;
                                    //WorldObjects[i].yVelocity = 0;
                                    WorldObjects[i].Transform.setPosition(WorldObjects[i].Transform.getPosition().x, lastPos.y);
                                    

                                    
                                }
                            }
                        }
                    }
                }
            }
            start = std::chrono::steady_clock::now();

        }

        */
        //ImGui Editor Windows/////////////
        if (editor) {

            //ToolBar

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save"))
                    {
                        string newFileName = "Assets\\" + currentScene;
                        SaveScene(WorldObjects, newFileName);
                        cout << newFileName << endl;
                    }

                    if (ImGui::MenuItem("Save As")) {
                        
                        saveAsWindow = true;
                    }

                    if (ImGui::MenuItem("Load"))
                    {
                        //TODO: Ask if you want to save before loading
                    //for now, clear vector
                        WorldObjects.clear();
                        string theFile = OpenFileExplorer();
                        LoadScene(theFile, WorldObjects);

                        //Loop through all new objects and set them up.
                        //Only have to do this in main because of stupid dereferencing which should even be happening
                        for (size_t i = 0; i < WorldObjects.size(); i++)
                        {
                            WorldObjects[i].Setup();
                            WorldObjects[i].worldObjects = &WorldObjects;
                        }
                        ObjectListUpdate(WorldObjects);
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("GameObject"))
                {
                    if (ImGui::MenuItem("Create New"))
                    {
                        GameObject mynewobj("DefaultSprite.png");
                        mynewobj.name = "New Object";
                        mynewobj.renderLayer = mouseLayer;
                        mynewobj.myTexture = "DefaultSprite.png";
                        mynewobj.numInWorldObjects = WorldObjects.size();
                        mynewobj.position = Camera.getCenter();
                        mynewobj.Transform.setPosition(Camera.getCenter());
                        mynewobj.worldObjects = &WorldObjects;
                        WorldObjects.push_back(mynewobj);
                        WorldObjects[mynewobj.numInWorldObjects].Setup();
                        ObjectListUpdate(WorldObjects);
                        selectedObject = &WorldObjects.back();
                        objectSelected = true;
                    }
                    ImGui::EndMenu();
                }

                if (saveAsWindow) {
                    ImGui::Begin("-Save Scene As-");
                    ImGui::Text("File Name:");
                    
                    static char filenameInputText[256] = ""; // Buffer to hold the input text
                    if (ImGui::InputText("##FileName", filenameInputText, IM_ARRAYSIZE(filenameInputText))) {
                        newFileName = filenameInputText;
                    }
                    string filenamefixed = "Assets\\" + newFileName + ".scene";
                    if (ImGui::Button("Save")) {
                        SaveScene(WorldObjects, filenamefixed);
                        saveAsWindow = false;
                    }
                    if (ImGui::Button("Close")) {
                        saveAsWindow = false;
                    }
                    
                    ImGui::End();
                }

                /*
                if (ImGui::Button("Save Scene Temp")) {
                    SaveScene(WorldObjects, "TestSave");
                }
                ImGui::SameLine();
                if (ImGui::Button("Load Scene Temp")) {
                    //TODO: Ask if you want to save before loading
                    //for now, clear vector
                    WorldObjects.clear();
                    LoadScene("TestSave", WorldObjects);
                    //Loop through all new objects and set them up.
                    //Only have to do this in main because of stupid dereferencing which should even be happening
                    for (size_t i = 0; i < WorldObjects.size(); i++)
                    {
                        WorldObjects[i].Setup();
                        WorldObjects[i].worldObjects = &WorldObjects;
                    }
                    ObjectListUpdate(WorldObjects);

                }
                */
                ImGui::SameLine();
                ImGui::Text("                                              ");
                ImGui::SameLine();
                if (playMode == false) {
                    if (ImGui::Button("Play")) {
                        string newFileName = "Assets\\" + currentScene;
                        SaveScene(WorldObjects, newFileName);
                        cout << newFileName << endl;
                        playMode = true;
                    }
                }
                else {
                    if (ImGui::Button("Stop")) {
                        WorldObjects.clear();
                        string fixedFileName = "Assets\\" + currentScene;
                        LoadScene(fixedFileName, WorldObjects);
                        //Loop through all new objects and set them up.
                        //Only have to do this in main because of stupid dereferencing which should even be happening
                        for (size_t i = 0; i < WorldObjects.size(); i++)
                        {
                            WorldObjects[i].Setup();
                            WorldObjects[i].worldObjects = &WorldObjects;
                        }
                        ObjectListUpdate(WorldObjects);
                        playMode = false;
                        hasRanStart = false;
                    }
                }
                ImGui::SameLine();
                //Level Name
                string levName = "                                 " + currentScene + " | ";
                ImGui::Text(levName.c_str());
                //ImGui::Separator();

                //Layer
                ImGui::SameLine();
                ImGui::Text("Selected Layer: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(75);
                ImGui::InputInt(" | ", &mouseLayer, 1, 0);
                ImGui::PopItemWidth();

                //Grid Size
                ImGui::SameLine();
                ImGui::Text("Grid Size: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(75);
                ImGui::InputInt("| ", &gridSize, 0, 0);
                ImGui::PopItemWidth();

                //FPS counter
                ImGui::SameLine();
                ImGui::PushItemWidth(75);
                string fpsCounter = "FPS: " + to_string((int)fps) + "";
                ImGui::Text(fpsCounter.c_str());
                ImGui::PopItemWidth();

                ImGui::PopStyleColor();

                ImGui::EndMainMenuBar();
            }

           //ToolBar ooooolllldddd
            /*
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 35));
            ImGui::Begin("   ", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);
            ImGui::Button("File");
            ImGui::SameLine();
            ImGui::Button("GameObject");
            ImGui::SameLine();
            if (ImGui::Button("Save Scene Temp")) {
                SaveScene(WorldObjects,"TestSave");
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Scene Temp")) {
                //TODO: Ask if you want to save before loading
                //for now, clear vector
                WorldObjects.clear();
                LoadScene("TestSave", WorldObjects);
                //Loop through all new objects and set them up.
                //Only have to do this in main because of stupid dereferencing which should even be happening
                for (size_t i = 0; i < WorldObjects.size(); i++)
                {
                    WorldObjects[i].Setup();
                    WorldObjects[i].worldObjects = &WorldObjects;
                }
                ObjectListUpdate(WorldObjects);

            }
            ImGui::SameLine();
            //Level Name
            ImGui::Text("                                       Scene One | ");
            //ImGui::Separator();

            //Layer
            ImGui::SameLine();
            ImGui::Text("Selected Layer: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(75);
            ImGui::InputInt(" | ", &mouseLayer, 1, 0);
            ImGui::PopItemWidth();

            //Grid Size
            ImGui::SameLine();
            ImGui::Text("Grid Size: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(75);
            ImGui::InputInt("| ", &gridSize, 0, 0);
            ImGui::PopItemWidth();

            //FPS counter
            ImGui::SameLine();
            ImGui::PushItemWidth(75);
            string fpsCounter = "FPS: " + to_string((int)fps) + "";
            ImGui::Text(fpsCounter.c_str());
            ImGui::PopItemWidth();

            ImGui::PopStyleColor();
            ImGui::End();
            */



            //Inspector
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.25f, 0.9f));
            ImGui::Begin("Inspector");
            if (selectedObject != NULL) { //If selected object exists

                static char nameInputText[256] = ""; // Buffer to hold the input text
                strcpy_s(nameInputText, selectedObject->name.c_str());
                if (ImGui::InputText("##Name", nameInputText, IM_ARRAYSIZE(nameInputText))) {
                    selectedObject->name = nameInputText;
                }
                //ImGui::Text(selectedObject->name.c_str());
                ImGui::Separator();
                //Transform Position////////////////////
                ImGui::Text("Position");
                ImGui::Text("X:");
                ImGui::SameLine();
                //Do some wierd stuff to get and change selected object pos
                float selectedxpos = selectedObject->Transform.getPosition().x;
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("|", &selectedxpos, 0, 0, "%.0f");
                ImGui::PopItemWidth();
                selectedObject->Transform.setPosition(selectedxpos, selectedObject->Transform.getPosition().y);
                ImGui::SameLine();
                ImGui::Text("Y:");
                ImGui::SameLine();
                float selectedypos = selectedObject->Transform.getPosition().y;
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("", &selectedypos, 0, 0, "%.0f");
                ImGui::PopItemWidth();
                selectedObject->Transform.setPosition(selectedObject->Transform.getPosition().x, selectedypos);
                //Transform Scale/////////////////
                ImGui::Text("Scale");
                //ImGui::SameLine();
                //Do some wierd stuff to get and change selected object scale
                ImGui::Text("X: ");
                ImGui::SameLine();
                float selectedxscale = selectedObject->Transform.getScale().x;
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("    ", &selectedxscale, 0, 0, "%.2f");
                ImGui::PopItemWidth();
                selectedObject->Transform.setScale(selectedxscale, selectedObject->Transform.getScale().y);
                ImGui::SameLine();
                ImGui::Text("Y: ");
                ImGui::SameLine();
                float selectedyscale = selectedObject->Transform.getScale().y;
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("      ", &selectedyscale, 0, 0, "%.2f");
                ImGui::PopItemWidth();
                selectedObject->Transform.setScale(selectedObject->Transform.getScale().x, selectedyscale);
                //Transform Rotation/////////////////
                ImGui::Text("Rotation");
                ImGui::SameLine();
                //Do some wierd stuff to get and change selected object rotation
                float selectedxrot = selectedObject->Transform.getRotation();
                ImGui::PushItemWidth(50);
                ImGui::InputFloat(" ", &selectedxrot, 0, 0, "%.2f");
                ImGui::PopItemWidth();
                selectedObject->Transform.setRotation(selectedxrot);
                //Physics Options
                ImGui::Checkbox("Collisions", &selectedObject->collisions);
                ImGui::PushItemWidth(100);
                ImGui::InputInt("Physics Layer", &selectedObject->physicsLayer);
                ImGui::PopItemWidth();
                ImGui::Checkbox("Physics", &selectedObject->usePhysics);
                if (selectedObject->usePhysics == true) {
                    ImGui::Separator();
                    ImGui::PushItemWidth(100);
                    ImGui::InputFloat("Bounciness", &selectedObject->bounciness, 0.05, 0.3, "%.2f");
                    ImGui::PopItemWidth();
                    ImGui::PushItemWidth(100);
                    ImGui::InputFloat("Drag Co.", &selectedObject->dragCo, 0.1, 0.5, "%.2f");
                    ImGui::PopItemWidth();
                    ImGui::PushItemWidth(100);
                    ImGui::InputFloat("Gravity", &selectedObject->yAcceleration, 0, 0, "%.2f");
                    ImGui::PopItemWidth();

                }
                /*
        float yVelocity = 0;
        float yAcceleration = -9.81;
        float xVelocity = 0;
        float xAcceleration = 0;
        float bounciness = 0.8;
        float dragCo = 0.98;*/


        

        //Texture/////////////////Render Layer
        // sf::Texture objectsTex = selectedObject->Transform.getTexture();
                ImGui::Separator();
                ImGui::PushItemWidth(100);
                if (ImGui::InputInt("Render Layer", &selectedObject->renderLayer, 1, 5)) { mouseLayer = selectedObject->renderLayer; }
                ImGui::PopItemWidth();
                ImGui::PushItemWidth(50);
                ImGui::InputFloat("Preview Scale", &previewSpriteScale, 0, 0, "%.2f");
                ImGui::PopItemWidth();
                ImGui::Text(selectedObject->myTexture.c_str());
                ImGui::Separator();
                sf::Sprite texSprite = selectedObject->Transform;
                texSprite.setScale(previewSpriteScale, previewSpriteScale);
                //texSprite.setTexture(selectedObject->Transform.getTexture());
                //ImGui::Image(texSprite);
                if (ImGui::Button("Select Image")) {
                    selectingNewImage = true;
                }

                //Select Image Window
                if (selectingNewImage) {
                    int newimageindex = 0;
                    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.25f, 0.9f));
                    ImGui::Begin("Select Texture");
                    for (size_t i = 0; i < worldGameTex.size(); i++)
                    {
                        ImGui::PushID(i);
                        if (ImGui::ImageButton("Textures List", worldGameTex[i].texture, sf::Vector2f(50, 50),sf::Color(0,0,0,0))) {
                            selectedObject->SetObjectTexture(i);
                            selectingNewImage = !selectingNewImage;
                        }
                        ImGui::PopID();
                    }
                    ImGui::End();
                    //
                }

                //ImGui::Image(texSprite);
                if (ImGui::ImageButton("ObjectTex", texSprite, sf::Vector2f(90, 90), sf::Color(0, 0, 0, 0))) {
                    //selectedObject->SetObjectTexture(i);
                    //selectingNewImage = false;
                }
                if (ImGui::Button("Load New Image")) {

                    string searched = OpenFileExplorer();
                    reverse(searched.begin(), searched.end());
                    string s = searched;
                    string delimiter = "\\";
                    string newName = s.substr(0, s.find(delimiter));
                    reverse(newName.begin(), newName.end());
                    cout << newName << endl;
                    string finalName = "Assets\\" + newName;

                    selectedObject->LoadNewTexture(finalName);
                }
                ImGui::Text("               ");
                ImGui::Separator();
                
                //Components
                // Need a way to delete them...
                //ImGui::Text("Components:");
                if (selectedObject != NULL) {
                    for (size_t i = 0; i < selectedObject->components.size(); i++)
                    {
                        
                        ImGui::Text(selectedObject->components[i]->returnName().c_str());
                        ImGui::SameLine();
                        ImGui::PushID(i);
                        if (ImGui::Button("X")) {
                            //Remove Component
                            selectedObject->components.erase(selectedObject->components.begin() + i);
                        }
                        ImGui::PopID();
                        ImGui::Indent(16.0f);
                        ImGui::Separator();
                        selectedObject->components[i]->EditorUI();
                        ImGui::Indent(-16.0f);
                        
                    }
                }
                if(ImGui::Button("Add Component")) {
                    selectingNewComponent = !selectingNewComponent;
                }

                //Add component Window
                if (selectingNewComponent) {
                    ImGui::Begin("Components");
                    map<string, int>::iterator it;
                    for (auto const& x : classFactory)
                    {
                        if (ImGui::Button(x.first.c_str())) {
                            selectedObject->AddComponent(CreateComponent(x.first.c_str()), &WorldObjects[selectedObject->numInWorldObjects]);
                            selectingNewComponent = false;
                        }
                    }

                    ImGui::End();
                }


                //ImGui::Text(selectedObject->name.c_str());
                //ImGui::Text(selectedObject->name.c_str());
            }
            ImGui::PopStyleColor();
            ImGui::End();


            //Object List
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.25f, 0.9f));
            ImGui::Begin("Object List");
            if (ImGui::Button("Create new Object")) {
                GameObject mynewobj(0);
                mynewobj.name = "New Object";
                //mynewobj.textureIndex = 0;
                mynewobj.renderLayer = mouseLayer;
                mynewobj.myTexture = "DefaultSprite.png";
                mynewobj.numInWorldObjects = WorldObjects.size();
                mynewobj.position = Camera.getCenter();
                mynewobj.Transform.setPosition(Camera.getCenter());
                mynewobj.worldObjects = &WorldObjects;
                WorldObjects.push_back(mynewobj);
                WorldObjects[mynewobj.numInWorldObjects].Setup();
                ObjectListUpdate(WorldObjects);
                selectedObject = &WorldObjects.back();
                objectSelected = true;
            }
            ImGui::Separator();
            vector<string> items = objectListStrings;
            
            if (selectedObject != NULL) {
                listbox_item_current = selectedObject->numInWorldObjects;
            }
            else {
                listbox_item_current = -5;
            }
            
            if (ImGui::ListBox(" ", &listbox_item_current,
                    [](void* data, int idx, const char** out_text) {
                        std::vector<std::string>* items = (std::vector<std::string>*)data;
                        if (idx < 0 || idx >= items->size()) return false;
                        *out_text = (*items)[idx].c_str();
                        return true;
                    }, (void*)&items, items.size(), 25)      ) 
            {
                selectedObject = &WorldObjects[listbox_item_current];
                objectSelected = true;
                mouseLayer = selectedObject->renderLayer;
            }
            //cout << listbox_item_current << endl;
            ImGui::PopStyleColor();
            ImGui::End();
           //SelectionLogic //Broken
            //selectedObject = &WorldObjects[listbox_item_current];
            //objectSelected = true;

            //////////////////////////////



            //Texture List
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.25f, 0.9f));
            ImGui::Begin("Texture List");
            //Add new texture button
            if (ImGui::Button("Add New Texture")) {

                string searched = OpenFileExplorer();
                reverse(searched.begin(), searched.end());
                string s = searched;
                string delimiter = "\\";
                string newName = s.substr(0, s.find(delimiter));
                reverse(newName.begin(), newName.end());
                cout << newName << endl;
                string finalName = "Assets\\" + newName;
                CreateTexture(finalName);
            }
            for (size_t i = 0; i < worldGameTex.size(); i++)
            {
                ImGui::PushID(i);
                
                if (ImGui::ImageButton("Textures List", worldGameTex[i].texture, sf::Vector2f(50, 50), sf::Color(0, 0, 0, 0))) {
                }
                ImGui::PopID();
                ImGui::SameLine();
            }

            ImGui::End();
        }



        
      //Render Objects
        window.setView(Camera);


        //Draw Everything Here

        //Change this number for how many layers you will have. (0 is a render layer)
        //This will render the sprites by layer, then in order from earliest in the vector to the latest
        int layerCount = 3;

        for (size_t l = layerCount; l-- > 0;)
        {
            for (size_t i = 0; i < WorldObjects.size(); i++)
            {
                //cout << "Layercount = " << l << "      i = " << i << endl;
                if (WorldObjects[i].renderLayer == l) {
                    //Must render object with the camera position in mind.
                    //GameObject renderingObject = WorldObjects[i];
                    //renderingObject.Transform.move(camOffset);
                    window.draw(WorldObjects[i].Transform);
                }

            }
        }

        //Draw Mouse position (Debug)
        //window.draw(mouseCollision.Transform);
        


        //window.draw(test.Transform);
        //window.draw(ground.Transform);

        /*
        // Draw the sprite

        for (size_t i = 0; i < spritevec.size(); i++)
        {
            //Move sprites
            spritevec[i].move();
            window.draw(spritevec[i].sprite);

            //check collision with window
            spritevec[i].windowCol(1600,1200);
        }
        */

        //If in editor
        if (editor) {

            //Draw grid if holding Lctr///////////
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                int windowWidth = window.getSize().x + 100;
                int windowHeight = window.getSize().y + 100;
                sf::Vector2f viewCenter = Camera.getCenter();
                float xOffset = std::fmod(viewCenter.x, gridSize);
                float yOffset = std::fmod(viewCenter.y, gridSize);
                sf::Vector2f gridOffset(-xOffset - gridSize / 2.0f, -yOffset - gridSize / 2.0f);

                // Draw the grid
                sf::Color gridColor(0, 0, 0, 50); // Adjust alpha value to control transparency
                for (float x = viewCenter.x - windowWidth / 2.0f; x < viewCenter.x + windowWidth / 2.0f; x += gridSize) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(x, viewCenter.y - windowHeight / 2.0f) + gridOffset, gridColor),
                        sf::Vertex(sf::Vector2f(x, viewCenter.y + windowHeight / 2.0f) + gridOffset, gridColor)
                    };
                    window.draw(line, 2, sf::Lines);
                }
                for (float y = viewCenter.y - windowHeight / 2.0f; y < viewCenter.y + windowHeight / 2.0f; y += gridSize) {
                    sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(viewCenter.x - windowWidth / 2.0f, y) + gridOffset, gridColor),
                        sf::Vertex(sf::Vector2f(viewCenter.x + windowWidth / 2.0f, y) + gridOffset, gridColor)
                    };
                    window.draw(line, 2, sf::Lines);
                }

                //Draw text that shows mouse co-ords
                sf::Text mcoords;
                mcoords.setPosition(mouseCollision.Transform.getPosition().x + 17, mouseCollision.Transform.getPosition().y + 17);
                mcoords.setFont(font);
                string mousecoordsstring = "X: " + to_string(static_cast<int>(mouseCollision.Transform.getPosition().x)) + "  Y:" + to_string(static_cast<int>(mouseCollision.Transform.getPosition().y));
                mcoords.setString(mousecoordsstring);
                mcoords.setCharacterSize(15);
                mcoords.setFillColor(sf::Color::White);
                window.draw(mcoords);

            }
            ///////////////////////////////


            //window.draw(text);

        }

        //editor UI
        if (selectedObject != NULL) {
            window.draw(outline);
            window.draw(selCenter);
        }

        //Display ImGui last
        ImGui::SFML::Render(window);

        // Display the window contents
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;

}


void ObjectListUpdate(vector<GameObject>& worldObjects) {
    objectListStrings.clear();
    for (size_t i = 0; i < worldObjects.size(); i++)
    {
        objectListStrings.push_back(worldObjects[i].name);
    }
}