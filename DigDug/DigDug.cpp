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
	if (deathType == "none")
	{
		deathWait.restart();

		std::cout << type;
		deathType = type;		
	}
}


void DigDug::update()
{
	// Checks collision
	if (deathType == "none")
		collide();

	// Checks player inputs
	playerInput();
	shot.update();

	if (deathWait.getElapsedTime().asSeconds() > 2.0f && deathType == "enemy")
		setActive(false);
	else if (anim.getFinished() && deathType == "enemy")
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
	for (int i = 0; i < game->getArrLength(Game::Object::enemy); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::enemy, i)
			&& game->getEnemyPointer(i)->getCurrentPump() <= 0)
		{
			die("enemy");
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
		if (shooting)
		{
			shot.reset(getPosition());
			shooting = false;
			setCanMove(true);
		}
	}

	if (deathType == "none")
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
		if (xPos == 0)
		{
			if (input == up)
				setDirection(up);
			else
				setDirection(down);
		}
	}
	else
	{
		if (yPos == 0)
		{
			if (input == left)
				setDirection(left);
			else
				setDirection(right);
		}
	}

	sf::FloatRect largeCollider = getCollider();
	bool collided = false;

	switch (getDirection())
	{
	case up:
		largeCollider.top += .25f;
		largeCollider.height -= 5.0f;
		break;
	case down:
		largeCollider.height -= 4.75f;
		largeCollider.top += 5.0f;
		break;
	case left:
		largeCollider.left += .25f;
		largeCollider.width -= 5.0f;
		break;
	case right:
		largeCollider.width -= 4.75f;
		largeCollider.left += 5.0f;
		break;
	}

	for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
	{
		if (game->checkCollision(largeCollider, Game::Object::rock, i))
		{
			collided = true;
		}
	}

	if (!collided)
	{
		switch (getDirection())
		{
		case up:
			move(upV);
			break;
		case down:
			move(downV);
			break;
		case left:
			move(leftV);
			break;
		case right:
			move(rightV);
			break;
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
		if (deathType == "enemy")
			anim.setAnimation(60, 63, .2f, true);
		else if (deathType == "rock")
		{
			if (anim.getFrame() == 51)
				anim.setActive(false);
			else
				anim.setAnimation(50, 51, .2f, true);
		}
		else if (shooting)
		{
			if (!shot.getAttached())
				spritesheet.loadSprite(25);
			else
				anim.setAnimation(34, 35, .5f, true);
		}
		else if (input != none && input != z)
		{
			if (sandCollided > 0 && input != z)
				anim.setAnimation(10, 11, .2f, true);
			else
				anim.setAnimation(2, 3, .2f, true);
		}
		break;
	case down:
		if (deathType == "enemy")
			anim.setAnimation(68, 71, .2f, true);
		else if (deathType == "rock")
		{
			if (anim.getFrame() == 55)
				anim.setActive(false);
			else
				anim.setAnimation(54, 55, .2f, true);
		}
		else if (shooting)
		{
			if (!shot.getAttached())
				spritesheet.loadSprite(27);
			else
				anim.setAnimation(38, 39, .5f, true);
		}
		else if (input != none && input != z)
		{
			if (sandCollided > 0)
				anim.setAnimation(14, 15, .2f, true);
			else
				anim.setAnimation(6, 7, .2f, true);
		}
		break;
	case left:
		if (deathType == "enemy")
			anim.setAnimation(64, 67, .2f, true);
		else if (deathType == "rock")
		{
			if (anim.getFrame() == 53)
				anim.setActive(false);
			else
				anim.setAnimation(52, 53, .2f, true);
		}
		else if (shooting)
		{
			if (!shot.getAttached())
				spritesheet.loadSprite(26);
			else
				anim.setAnimation(36, 37, .5f, true);
		}
		else if (input != none && input != z)
		{
			if (sandCollided > 0)
				anim.setAnimation(12, 13, .2f, true);
			else
				anim.setAnimation(4, 5, .2f, true);
		}
		break;
	case right:
		if (deathType == "enemy")
			anim.setAnimation(56, 59, .2f, true);
		else if (deathType == "rock")
		{
			if (anim.getFrame() == 49)
				anim.setActive(false);
			else
				anim.setAnimation(48, 49, .2f, true);
		}
		else if (shooting)
		{
			if (!shot.getAttached())
				spritesheet.loadSprite(24);
			else
				anim.setAnimation(32, 33, .5f, true);
		}
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

	deathType = "none";
	sandCollided = false;
	shooting = false;
	speed = .25f;
	shot.reset(pos);

	anim.setAnimation(0, 1, .2f, false);
}