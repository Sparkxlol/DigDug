#pragma once
#include "Spritesheet.h"

class Sand
{
public:
	Sand();
	~Sand();
	Sand(Spritesheet* fore, Spritesheet* back, int initialFore, int initialBack);

	void changeSand(int dir, int mask);
	void reset(int spriteIndex, int initialFore, int initialBack);

	sf::FloatRect* getCollider();
private:
	Spritesheet* foreground; // Foreground sprite with black that will be shown as the player walks into the sand.
	Spritesheet* background; // Background sprite of sand which is the colorful part, which will be draw over with foreground as player walks.
	sf::FloatRect boundingBox; // Allows for collisions with enemies.
	int direction; // Current direction that player has moved into sand, will only change when player walks toward sand.
	int xMask; // Pixels that the TextureRect will show horizontally on background.
	int yMask; // Pixels that the TextureRect will show vertically on background.
};

