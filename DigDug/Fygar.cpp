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


// Updates collisions, pump amount, fire and animations.
void Fygar::update()
{
	collide();

	if (getCurrentPump() <= 0)
	{
		// If fire isn't active update movement.
		if (!fire.getActive())
			movement();
	}
	else
		pumpUpdate(); // If pump > 0, updates the pump.

	anim.playAnimation();

	// Updates fire if active.
	if (fire.getActive())
		fire.update();
}


// Draws both fygar and the fire.
void Fygar::drawObject()
{
	if (fire.getActive())
		fire.drawObject();
	GameObject::drawObject();
}


// Either randomly shoots fire if not floating or uses the overrided movements.
void Fygar::movement()
{
	int randChoice = rand() % 1000 + 1;

	if (randChoice <= 1 && !getFloat())
		shootFire();
	else
		Enemy::movement();
}


// Sets fire active and shoots it.
void Fygar::shootFire()
{
	// Sets fire active and uses its shoot method.
	fire.setActive(true);
	fire.shoot(getPosition(), getDirection());
}


// Resets fygar and fire to default variables and animation.
void Fygar::reset(sf::Vector2f pos)
{
	Enemy::reset(pos);

	fire.reset(pos);
	anim.setAnimation(0, 1, .2f, false);
}
