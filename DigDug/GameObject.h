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

	bool getActive();
	sf::Vector2f getPosition();
	bool getCanMove();
	sf::FloatRect* getCollider(); 

	void setActive(const bool&);
	void setPosition(const sf::Vector2f);
	bool setCanMove(const bool&);

	void move(sf::Vector2f);

	virtual void update() = 0;
	virtual void drawObject();

private:
	bool isActive, canMove;
	sf::Vector2f position;
	sf::FloatRect boundingBox;
	Spritesheet* spritesheet;
	Animator anim;
 	sf::RenderWindow* window;
};

