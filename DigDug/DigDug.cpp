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
	speed = .25f;
	shooting = false;
	setDirection(right);

	maxLeftMove = 0;
	maxRightMove = window->getSize().x - 48;
	maxTopMove = 16;
	maxBottomMove = window->getSize().y;
}


// Runs the shoot method from shot.
void DigDug::shoot()
{
	//"fling sprite"
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
	if (shot.getActive())
		shot.drawObject();
	GameObject::drawObject();
}


void DigDug::collide()
{
	// Checks collision with sand using input of direction.
	// If alligned properly with sand move toward sand, if not
	// move the direction of previous input, based on sand
	// mask. When moving towards sand, should run sand mask variable
	// with player direction and position. 
	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandPath, i))
		{
			game->getSandPointer(i)->changeSand(getPosition(), getDirection());
		}
	}

	// Checks collision with enemy, dies if touches.
	for (int i = 0; i < game->getArrLength(Game::Object::fygar); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::fygar, i))
		{
			die();
		}
	}

	for (int i = 0; i < game->getArrLength(Game::Object::pooka); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::pooka, i))
		{
			die();
		}
	}

	// Checks collision with rock, dies if rock is falling.
	for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::rock, i)
			&& game->getRockPointer(i)->getFall())
		{
			die();
		}
	}
}


// Takes input for movement using the arrow keys
// and takes input to shoot using the Z key
void DigDug::playerInput()
{
	int input;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		input = z;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		input = right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		input = left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		input = up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		input = down;
	else
		input = none;

	if (!shot.getActive())
		setCanMove(true);

	switch(input)
	{
	case up:
	case down:
	case right:
	case left:
		if (getCanMove())
			playerMovement(input);
	break;
	case z:
		if (!shot.getActive())
		{
			anim.setActive(false);
			setCanMove(false);
			shoot();
		}
		break; 
	default:
		anim.setActive(false);
		break;
	}
}


void DigDug::playerMovement(const int& input)
{
	//if digdug is not on a multiple of 16, prevent movement in a different
	//direction and instead move digdug to a multiple of 16
	//ex. moves right from 0 to 12, tries to move up, move digdug to 16 then allow to move up

	//!! Might need to change pos to 0 properly if off-sync !!

	sf::Vector2f upV = sf::Vector2f(0, -speed); // Change to GameObject move class
	sf::Vector2f downV = sf::Vector2f(0, speed);
	sf::Vector2f leftV = sf::Vector2f(-speed, 0);
	sf::Vector2f rightV = sf::Vector2f(speed, 0);
	
	float xPos = getPosition().x / 16 - (static_cast<int>(getPosition().x) / 16);
	float yPos = getPosition().y / 16 - (static_cast<int>(getPosition().y) / 16);

	xPos = (xPos < .002f || xPos > .998f) ? 0.0f : xPos;
	yPos = (yPos < .002f || yPos > .998f) ? 0.0f : yPos;


	/*
	WIP Eric

	maxLeftMove = 0;
	maxRightMove = window->getSize().x - 48;
	maxTopMove = 16;
	maxBottomMove = window->getSize().y;
	up down left right
	

	bool valid[4];
	if (getPosition().x == maxLeftMove && getDirection() == left)
		valid[left] = false;
	else
		valid[left] = true;
	*/

	if (input == up || input == down)
	{
		if (xPos != 0)
		{
			if (getDirection() == left)
				move(leftV); 
			else
				move(rightV);
		}
		else
		{
			if (input == up)
			{
				move(upV);
				if (getDirection() != up || anim.getActive() == false)
				{
					setDirection(up);
					anim.setAnimation(2, 3, .2f, true);
				}
			}
			else
			{
				move(downV);
				if (getDirection() != down || anim.getActive() == false)
				{
					setDirection(down);
					anim.setAnimation(6, 7, .2f, true);
				}
			}
		}
	}
	else
	{
		if (yPos != 0)
		{
			if (getDirection() == up)
				move(upV);
			else
				move(downV);
		}
		else
		{
			if (input == left)
			{
				move(leftV);
				if (getDirection() != left || anim.getActive() == false)
				{
					setDirection(left);
					anim.setAnimation(4, 5, .2f, true);
				}
			}
			else
			{
				move(rightV);
				if (getDirection() != right || anim.getActive() == false)
				{
					setDirection(right);
					anim.setAnimation(0, 1, .2f, true);
				}
			}
		}
	}

	return;
}