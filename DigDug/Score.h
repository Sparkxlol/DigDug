#pragma once
#include "Spritesheet.h"

class Score
{
public:
	Score(sf::RenderWindow*);
	~Score();

	bool getActive();
	void setActive(const bool&);

	void changeScore(int scoreIndex);
	void update();
	void drawObject();
private:
	Spritesheet spritesheet;
	sf::Clock clock;
	sf::RenderWindow* window;
	bool isActive;
	float deathTime; // Time until object is nonActive.
};

