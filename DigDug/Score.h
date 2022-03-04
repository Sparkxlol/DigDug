#pragma once
#include "Spritesheet.h"
#include "UI.h"

class Score
{
public:
	Score(sf::RenderWindow*, UI* const);
	~Score();

	bool getActive();
	void setActive(const bool&);

	void changeScore(sf::Vector2f, std::string);
	void update();
	void drawObject();
private:
	UI* const ui;
	Spritesheet spritesheet;
	sf::Clock clock;
	sf::RenderWindow* window;
	int totalScore;
	bool isActive;
	float deathTime; // Time until object is nonActive.
};

