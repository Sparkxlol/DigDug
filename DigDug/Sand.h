#pragma once
#include "Spritesheet.h"

class Game;

class Sand
{
public:
	Sand();
	~Sand();
	Sand(sf::RenderWindow*, Game*);

	bool changeSand(sf::Vector2f, int dir);
	void reset(int round, bool, bool[12][12]);
	void setActive(const bool&);
	void preReset(sf::Vector2f); // Sets if full sand or partial sand at start
	bool getBackActive();
	bool getTopActive();

	sf::FloatRect& getBackCollider();
	sf::FloatRect& getForeCollider();
	bool checkTopCollider(const sf::FloatRect&);
	sf::Vector2f getPosition();

	void drawObject();
private:
	void setSprite();

	Game* game;
	sf::RenderWindow* window;
	Spritesheet foreground; // Foreground sprite with black that will be shown as the player walks into the sand.
	Spritesheet background2; // Background sprite of sand which is the colorful part, which will be draw over with foreground as player walks.
	sf::FloatRect boundingBox; // Allows for collisions with enemies.
	int topMask; // Pixels that the TextureRect will show horizontally on background.
	int bottomMask;
	int leftMask; // Pixels that the TextureRect will show vertically on background.
	int rightMask;
	bool upMove;
	bool downMove;
	bool leftMove;
	bool rightMove;
	bool backActive;
	bool topActive;
};

