#include "DigDug.h"
#include "Game.h"


DigDug::DigDug() : DigDug(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/digDugSpritesheet.png",
		sf::Vector2i(256, 80), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
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
	reset(sf::Vector2f(0, 0));
}


// Runs the shoot method from shot.
void DigDug::shoot()
{
	shot.shoot(getPosition(), getDirection());
	shooting = true;
}


void DigDug::die(std::string type)
{
	if (dead == false)
	{
		dead = true;
		deathWait.restart();

		if (type == "rock")
			setActive(false);
	}
}


void DigDug::update()
{
	// Checks collision
	collide();

	// Checks player inputs
	playerInput();
	shot.update();

	if (deathWait.getElapsedTime().asSeconds() > 2.0f && dead)
		setActive(false);
	else if (anim.getFinished() && dead)
		anim.setActive(false);

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

	// Frames of leniency between each animation changing from
	// walking to digging.
	if (sandCollided > 0)
		sandCollided--;

	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandPath, i))
		{
			if (game->getSandPointer(i)->changeSand(getPosition(), getDirection()))
				sandCollided = 5;
		}
	}

	// Checks collision with enemy, dies if touches.
	for (int i = 0; i < game->getArrLength(Game::Object::fygar); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::fygar, i)
			&& game->getFygarPointer(i)->getCurrentPump() <= 0)
		{
			die("enemy");
		}
	}

	for (int i = 0; i < game->getArrLength(Game::Object::pooka); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::pooka, i)
			&& game->getPookaPointer(i)->getCurrentPump() <= 0)
		{
			die("enemy");
		}
	}

	// Checks collision with rock, dies if rock is falling.
	for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::rock, i)
			&& game->getRockPointer(i)->getFall())
		{
			die("rock");
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
	{
		shooting = false;
		setCanMove(true);
	}

	if (!dead)
	{
		switch (input)
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
			}
			shoot();
			break;
		default:
			anim.setActive(false);
			break;
		}
	}

	setAnimations(input);
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
				setDirection(up);
			}
			else
			{
				move(downV);
				setDirection(down);
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
				setDirection(left);
			}
			else
			{
				move(rightV);
				setDirection(right);
			}
		}
	}

	return;
}


void DigDug::setAnimations(const int& input)
{
	anim.setActive(false);

	switch (getDirection())
	{
	case up:
		if (dead)
			anim.setAnimation(60, 63, .2f, true);
		else if (shooting)
			spritesheet.loadSprite(25);
		else if (input != none && input != z)
		{
			if (sandCollided > 0 && input != z)
				anim.setAnimation(10, 11, .2f, true);
			else
				anim.setAnimation(2, 3, .2f, true);
		}
		break;
	case down:
		if (dead)
			anim.setAnimation(68, 71, .2f, true);
		else if (shooting)
			spritesheet.loadSprite(27);
		else if (input != none && input != z)
		{
			if (sandCollided > 0)
				anim.setAnimation(14, 15, .2f, true);
			else
				anim.setAnimation(6, 7, .2f, true);
		}
		break;
	case left:
		if (dead)
			anim.setAnimation(64, 67, .2f, true);
		else if (shooting)
			spritesheet.loadSprite(26);
		else if (input != none && input != z)
		{
			if (sandCollided > 0)
				anim.setAnimation(12, 13, .2f, true);
			else
				anim.setAnimation(4, 5, .2f, true);
		}
		break;
	case right:
		if (dead)
			anim.setAnimation(56, 59, .2f, true);
		else if (shooting)
			spritesheet.loadSprite(24);
		else if (input != none && input != z)
		{
			if (sandCollided > 0)
				anim.setAnimation(8, 9, .2f, true);
			else
				anim.setAnimation(0, 1, .2f, true);
		}
		break;
	}
}


void DigDug::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	dead = false;
	sandCollided = false;
	shooting = false;
	speed = .25f;
	shot.reset(pos);

	anim.setAnimation(0, 1, .2f, false);
}