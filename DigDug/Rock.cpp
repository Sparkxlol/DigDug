#include "Rock.h"

Rock::Rock()
{
	isFalling = false;
}


Rock::~Rock()
{
}


void Rock::fall()
{
	isFalling = true;
	//falling stuff

	return;
}


bool Rock::getIsFalling()
{
	return isFalling;
}


void Rock::update()
{
	//update things

	return;
}
