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
	sf::Clock fallClock;
	bool startFalling;
	bool isFalling;
	bool endFalling;
	float speed;
	bool bottomCollider;
	
	bool checkedBottom;
};

