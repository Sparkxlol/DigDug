#include "DigDug.h"
#include "Game.h"


DigDug::DigDug() : DigDug(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/digDugSpritesheet.png",
		sf::Vector2i(256, 80), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	speed = 0.0f;
	shooting = false;
}

DigDug::~DigDug()
{

}


DigDug::DigDug(sf::RenderWindow* win, Game* game)
	: shot(win, game), GameObject(win, game) 
{
	spritesheet.setupSprite("Images/digDugSpritesheet.png",
		sf::Vector2i(256, 80), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	speed = .025f;
	shooting = false;
}


// Runs the shoot method from shot.
void DigDug::shoot()
{
	shot.shoot(getPosition(), getDirection());
	shooting = true;
}


void DigDug::die()
{
	setActive(false);
}


void DigDug::update()
{
	// Checks collision
	collide();

	// Checks player inputs
	playerInput();
	shot.update();

	// Updates animator
	anim.playAnimation();
}


// Draws digDug but also calls the shot's drawObject.
void DigDug::drawObject()
{
	if (getActive())
	{
		window->draw(spritesheet);
		shot.drawObject();
	}
}


void DigDug::collide()
{
	// Checks collision with sand using input of direction.
	// If alligned properly with sand move toward sand, if not
	// move the direction of previous input, based on sand
	// mask. When moving towards sand, should run sand mask variable
	// with player direction and position. 
	for (int i = 0; i < game->getArrLength(Game::Object::sand); i++)
	{
		if (getCollider().intersects(game->getCollider(Game::Object::sand, i)))
		{
			game->getSandPointer(i)->changeSand(getPosition(), getDirection());
		}
	}

	// Checks collision with enemy, dies if touches.
	for (int i = 0; i < game->getArrLength(Game::Object::fygar); i++)
	{
		if (getCollider().intersects(game->getCollider(Game::Object::fygar, i)))
		{
			//
		}
	}

	// Checks collision with rock, dies if rock is falling.

}


// Takes input for movement using the arrow keys
// and takes input to shoot using the Z key
void DigDug::playerInput()
{
	int input;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		input = right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		input = left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		input = up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		input = down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		input = z;
	}
	else
	{
		input = none;
	}

	switch(input)
	{
	case up:
		if (getCanMove())
		{
			move(sf::Vector2f(0, -speed));
			if (getDirection() != up || anim.getActive() == false)
			{
				setDirection(up);
				anim.setAnimation(2, 3, .2f, true);
			}
		}
		break; 
	case down:
		if (getCanMove())
		{
			move(sf::Vector2f(0, speed));
			if (getDirection() != down || anim.getActive() == false)
			{
				setDirection(down);
				anim.setAnimation(6, 7, .2f, true);
			}
		}
		break; 
	case left:
		if (getCanMove())
		{
			move(sf::Vector2f(-speed, 0));
			if (getDirection() != left || anim.getActive() == false)
			{
				setDirection(left);
				anim.setAnimation(4, 5, .2f, true);
			}
		}
		break;
	case right:
		if (getCanMove())
		{
			move(sf::Vector2f(speed, 0));
			if (getDirection() != right || anim.getActive() == false)
			{
				setDirection(right);
				anim.setAnimation(0, 1, .2f, true);
			}
		}
		break;
	case z:
		setCanMove(false);
		shoot();
		break; 
	default:
		if (!shot.getActive())
			setCanMove(true);
		anim.setActive(false);
		break;
	}
}


void DigDug::playerMovement(const int& input)
{
	//if digdug is not on a multiple of 16, prevent movement in a different
	//direction and instead move digdug to a multiple of 16
	//ex. moves right from 0 to 12, tries to move up, move digdug to 16 then allow to move up

	return;
}