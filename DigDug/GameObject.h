#pragma once
#include <SFML/Graphics.hpp>
#include "Spritesheet.h"
#include "Animator.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	GameObject(Spritesheet*, sf::Vector2f, sf::RenderWindow*);

	int getDirection();
	bool getActive();
	sf::Vector2f getPosition();
	bool getCanMove();
	//sf::FloatRect* getCollider(); // Could all be protected instead of get/set :)
	//Animator* getAnimator();
	//sf::RenderWindow* getWindow();

	void setDirection(const int&);
	void setActive(const bool&);
	void setPosition(const sf::Vector2f);
	bool setCanMove(const bool&);

	void move(sf::Vector2f);

	virtual void update() = 0; // Runs every frame and should update collisions, but not draw object.
	virtual void drawObject();
	virtual void collide() = 0; // Checks collisions with objects that most affect player.

protected:
	sf::FloatRect boundingBox;
	Spritesheet* spritesheet;
	Animator anim;
	sf::RenderWindow* window;

private:
	int direction; // Direction of object animation, *might change to enum class*
	bool isActive, canMove; // If not active, do not run update.
};

