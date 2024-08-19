#pragma once

struct Vector2
{
	int x;
	int y;
	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Vector2 operator + (Vector2 _vec2)
	{
		Vector2 value;
		value.x = x + _vec2.x;
		value.y = y + _vec2.y;
		return value;
	}
	void operator+=(Vector2 _vec2)
	{
		x += _vec2.x;
		y += _vec2.y;
	}
};


struct Player
{
	int m_iID;
	int m_iRadius;
	Vector2 m_Position;
};

struct Room
{
	int m_iRoomID;
	int m_iCount;
};
enum BOARD_SCALE
{
	START = 32,
	INTERVAL = 36,
	WIDTH = 15,
	HEIGHT = 15,
};

