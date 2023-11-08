#pragma once
class Blocks
{
	int x;
	int y;
	bool onPlayer1;
	bool onPlayer2;
public:
	Blocks();
	void setCord(int x, int y);

	int getX();
	int getY();
};

