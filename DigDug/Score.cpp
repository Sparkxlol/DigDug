#include "Score.h"


Score::Score() : Score(nullptr)
{ }


Score::~Score()
{
	// Deconstructor
}


Score::Score(Spritesheet* s)
	: sprite(s)
{
	isActive = false;
	deathTime = 2.5f;
}


bool getActive()
{

}


void setActive(const bool&)
{

}


void changeScore(int scoreIndex)
{
	// Changes texture to scoreIndex
}


void update()
{
	// Waits until clock is at deathTime, and sets not active.
}
