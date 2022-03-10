#pragma once
#include "GameObject.h"

class Rock : public GameObject
{
public:
	Rock();
	~Rock();
	Rock(sf::RenderWindow*, Game* game);

	bool getFall() const;

	sf::FloatRect& getCollider();

	void fall();
	void die();

	virtual void update();
	virtual void collide();
	virtual void reset(sf::Vector2f) override;

private:
	bool isFalling;
	float speed;
	bool normalCollider, bottomCollider;
	//sf::RectangleShape* bottom;
};

