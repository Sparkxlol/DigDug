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

	enum class Object { dig, fygar, pooka, rock, sand, sand2 };

	int getArrLength(const Game::Object&) const;
	sf::FloatRect& getCollider(const Game::Object&, const int&) const; // Used by objects
	DigDug* getDigDugPointer();
	Sand* getSandPointer(const int);
	Fygar* getFygarPointer(const int);
	Pooka* getPookaPointer(const int);
	Rock* getRockPointer(const int);
	bool getActive(const Game::Object&, const int&) const;

	void update();

private:
	void drawObjects();
	void updateObjects();
	void setupObjects();
	void setupLevels();
	GameObject& getObject(const Game::Object&, const int&) const;
	void loadLevel(int); // input is level number 

	bool enemiesLeft();

	DigDug* digDug;
	std::vector<Fygar*> fygars;
	std::vector<Pooka*> pookas;
	std::vector<Rock*> rocks;
	std::vector<Sand*> sand;
	std::vector<Score*> scores;

	int currentLevel;
	//UI* ui;
	sf::RenderWindow* window;
	std::vector<std::string> levelLocations;
	std::vector<std::string> sandLocations;
};

