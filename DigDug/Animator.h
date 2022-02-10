#pragma once
#include "Spritesheet.h"

class Animator
{
public:
	Animator(Spritesheet* sprite);
	~Animator();
	// Sets the animation to the frames it should run through
	// and the duration it should take for each frame to change.
	void setAnimation(int minIndex, int maxIndex,
		float frameLength, bool active);
	void nextFrame();
	void playAnimation();
	void playAnimation(float newFrameLength);

	void setActive(bool active);
	bool getActive();
	int getFrame();

private:
	Spritesheet* sprite; 
	sf::Clock clock;

	int maxIndex; // sprite index that is start of animation.
	int minIndex; // sprite index that is the end of animation.
	int currentIndex; // sprite index that animation is currently on.
	float frameLength; // how long each frame lasts for until updating.
	bool active; // runs animation if true.
};

