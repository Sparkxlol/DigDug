#pragma once
#include "GameObject.h"
#include "Enemy.h"

class Shot : public GameObject
{
public:
	Shot();
	~Shot();
	Shot(sf::RenderWindow*, Game*);
	void shoot(sf::Vector2f, int);
	void update() override;
	void collide() override;
	void reset(sf::Vector2f) override;

	bool getAttached();
private:
	void updateMask();

	sf::Clock shootWait;
	bool attached;
	Enemy* attachedEnemy;
	int currentMask;
	sf::Vector2f playerPos;
};

