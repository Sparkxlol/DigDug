#pragma once
#include "Spritesheet.h"

class Animator
{
public:
	Animator();
private:
	Spritesheet* sprite; 
	sf::Clock clock;
	int maxIndex;
	int currentIndex;
};

