#include "DigDug.h"


DigDug::DigDug() : DigDug(nullptr, nullptr, sf::Vector2f(0, 0), nullptr)
{
	speed = 0.0f;
	shooting = false;
}

DigDug::~DigDug()
{

}


DigDug::DigDug(Spritesheet* s, Spritesheet* shotS, sf::Vector2f pos, sf::RenderWindow* win)
	: shot(shotS, pos, win), GameObject(s, pos, win) 
{
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
	// 
}


void DigDug::update()
{
	// Checks collision
	collide();

	// Checks player inputs
	playerInput();

	// Updates animator
	anim.playAnimation();
}


// Draws digDug but also calls the shot's drawObject.
void DigDug::drawObject()
{
	window->draw(*spritesheet);
	shot.drawObject();
}


void DigDug::collide()
{
	// Checks collision with sand using input of direction.
	// If alligned properly with sand move toward sand, if not
	// move the direction of previous input, based on sand
	// mask. When moving towards sand, should run sand mask variable
	// with player direction and position. 

	// Checks collision with enemy, dies if touches.

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
		setCanMove(false);
	}
	else
		input = none;

	switch(input)
	{
	case up:
		move(sf::Vector2f(0, -speed));
		if (getDirection() != up || anim.getActive() == false)
		{
			setDirection(up);
			anim.setAnimation(2, 3, .2f, true);
		}
		break; 
	case down:
		move(sf::Vector2f(0, speed));
		if (getDirection() != down || anim.getActive() == false)
		{
			setDirection(down);
			anim.setAnimation(6, 7, .2f, true);
		}
		break; 
	case left:
		move(sf::Vector2f(-speed, 0));
		if (getDirection() != left || anim.getActive() == false)
		{
			setDirection(left);
			anim.setAnimation(4, 5, .2f, true);
		}
		break;
	case right:
		move(sf::Vector2f(speed, 0));
		if (getDirection() != right || anim.getActive() == false)
		{
			setDirection(right);
			anim.setAnimation(0, 1, .2f, true);
		}
		break;
	case z:
		shoot();
		break; 
	default:
		anim.setActive(false);
		break;
		// nothing
	}

	//set coordinate to move in switch statement above.
	//move()
	// If player presses Z, shoot the shot and prevent movement.
}