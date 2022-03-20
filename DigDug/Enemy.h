#pragma once
#include <cmath>
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	enum class EnemyType { pooka, fygar };

	Enemy();
	~Enemy();
	Enemy(sf::RenderWindow*, Game*, EnemyType);

	void changeCurrentPump(int);
	int getCurrentPump();
	bool getSandCollided();
	float getSpeed();
	bool getFloat();
	std::string getDeathType();

	void pumpUpdate();
	void die(std::string);

	virtual void collide();
	virtual void update() = 0;
	void movement();
	void reset(sf::Vector2f);

	int moveTowardPlayer();
	int moveAwayPlayer();
	int moveRandom();
	int moveFromRock(sf::Vector2f);
	int escapeLevel();
	int moveFloat();
	void checkSurroundingSand(sf::Vector2f, bool[4]);
	bool getSandCollision(int, int, int);
	void findTarget(sf::Vector2f, sf::Vector2f, int, std::vector<int>);

private:
	// Pathing based variables.
	std::vector<int> path;
	int currentPath;
	bool pathEndFound;

	// Pump based variables.
	EnemyType type; // Type of enemy: Pooka or Fygar for animations
	int currentPump; // Current amount enemy has been attacked up to 3.
	sf::Clock pumpClock; // Pump clock to determine deflation.

	// Movement based variables.
	sf::Clock escapeTimer; // Timer until enemy escapes level.
	bool sandCollided[4]; // Bool for each side of collision with sand.
	bool canFloat; // If enemy can float through sand or not.
		// Current target for where to float to if not just player.
	sf::Vector2f floatTarget;
	float speed; // Speed at which enemy moves per frame.
	int currentCollides;

	// Death based variables.
	std::string deathType;
	sf::Clock deathWait;
};

