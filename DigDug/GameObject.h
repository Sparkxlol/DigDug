#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Spritesheet.h"
#include "Animator.h"
#include <vector>

class Game;

class GameObject
{
public:
	GameObject();
	~GameObject();
	GameObject(sf::RenderWindow*, Game*);

	int getDirection();
	bool getActive();
	sf::Vector2f getPosition();
	bool getCanMove();
	virtual sf::FloatRect& getCollider();

	void setDirection(const int&);
	void setActive(const bool&);
	void setPosition(const sf::Vector2f);
	bool setCanMove(const bool&);

	void move(sf::Vector2f);

	// Runs every frame and should update collisions, but not draw object.
	virtual void update() = 0; 
	virtual void drawObject();
	// Checks collisions with objects that most affect player.
	virtual void collide() = 0; 
	virtual void reset(sf::Vector2f);
	// Collide levels (who should be the collision checker for most cases):
	// Rock
	// Player
	// Enemies
	// Sand


protected:
	Game* game;
	sf::FloatRect boundingBox;
	Spritesheet spritesheet;
	Animator anim;
	sf::RenderWindow* window;
	std::vector<sf::SoundBuffer> soundFiles;
	std::vector<sf::Sound> sounds;
	enum Input { up, down, left, right, z, none };
	enum class SoundChoice {theme, shot, pump, digdugDeath};

private:
	int direction; // Direction of object animation, *might change to enum class*
	bool isActive, canMove; // If not active, do not run update.
};

