#pragma once
#include "Spritesheet.h"

class Sand
{
public:
	Sand();
	~Sand();
	Sand(sf::RenderWindow*);

	void changeSand(sf::Vector2f, int dir);
	void reset(int foreIndex, int backIndex, sf::Vector2f, int initialFore, int initialBack);
	void setActive(const bool&);
	bool getBackActive();
	bool getTopActive();

	sf::FloatRect& getBackCollider();
	sf::FloatRect& getForeCollider();
	sf::Vector2f getPosition();
	void drawObject();
private:
	sf::RenderWindow* window;
	Spritesheet foreground; // Foreground sprite with black that will be shown as the player walks into the sand.
	Spritesheet background;
	Spritesheet background2; // Background sprite of sand which is the colorful part, which will be draw over with foreground as player walks.
	sf::FloatRect boundingBox; // Allows for collisions with enemies.
	int topMask = 0; // Pixels that the TextureRect will show horizontally on background.
	int bottomMask = 0;
	int leftMask = 0; // Pixels that the TextureRect will show vertically on background.
	int rightMask = 0;
	bool upMove = false;
	bool downMove = false;
	bool leftMove = false;
	bool rightMove = false;
	int initialForeground;
	int initialBackground;
	bool backActive;
	bool topActive;
};

