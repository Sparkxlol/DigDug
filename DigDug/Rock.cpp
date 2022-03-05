#include "Rock.h"
#include "Game.h"

Rock::Rock() : Rock(nullptr, nullptr)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	isFalling = false;
	speed = .5f;
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
	speed = .5f;
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
	boundingBox.top = getPosition().y + 16.05f;
	boundingBox.height = .125f;
	boundingBox.left = getPosition().x + .125f;
	boundingBox.width = 15.875f;

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

	normalCollider = false;
	bottomCollider = game->checkCollision(getCollider(), Game::Object::dig, 0);

	// Check collision of sand under rock, if doesn't collide, fall.

	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (!normalCollider)
			normalCollider = game->checkCollision(GameObject::getCollider(), Game::Object::sandSand, i);
		if (!bottomCollider)
			bottomCollider = game->checkCollision(getCollider(), Game::Object::sandSand, i);
	}

	if(!bottomCollider)//initial check if sand below
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
