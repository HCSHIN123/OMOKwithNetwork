#pragma once
#include "test.pb.h"
class Texture;
class Stone
{
private:
	Vector2 m_BoardPosition;
	bool m_bClick;
	bool m_bCursorOn;
	RECT	m_Rect;
	Protocol::STONE_COLOR m_eColor;
	Texture* m_pTexture;
	std::function<void(Vector2)> m_funcWinCheck;
public:
	Stone();
	void Init(Vector2 _pos, std::function<void(Vector2)> _func);
	void Reset();
    void Render(HDC _hdc);
	bool isCursorInRect(POINT _pt);
	void Click(Protocol::STONE_COLOR _color);
	void CursorOn(Protocol::STONE_COLOR _color);
	Protocol::STONE_COLOR GetStoneColor() { return m_eColor; };
	void SetStoneColor(Protocol::STONE_COLOR _color) { m_eColor = _color; };
};

