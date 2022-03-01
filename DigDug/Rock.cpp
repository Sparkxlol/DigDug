#include "Rock.h"
#include "Game.h"

Rock::Rock() : Rock(nullptr, nullptr)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	isFalling = false;
}


Rock::~Rock()
{

}


Rock::Rock(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	isFalling = false;
}


bool Rock::getFall() const
{
	return isFalling;
}


//overriden collider to allow rock to check collision below itself
sf::FloatRect& Rock::getCollider()
{
	// Smaller than sprite to allow objects to not collide
	// with things with tiny extrusions. Ex. Dig Dug & Sand
	boundingBox = spritesheet.getGlobalBounds();
	boundingBox.left += .125f;
	boundingBox.width -= .25f;
	boundingBox.top += .125f;
	boundingBox.height += .25f;

	return boundingBox;
}


void Rock::startFall()
{
	// Set is falling to true, and run falling animation.

	isFalling = true;

}


void Rock::die()
{
	isFalling = false;
	setActive(false);
}


void Rock::update()
{
	// Check collisions
	collide();

	// If rock started falling, check when animation is done
	// and run fall.
}


void Rock::collide()
{
	//GameObject::getCollider() -- base collider, used when falling
	//getCollider() -- overriden collider, offset down to allow falling to work

	bool largeColliderCollision = false;
	bool normalColliderCollision = false;

	// Check collision of sand under rock, if doesn't collide, fall.
	for (int i = 0; i < game->getArrLength(Game::Object::sand); i++)
	{
		if (GameObject::getCollider().intersects(game->getCollider(Game::Object::sand, i)))
		{
			largeColliderCollision = true;
		}
		if (getCollider().intersects(game->getCollider(Game::Object::sand, i)))
		{
			normalColliderCollision = true;
		}
	}

	if(!isFalling && !largeColliderCollision)
	{
		startFall();
	}
	else if(isFalling && normalColliderCollision)
	{
		die();
	}
	// Check collision with enemies/player, run death functions.



	//if rock is not falling, use bigger collider that extends lower into sand below

	//if rock is falling, use smaller collider to allow proper collision with enemies/digdug

	//if digdug is below rock and looking up when sand is broken, don't fall until digdug turns a different direction
}
