#pragma once
enum
{
	IDX_P1 = 0,
	IDX_P2 = 1,
	BOARD_HEIGHT = 15,
	BOARD_WIDTH = 15,
	ROOM_MAX = 5,
};

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

