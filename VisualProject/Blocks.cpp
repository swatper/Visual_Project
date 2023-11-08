#include "pch.h"
#include "Blocks.h"

Blocks::Blocks()
{
	this->x = 50;
	this->y = 50;
	onPlayer1 = false;
	onPlayer2 = false;
}

void Blocks::setCord(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Blocks::getX()
{
	return x;
}

int Blocks::getY()
{
	return y;
}