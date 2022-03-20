#pragma once
#include "DigDug.h"
#include "Fygar.h"
#include "Pooka.h"
#include "Rock.h"
#include "Sand.h"
#include "Score.h"
#include "UI.h"

class Game
{
public:
	Game(sf::RenderWindow*);
	Game();
	~Game();

	enum class Object { dig, enemy, rock, sandPath, sandSand };

	int getArrLength(const Game::Object&) const;
	bool checkCollision(const sf::FloatRect&,
		const Game::Object&, const int&) const; // Used by objects
	DigDug* const getDigDugPointer();
	Sand* const getSandPointer(const int);
	Enemy* const getEnemyPointer(const int);
	Rock* const getRockPointer(const int);
	void createScore(sf::Vector2f, std::string);
	bool getActive(const Game::Object&, const int&) const;
	bool enemiesLeft();

	void update();

private:
	void drawObjects();
	void updateObjects();
	void setupObjects();
	void setupLevels();
	GameObject& getObject(const Game::Object&, const int&) const;
	void loadLevel(int); // input is level number 

	DigDug* digDug;
	std::vector<Enemy*> enemies;
	std::vector<Rock*> rocks;
	std::vector<Sand*> sand;
	std::vector<Score*> scores;
	UI* ui;

	bool settingUpMenu;
	int currentLevel;
	bool sandLoc[12][12] = {};
	sf::RenderWindow* window;
	std::vector<std::string> levelLocations;
};

