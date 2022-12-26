#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/audio.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// Font and text for the menu title
sf::Font font;
Text titleText;

// Buttons for menu options
RectangleShape playButton;
Text playText;

RectangleShape quitButton;
Text quitText;

// Variables for button hover animation
Color originalColor;


void MenuScene::Load() {
    cout << "Menu Load \n";

// Load font from file
    font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");

// Set up title text
    titleText.setFont(font);
    titleText.setOutlineColor(sf::Color::Green);
    titleText.setOutlineThickness(4);
    titleText.setString("Doodle Jump");
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::Black);
    titleText.setPosition(Vector2f(450, 50));

// Set up play button
    playButton.setSize(Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Transparent);
    playButton.setOutlineThickness(4);
    playButton.setOutlineColor(Color::Black);
    playButton.setPosition(Vector2f(550, 250));


    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(32);
    playText.setFillColor(Color::Black);
    playText.setPosition(Vector2f(610, 255));
    playText.setOutlineColor(sf::Color::Green);
    playText.setOutlineThickness(4);

// Set up quit button
    quitButton.setSize(Vector2f(200, 50));
    quitButton.setFillColor(Color::Transparent);
    quitButton.setOutlineThickness(4);
    quitButton.setOutlineColor(Color::Black);
    quitButton.setPosition(Vector2f(550, 350));

    quitText.setFont(font);
    quitText.setString("quit");
    quitText.setCharacterSize(32);
    quitText.setFillColor(Color::Black);
    quitText.setPosition(Vector2f(610, 355));
    quitText.setOutlineColor(sf::Color::Green);
    quitText.setOutlineThickness(4);

    setLoaded(true);
    audio::playAudio("res/music/music.wav");
}

void MenuScene::Update(const double& dt) {
// Check if the play button is hovered over
    if (playButton.getGlobalBounds().contains(Vector2f (static_cast<float>(Mouse::getPosition(Engine::GetWindow()).x), static_cast<float>(Mouse::getPosition(Engine::GetWindow()).y)))) {

        playButton.setFillColor(sf::Color::Green);

        // Check if the play button is clicked
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Engine::ChangeScene(&level3);
        }
    }
    else {
        playButton.setFillColor(originalColor);
    }


// Check if the quit button is hovered over
    if (quitButton.getGlobalBounds().contains(Vector2f (static_cast<float>(Mouse::getPosition(Engine::GetWindow()).x), static_cast<float>(Mouse::getPosition(Engine::GetWindow()).y)))) {


        quitButton.setFillColor(sf::Color::Green);

        // Check if the quit button is clicked
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Engine::GetWindow().close();
        }
    }
    else {
        quitButton.setFillColor(originalColor);
    }

    Scene::Update(dt);

}

void MenuScene::Render() {
    Engine::GetWindow().draw(titleText);
    Engine::GetWindow().draw(playButton);
    Engine::GetWindow().draw(playText);
    Engine::GetWindow().draw(quitButton);
    Engine::GetWindow().draw(quitText);
    Scene::Render();

}

void MenuScene::UnLoad() {
    cout << "Menu Unload \n";
    setLoaded(false);
}