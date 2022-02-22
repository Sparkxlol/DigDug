#pragma once
#include "Spritesheet.h"

class Score
{
public:
	Score();
	~Score();
	Score(Spritesheet* sprite);

	bool getActive();
	void setActive(const bool&);

	void changeScore(int scoreIndex);
	void update();
private:
	Spritesheet* sprite;
	sf::Clock clock;
	bool isActive;
	float deathTime; // Time until object is nonActive.
};

