#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include "Scene.h"
#include "TriangleScene.h"
#include "GradiantTriangleScene.h"
#include "SquareScene.h"
#include "SquareTextureScene.h"
#include "Settings.h"
#include "MapScene.h"
#include "Minimap.h"


int main()
{  

    sf::Clock clock = sf::Clock();
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(Settings::GetScreenWidth(), Settings::GetScreenHeight()), "Tryout 3D", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
    //window.setMouseCursorGrabbed(true);

    glewExperimental = GL_TRUE;
    glewInit();
    
    Scene* scene = new MapScene();
   
    Camera cam = Camera(glm::vec3(0, 5, 0), glm::vec3(0, 1, 0), 0,0);
    cam.MouseSensitivity = 0.01f;
    float mouseXdt = 0;
    float mouseYdt = 0;
    float dt = 0;
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds()*10;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {

                window.setActive();
                sf::Vector2f size = sf::Vector2f(event.size.width, event.size.height);
                window.setView(sf::View(sf::FloatRect(0,0,size.x,size.y)));
                glViewport(0, 0, size.x, size.y);
                window.setActive(false);

                Settings::SetScreenWidth(size.x);
                Settings::SetScreenHeight(size.y);
            }
            else if (event.type == sf::Event::MouseMoved) {
                mouseXdt = event.mouseMove.x < 70 ? -1 : event.mouseMove.x > Settings::GetScreenWidth() - 70 ? 1 : 0;
                mouseYdt = event.mouseMove.y < 70 ? -1 : event.mouseMove.y > Settings::GetScreenHeight() - 70 ? 1 : 0;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
            cam.ProcessKeyboard(Camera_Movement::FORWARD, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            cam.ProcessKeyboard(Camera_Movement::LEFT, dt );
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            cam.ProcessKeyboard(Camera_Movement::BACKWARD, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            cam.ProcessKeyboard(Camera_Movement::RIGHT, dt);
        }
        cam.ProcessMouseMovement(mouseXdt*6, -mouseYdt*6, true);
        
        glClearColor(0.1f, 0.2f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->Update(dt, &cam);
        scene->render();       
        
        window.display();        
    }

    delete scene;
    window.close();

    return 0;
}