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

	if (!theme.loadFromFile("Sounds/theme.wav"))
		std::cout << "error loading music" << std::endl; // error

	sound.setBuffer(theme);

	playSound = false;
}


// Runs the shoot method from shot.
void DigDug::shoot()
{
	shot.shoot(getPosition(), getDirection());
	shooting = true;
}


/* Sets the death type to the corresponding given type:
* "rock": death by falling rock; "enemy": death by enemy/fire.
* Restarts death time so the player can be set non-active
* either after an animation or when the rock hits the floor. */
void DigDug::die(std::string type)
{
	if (deathType == "none")
	{
		deathWait.restart();
		deathType = type;		
	}
}


// Updates digDug, doing collision, input, animation updates
// as well as checking for death and updating shot.
void DigDug::update()
{
	// Checks collision if not dead.
	if (deathType == "none")
		collide();

	//if (playSound)
	//{
	//	sound.play();
	//}
	//else
	//	sound.pause();

	playerInput();
	shot.update();

	// If dead by enemy, wait 2 seconds and then restart level.
	if (deathWait.getElapsedTime().asSeconds() > 2.0f && deathType == "enemy")
		setActive(false);
	// If dead by enemy animation is finished, prevent loops.
	else if (anim.getFinished() && deathType == "enemy")
		anim.setActive(false);

	anim.playAnimation();
}


// Draws digDug but also calls the shot's drawObject.
void DigDug::drawObject()
{
	if (shot.getActive())
		shot.drawObject();
	GameObject::drawObject();
}


// Checks collision with sand and enemy, either changing how sand looks
// or killing digDug.
void DigDug::collide()
{	
	// Leniency for the sand digging animation.
	// Allows 5 frames of leniency between when animation switches from
	// digging back to walking.
	if (sandCollided > 0)
		sandCollided--;

	// Checks each sand and changes it if collided based on player position.
	for (int i = 0; i < game->getArrLength(Game::Object::sandPath); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandPath, i))
		{
			if (game->getSandPointer(i)->changeSand(getPosition(), getDirection()))
				sandCollided = 5; // Shows digging animation.
		}
	}

	// Checks collision with enemy, dies if enemy is not pumped.
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

	// If shot isn't active, resets the shot and allows movement
	// and a new shot.
	if (!shot.getActive())
	{
		if (shooting)
		{
			shot.reset(getPosition());
			shooting = false;
			setCanMove(true);
		}
	}

	// If not dead, either moves player in corresponding direction or shoots.
	if (deathType == "none")
	{
		switch (input)
		{
		case up:
		case down:
		case right:
		case left:
			if (getCanMove())
			{
				playerMovement(input);
				playSound = true;
			}
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
			// Stops animations if staying still.
			anim.setActive(false);
			playSound = false;
			break;
		}
	}

	// Sets animations based on input.
	setAnimations(input);
}


// Moves based on the input but forces movement based on position
// as well as collisions, specifically rock.
void DigDug::playerMovement(const int& input)
{
	// Finds the distance away from a 16 (from the top & left) in both axes.
	float xPos = (getPosition().x / 16)
		- (static_cast<int>(getPosition().x) / 16);
	float yPos = (getPosition().y / 16)
		- (static_cast<int>(getPosition().y) / 16);

	// If xPos/yPos is close to 0 set to 0,
	// to check when digDug is close enough to the axis to move on that axis.
	xPos = (xPos < .002f || xPos > .998f) ? 0.0f : xPos;
	yPos = (yPos < .002f || yPos > .998f) ? 0.0f : yPos;

	// If inputted up or down attempt to change direction to up/down.
	// Otherwise try to change direction to left/right.
	if (input == up || input == down)
	{
		// If on y-axis change direction to the "correct" direction.
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
		// If on x-axis change direction to the "correct" direction.
		if (yPos == 0)
		{
			if (input == left)
				setDirection(left);
			else
				setDirection(right);
		}
	}

	// Creates a new collider and changes the size of it
	// based on the current direction to check in that direction.
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

	// Checks collision with all active rocks in specified direction.
	for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
	{
		if (game->checkCollision(largeCollider, Game::Object::rock, i))
		{
			collided = true;
		}
	}

	// If not collided in direction with a rock, move in that direction.
	if (!collided)
	{
		switch (getDirection())
		{
		case up:
			if (getPosition().y - speed >= 16)
				move(sf::Vector2f(0, -speed));
			break;
		case down:
			if (getPosition().y + speed <= 13 * 16)
				move(sf::Vector2f(0, speed));
			break;
		case left:
			if (getPosition().x - speed >= 0)
				move(sf::Vector2f(-speed, 0));
			break;
		case right:
			if (getPosition().x + speed <= 11 * 16)
				move(sf::Vector2f(speed, 0));
			break;
		}
	}

	return;
}


/* Sets animation to the correct animation based on direction and variables
* 1. If dead by enemy, sets animation to dying animation
* 2. If dead by rock, sets animation to crushed animation or stops
*    the animation on the last frame.
* 3. If currently shooting, either sets to a throwing animation
*    or a pumping animation if shot is already attached.
* 4. Otherwise, sets animation to digging if currently digging through sand
*    or normal walking. */
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


// Resets digDug to default variables and animation.
void DigDug::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	deathType = "none";
	sandCollided = false;
	shooting = false;
	speed = .25f;
	shot.reset(pos);

	anim.setAnimation(0, 1, .2f, false); // Walking right.
}