#include "Fygar.h"


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


void Fygar::shootFire()
{
	// Sets fire active and uses its shoot method
	fire.setActive(true);
	fire.shoot(getPosition(), getDirection());
}


void Fygar::update()
{
	// Update movement

	// Check collisions
	collide();
	// Update animator

	// Updates fire
	fire.update();
}


void Fygar::drawObject()
{
	// Calls overrided draw, but also calls the fires
	if(fire.getActive())
		fire.drawObject();
	GameObject::drawObject();
}


void Fygar::collide()
{
	//
}


void Fygar::movement()
{
	// Moves until hits wall, using collision checks

	// When stuck floats periodically
	
	// If in range of player moves toward player and shoots fire.
}