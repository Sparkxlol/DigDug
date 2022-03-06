#include "Fygar.h"
#include "Game.h"


Fygar::Fygar() : Fygar(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/fygarSpritesheet.png",
		sf::Vector2i(128, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
}


Fygar::~Fygar()
{
	// Deconstructor
}


Fygar::Fygar(sf::RenderWindow* win, Game* game)
	: Enemy(win, game), fire(win, game)
{
	spritesheet.setupSprite("Images/fygarSpritesheet.png",
		sf::Vector2i(128, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);

}


void Fygar::update()
{
	collide();

	// Update movement
	if (getCurrentPump() <= 0)
	{
		if (!fire.getActive())
			movement();
	}
	else
		pumpUpdate();

	// Updates fire
	if (fire.getActive())
		fire.update();
}


void Fygar::drawObject()
{
	// Calls overrided draw, but also calls the fires
	if(fire.getActive())
		fire.drawObject();
	GameObject::drawObject();
}


void Fygar::movement()
{
	int randChoice = rand() % 1000 + 1;
	float xChange = getPosition().x - game->getDigDugPointer()->getPosition().x;
	float yChange = getPosition().y - game->getDigDugPointer()->getPosition().y;

	if (randChoice <= 1)
		shootFire();
	else
		Enemy::movement();
}


void Fygar::shootFire()
{
	// Sets fire active and uses its shoot method
	fire.setActive(true);
	fire.shoot(getPosition(), getDirection());
}