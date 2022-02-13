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
	std::vector<Spritesheet*> sprites;
	std::vector<std::string> fileLocations;
};

