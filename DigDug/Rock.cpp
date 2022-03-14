#include "Rock.h"
#include "Game.h"

Rock::Rock() : Rock(nullptr, nullptr)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
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
	reset(sf::Vector2f(0, 0));
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
	move(sf::Vector2f(0, speed));
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

	if (!checkedBottom)
	{
		checkedBottom = true;
		if (!bottomCollider)
			setActive(false); // This method is a bit glitchy to prevent rocks from spawning on non-ground areas.
	}

	if (!bottomCollider && !isFalling && !startFalling)//initial check if sand below
	{
		startFalling = true;
		fallClock.restart();
		anim.setAnimation(0, 1, .2f, true);
	}
	else if (startFalling && fallClock.getElapsedTime().asSeconds() > 1.0f)
	{
		isFalling = true;
		startFalling = false;
		anim.setAnimation(0, 0, 0, false);
	}
	else if (endFalling)
	{
		if (anim.getFinished())
		{
			die();
			if (game->checkCollision(GameObject::getCollider(), Game::Object::dig, 0))
				game->getDigDugPointer()->setActive(false);
			for (int i = 0; i < game->getArrLength(Game::Object::enemy); i++)
			{
				if (game->checkCollision(GameObject::getCollider(), Game::Object::enemy, i))
					game->getEnemyPointer(i)->setActive(false);
			}
		}
	}
	else if (isFalling && bottomCollider)//kills rock if collides with bottom sand
	{
		isFalling = false;
		endFalling = true;
		anim.setAnimation(0, 3, .2f, true);
	}

	if (isFalling)
		fall();

	anim.playAnimation();

}


void Rock::collide()
{
	//GameObject::getCollider() -- base collider, used when falling
	//getCollider() -- overriden collider, offset down to allow falling to work

	normalCollider = false;
	bottomCollider = false;
	//bottomCollider = game->checkCollision(getCollider(), Game::Object::dig, 0);

	// Check collision of sand under rock, if doesn't collide, fall.

	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (!normalCollider)
			normalCollider = game->checkCollision(GameObject::getCollider(), Game::Object::sandSand, i);
		if (!bottomCollider)
			bottomCollider = game->checkCollision(getCollider(), Game::Object::sandSand, i);
	}

	if (isFalling)
	{
		for (int i = 0; i < game->getArrLength(Game::Object::enemy); i++)
		{
			if (game->checkCollision(getCollider(), Game::Object::enemy, i))
			{
				Enemy* const currentEnemy = game->getEnemyPointer(i);
				currentEnemy->die("rock");
				currentEnemy->setPosition(sf::Vector2f(currentEnemy->getPosition().x, getPosition().y + 8.0f));
				if (anim.getFinished())
					currentEnemy->setActive(false);
			}
		}

		if (game->checkCollision(getCollider(), Game::Object::dig, 0))
		{
			DigDug* const currentDig = game->getDigDugPointer();
			currentDig->die("rock");
			currentDig->setPosition(sf::Vector2f(currentDig->getPosition().x, getPosition().y + 8.0f));
			if (anim.getFinished())
				currentDig->setActive(false);
		}
	}

	// Check collision with enemies/player, run death functions.

	//if rock is not falling, use bigger collider that extends lower into sand below

	//if rock is falling, use smaller collider to allow proper collision with enemies/digdug

	//if digdug is below rock and looking up when sand is broken, don't fall until digdug turns a different direction
}


void Rock::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	fallClock.restart();
	startFalling = false;
	isFalling = false;
	endFalling = false;
	speed = .5f;
	normalCollider = false;
	bottomCollider = false;
	checkedBottom = false;
}
