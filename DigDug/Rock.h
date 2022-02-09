#pragma once
class Rock
{
public:
	Rock();
	~Rock();

	void fall();
	bool getIsFalling();

	void update();

private:
	bool isFalling;
};

