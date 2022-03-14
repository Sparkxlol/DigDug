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
	void die(std::string);

	void update() override;
	void drawObject() override;
	void collide() override;
	void reset(sf::Vector2f) override;

private:
	void playerMovement(const int&);
	void setAnimations(const int&);

	bool shooting;
	Shot shot;
	float speed;

	sf::SoundBuffer theme;
	sf::Sound sound;
	bool playSound;

	std::string deathType;
	sf::Clock deathWait;
	int sandCollided;
};

