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


// Returns if the rock is falling.
bool Rock::getFall() const
{
	return isFalling;
}


// Overriden collider to allow rock to check collision below itself.
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


// Moves the rock down from its position.
void Rock::fall()
{
	move(sf::Vector2f(0, speed));
}


// Kills the rock, setting it un-active.
void Rock::die()
{
	isFalling = false;

	setActive(false);
}


// Updates the rock based on current variables.
void Rock::update()
{
	// Check collisions
	collide();

	// If the bottom does not exist at the start, set active un-active.
	if (!checkedBottom)
	{
		checkedBottom = true;
		if (!bottomCollider)
			setActive(false); 
	}

	// If bottom is not collided and hasn't started falling, fall.
	if (!bottomCollider && !isFalling && !startFalling)
	{
		startFalling = true;
		fallClock.restart();
		anim.setAnimation(0, 1, .2f, true);
	}
	// If started falling and has been 1 second, begin downward movement.
	else if (startFalling && fallClock.getElapsedTime().asSeconds() > 1.0f)
	{
		isFalling = true;
		startFalling = false;
		anim.setAnimation(0, 0, 0, false);
	}
	// If the rock should stop falling, and the death animation is finished,
	// kill collided objects (enemies and digDug).
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
	// If falling and has collided, end fall.
	else if (isFalling && bottomCollider)//kills rock if collides with bottom sand
	{
		isFalling = false;
		endFalling = true;
		anim.setAnimation(0, 3, .2f, true);
	}

	// If just falling, continue fall. ???????
	if (isFalling)
		fall();

	anim.playAnimation();

}


// Checks collisions with objects under it and 
void Rock::collide()
{
	//getCollider() -- overriden collider, offset down to allow falling to work

	bottomCollider = false;

	// Check collision of sand under rock, if doesn't collide, fall.
	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (!bottomCollider)
			bottomCollider = game->checkCollision(getCollider(), Game::Object::sandSand, i);
	}

	// If currently falling check collisions for enemies or digDug
	// and kill them as well as move them downward until stopping.
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
}


// Resets rock to default variables.
void Rock::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	fallClock.restart();
	startFalling = false;
	isFalling = false;
	endFalling = false;
	speed = .5f;
	bottomCollider = false;
	checkedBottom = false;
}
