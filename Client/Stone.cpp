#include "pch.h"
#include "Stone.h"
#include"InputManager.h"
#include "GameCore.h"
#include "ResourceManager.h"

Stone::Stone()
{
	Reset();
}

void Stone::Init(Vector2 _pos, std::function<void(Vector2)> _func)
{
	Reset();

	m_funcWinCheck = _func;

	m_BoardPosition = _pos;
	
	m_Rect.left = START + m_BoardPosition.x * BOARD_SCALE::INTERVAL - BOARD_SCALE::INTERVAL / 2;
	m_Rect.top = START + m_BoardPosition.y * BOARD_SCALE::INTERVAL - BOARD_SCALE::INTERVAL / 2;
	m_Rect.right = m_Rect.left + BOARD_SCALE::INTERVAL;
	m_Rect.bottom = m_Rect.top + BOARD_SCALE::INTERVAL;
}

void Stone::Reset()
{
	m_bCursorOn = false;
	m_bClick = false;
	m_eColor = Protocol::STONE_COLOR::NONE;
	m_pTexture = nullptr;
}

void Stone::Render(HDC _hdc)
{

	if (!m_bClick && !m_bCursorOn)
		return;
	
	TransparentBlt(_hdc, m_Rect.left, m_Rect.top, m_pTexture->GetWidth(), m_pTexture->GetHeight(), m_pTexture->GetDC(),	
		0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight(), RGB(255, 0, 255));
	
	m_bCursorOn = false;
}

bool Stone::isCursorInRect(POINT _pt)
{
	return PtInRect(&m_Rect, _pt);
}

void Stone::Click(Protocol::STONE_COLOR _color)
{
	if (m_bClick)
		return;
	m_pTexture = _color == Protocol::STONE_COLOR::BLACK ?
		ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BLACK_STONE) : ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::WHITE_STONE);
	m_eColor = _color;
	m_bClick = true;

	m_funcWinCheck(m_BoardPosition);
}

void Stone::CursorOn(Protocol::STONE_COLOR _color)
{
	if (m_bClick)
		return;
	m_pTexture = _color == Protocol::STONE_COLOR::BLACK ?
		ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BLACK_STONE) : ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::WHITE_STONE);
	m_bCursorOn = true;
}

