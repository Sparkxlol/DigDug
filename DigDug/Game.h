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

	void update();
	void setupTextures();//Not really textures
	void setupLevels();
	void loadLevel(int); // input is level number 
	void drawObjects();
	void playerInputs(); // for pause menu or something else

	int enemiesLeft();

private:
	DigDug* digDug;
	std::vector<Fygar*> fygars;
	std::vector<Pooka*> pookas;
	std::vector<Rock*> rocks;
	std::vector<Sand*> sand;
	std::vector<Level*> levels;
	std::vector<Score*> scores;

	int currentLevel;
	UI* ui;
	sf::RenderWindow* window;
	std::vector<std::string> fileLocations;
};

