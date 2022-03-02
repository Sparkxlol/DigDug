#pragma once
#include <string>
#include <cmath>
#include "Spritesheet.h"

class UI
{
public:
	UI(sf::RenderWindow*, int);
	~UI();

	void setScore(int);
	void setRound(int);
	void setLives(int);
	void drawObject();

private:
	void setFlowers();

	int currentLives = 2;
	int currentRound = 1;
	sf::RenderWindow* win;
	sf::Font font;
	sf::Text hiScore;
	sf::Text round;
	sf::Text score;
	Spritesheet lives[3];
	Spritesheet flowers[12];
};

