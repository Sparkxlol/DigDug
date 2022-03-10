#pragma once
#include "GameObject.h"
#include "Shot.h"

class DigDug : public GameObject
{
public:
	DigDug();
	~DigDug();
	DigDug(sf::RenderWindow*, Game*);

	void shoot();
	void playerInput();
	void die();

	void update() override;
	void drawObject() override;
	void collide() override;

private:
	void playerMovement(const int&);

	bool shooting;
	Shot shot;
	float speed;
};

