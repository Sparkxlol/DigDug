#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Score.h"
#include "Sand.h"
#include "Rock.h"
#include "Pooka.h"
#include "Fygar.h"
#include "DigDug.h"
#include "UI.h"
#include "Level.h"

class Game
{
public:
	Game(sf::RenderWindow*);
	Game();
	~Game();

	void setupObjects(); 
	void setupLevels();
	void loadLevel(int); // input is level number 

	bool enemiesLeft();

	void playerInputs(); // for pause menu or something else
	void update();
	void drawObjects();

private:
	std::vector<Level*> levels;

	int currentLevel;
	UI* ui;
	sf::RenderWindow* window;
	std::vector<std::string> fileLocations;
};

