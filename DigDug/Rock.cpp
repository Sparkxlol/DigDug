#include "Rock.h"
#include "Game.h"

Rock::Rock() : Rock(nullptr, nullptr)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	isFalling = false;
	speed = .25f;
	bottomCollider = false;
	normalCollider = false;
}


Rock::~Rock()
{
	//delete bottom;
}


Rock::Rock(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	speed = .25f;
	isFalling = false;
	normalCollider = false;
	bottomCollider = false;
	//bottom = new sf::RectangleShape(sf::Vector2f(15.75, 15.75));
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
	boundingBox.top += 16.125;
	boundingBox.height -= .25f;
	boundingBox.left += .125f;
	boundingBox.width -= .25f;

	return boundingBox;
}

void Rock::fall()
{
	if (isFalling)
	{
		move(sf::Vector2f(0, speed));
	}
}


void Rock::die()
{
	isFalling = false;

	//play animation

	setActive(false);
}


void Rock::update()
{
	// Check collisions
	collide();
	
	fall();
}


void Rock::collide()
{
	//GameObject::getCollider() -- base collider, used when falling
	//getCollider() -- overriden collider, offset down to allow falling to work


	// Check collision of sand under rock, if doesn't collide, fall.
	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (game->checkCollision(GameObject::getCollider(), Game::Object::sandPath, i))
		{
			normalCollider = true;
		}
		else
		{
			normalCollider = false;
		}
			
		if (game->checkCollision(getCollider(), Game::Object::sandPath, i))
		{
			bottomCollider = true;
		}
		else
		{
			bottomCollider = false;
		}
			
	}


	if(!isFalling && !bottomCollider)//initial check if sand below
	{
		isFalling = true;
	}
	else if(isFalling && bottomCollider)//kills rock if collides with bottom sand
	{
		die();
	}
	// Check collision with enemies/player, run death functions.

	//if rock is not falling, use bigger collider that extends lower into sand below

	//if rock is falling, use smaller collider to allow proper collision with enemies/digdug

	//if digdug is below rock and looking up when sand is broken, don't fall until digdug turns a different direction
}
