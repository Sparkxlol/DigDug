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


// Creates a collider for the player that is more lenient on enemy collision.
sf::FloatRect& DigDug::getCollider()
{
	boundingBox = spritesheet.getGlobalBounds();
	boundingBox.left += 3.0f;
	boundingBox.width -= 6.0f;
	boundingBox.top += 3.0f;
	boundingBox.height -= 6.0f;

	return boundingBox;
}


// Updates digDug, doing collision, input, animation updates
// as well as checking for death and updating shot.
void DigDug::update()
{
	// Checks collision if not dead.
	if (deathType == "none")
		collide();

	//if sound is stopped and should be playing, play sound (playing for first time)
	if (sounds.at((int)SoundChoice::theme).getStatus() == sf::Sound::Status::Stopped && playSound)
		sounds.at((int)SoundChoice::theme).play();
	//if sound is paused and should be playing, play sound
	else if (sounds.at((int)SoundChoice::theme).getStatus() == sf::Sound::Status::Paused && playSound)
		sounds.at((int)SoundChoice::theme).play();
	//if sound is playing and should stop, pause sound
	else if(sounds.at((int)SoundChoice::theme).getStatus() == sf::Sound::Status::Playing && !playSound)
		sounds.at((int)SoundChoice::theme).pause();

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
		if (game->checkCollision(GameObject::getCollider(), Game::Object::sandPath, i))
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
	// If a shot is active, and input is opposite of direction,
	// disconnect shot.
	else if (input <= right)
	{
		int oppDirection = (input % 2 == 0) ? input + 1 : input - 1;
		if (oppDirection == getDirection())
			shot.setActive(false);
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

	// Creates a new collider to check for rock collisions that is larger.
	sf::FloatRect largeCollider = GameObject::getCollider();
	bool collided = false;

	largeCollider.top -= speed * 3;
	largeCollider.height += speed * 12;
	largeCollider.left -= speed * 3;
	largeCollider.width += speed * 12;

	// Checks collision with all active rocks in specified direction.
	for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
	{
		if (game->checkCollision(largeCollider, Game::Object::rock, i))
		{
			sf::Vector2f rockPos = game->getRockPointer(i)->getPosition();
			sf::Vector2f digPos = getPosition();

			// Adds the speed to the position to see if it's the same as rock.
			switch (getDirection())
			{
			case up:
				digPos.y -= speed;
				break;
			case down:
				digPos.y += speed;
				break;
			case left:
				digPos.x -= speed;
				break;
			case right:
				digPos.x += speed;
				break;
			}

			// Finds the tile that the rock and dig + speed are on.
			rockPos = sf::Vector2f(static_cast<int>(rockPos.x) / 16,
				static_cast<int>(rockPos.y) / 16);
			digPos = sf::Vector2f(static_cast<int>(digPos.x) / 16,
				static_cast<int>(digPos.y) / 16);

			// If the digDug pos is the same as the rock, and moving in
			// correct direction, prevent movement.
			// Down/Right have an added 1 because they are 16 pixels off
			// compared to the left and right, since pos is top left.
 			if (getDirection() == up && digPos.y == rockPos.y
				&& digPos.x == rockPos.x)
				collided = true;
			else if (getDirection() == down && digPos.y + 1 == rockPos.y
				&& digPos.x == rockPos.x)
				collided = true;
			else if (getDirection() == left && digPos.x == rockPos.x
				&& digPos.y == rockPos.y)
				collided = true;
			else if (getDirection() == right && digPos.x + 1 == rockPos.x
				&& digPos.y == rockPos.y)
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