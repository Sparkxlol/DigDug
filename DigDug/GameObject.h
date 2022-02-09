#pragma once
#include <SFML/Graphics.hpp>


class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void drawObject(sf::RenderWindow*);

	bool getActive();
	sf::Vector2f getPosition();
	bool getCanMove();
	//BoundingBox getCollider(); //need to fix what bounding box is

	void setActive(bool);
	void setPosition(sf::Vector2f);
	bool setCanMove();
	
	void move(sf::Vector2f);
	virtual void update() = 0;

private:
	bool active, move;

};

