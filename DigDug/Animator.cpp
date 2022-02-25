#include "Animator.h"

Animator::Animator()
{
	this->sprite = nullptr;
	sf::Clock clock;

	maxIndex = 0;
	minIndex = 0;
	currentIndex = 0;
	frameLength = .0f;
	active = false;
	animFinished = false;
}


Animator::~Animator()
{

}


void Animator::setSprite(Spritesheet* s)
{
	sprite = s;
	s->setPosition(50, 50);
}


// Sets animation values to parameters and loads the initial
// sprite of the animation.
void Animator::setAnimation(int minIndex, int maxIndex,
	float frameLength, bool active)
{
	this->maxIndex = maxIndex;
	this->minIndex = minIndex;
	currentIndex = minIndex;
	this->frameLength = frameLength;
	this->active = active;

	sprite->loadSprite(currentIndex);
}


// Loads the next frame of the animation.
void Animator::nextFrame()
{
	// Current index is set back to min if at max. Otherwise, it increments.
	currentIndex = (currentIndex < maxIndex) ? currentIndex + 1 : minIndex;
	sprite->loadSprite(currentIndex);
}


// Plays animation frame by frame based on the frame length.
void Animator::playAnimation()
{
	// Only plays if specified to animate.
	if (active)
	{
		if (clock.getElapsedTime().asSeconds() >= frameLength)
		{
			if (getFrame() == maxIndex)
				animFinished = true;
			nextFrame();
			clock.restart(); // Restarts clock to 0.
		}
		else
			animFinished = false;
	}
}


// Optional parameter to change the duration of a frame
// before playing the next frame.
void Animator::playAnimation(float newFrameLength)
{
	frameLength = newFrameLength;
	playAnimation();
}


bool Animator::getFinished()
{
	return animFinished;
}


// Sets auto animation.
void Animator::setActive(bool active)
{
	this->active = active;
}


bool Animator::getActive()
{
	return active;
}


int Animator::getFrame()
{
	return currentIndex;
}