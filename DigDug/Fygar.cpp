#include "Fygar.h"
#include "Game.h"


Fygar::Fygar() : Fygar(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/fygarSpritesheet.png",
		sf::Vector2i(128, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
}


Fygar::~Fygar()
{
	// Deconstructor
}


Fygar::Fygar(sf::RenderWindow* win, Game* game)
	: Enemy(win, game, EnemyType::fygar), fire(win, game)
{
	spritesheet.setupSprite("Images/fygarSpritesheet.png",
		sf::Vector2i(128, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
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

	anim.playAnimation();

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

	if (randChoice <= 1 && !getFloat())
		shootFire();
	else
	{
		Enemy::movement();
		
		// Set animator to correct animation based on movement direction
		if (getDirection() == right)
			spritesheet.loadSprite(0);
		else
			spritesheet.loadSprite(8);
	}
}


void Fygar::shootFire()
{
	// Sets fire active and uses its shoot method
	fire.setActive(true);
	fire.shoot(getPosition(), getDirection());
}


void Fygar::reset(sf::Vector2f pos)
{
	Enemy::reset(pos);

	anim.setAnimation(0, 1, .2f, false);
	fire.reset(pos);
}
