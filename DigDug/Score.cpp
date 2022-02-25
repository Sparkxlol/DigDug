#include "Score.h"


Score::Score(sf::RenderWindow* win)
	: window(win)
{ 
	spritesheet.setupSprite("Images/scores1Spritesheet.png",
		sf::Vector2i(64, 112), sf::Vector2i(32, 16));
	isActive = false;
	deathTime = 2.5f;
}


Score::~Score()
{
	// Deconstructor
}


bool Score::getActive()
{
	return true;
}


void Score::setActive(const bool& active)
{
	isActive = active;
}


void Score::changeScore(int scoreIndex)
{
	// Changes texture to scoreIndex
}


void Score::update()
{
	// Waits until clock is at deathTime, and sets not active.
}


void Score::drawObject()
{
	window->draw(spritesheet);
}
