#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UI.h"

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

	void playerInputs();
	void update();
	void drawObjects();

private:
	int currentLevel;
	UI* ui;
	sf::RenderWindow* window;
	std::vector<std::string> fileLocations;
};

