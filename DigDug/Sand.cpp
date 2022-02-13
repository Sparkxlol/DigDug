#include "Sand.h"


Sand::Sand() : Sand(nullptr, nullptr, 0, 0)
{ }


Sand::~Sand()
{
	// Deconstructor
}


Sand::Sand(Spritesheet* fore, Spritesheet* back, int initialFore, int initialBack)
	: foreground(fore), background(back)
{
	// Sets correct sprites to foreground and background.
	// Sets x and y mask / direction to corresponding foreground.
}


void changeSand(int dir, int mask)
{
	// Change x and y mask to correct values based on inputted direction and mask
	// Use mask values to edit spritesheet texture rect.
}


void reset(int spriteIndex, int initialFore, int initialBack)
{
	// Resets the values of sand to load for the next level. 
	// Sprite index is for color changes.
}


// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect* Sand::getCollider()
{
	return &boundingBox;
}