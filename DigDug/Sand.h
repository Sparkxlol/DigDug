#pragma once
#include "Spritesheet.h"

class Sand
{
public:
	Sand();
	~Sand();
	Sand(Spritesheet* fore, Spritesheet* back, Spritesheet* back2, int initialFore, int initialBack, sf::Vector2f, sf::RenderWindow*);

	void changeSand(sf::Vector2f, int dir);
	void reset(int spriteIndex, int initialFore, int initialBack);

	sf::FloatRect& getCollider();
	void drawObject();
private:
	sf::RenderWindow* window;
	Spritesheet* foreground; // Foreground sprite with black that will be shown as the player walks into the sand.
	Spritesheet* background;
	Spritesheet* background2; // Background sprite of sand which is the colorful part, which will be draw over with foreground as player walks.
	sf::FloatRect boundingBox; // Allows for collisions with enemies.
	int topMask = 0; // Pixels that the TextureRect will show horizontally on background.
	int bottomMask = 0;
	int leftMask = 0; // Pixels that the TextureRect will show vertically on background.
	int rightMask = 0;
	int initialForeground;
	int initialBackground;
};

