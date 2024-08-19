#include "pch.h"
#include "GameBoard.h"
#include "InputManager.h"
#include "GameCore.h"
#include "NetworkManager.h"
#include "ServerPacketHandler.h"

GameBoard::GameBoard()
{
	m_eMyColor = Protocol::STONE_COLOR::NONE;
	m_bMyTurn = false;
	m_bGameEnd = false;
}

GameBoard::~GameBoard()
{
}

void GameBoard::Init()
{
	InitStone();
}

void GameBoard::InitStone()
{
	for (int y = 0; y < BOARD_SCALE::HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_SCALE::WIDTH; ++x)
		{
			m_Board[y][x].Init({x,y}, std::bind(&GameBoard::WinCheck, this, std::placeholders::_1));
		}
	}
}

void GameBoard::Reset()
{
	m_eMyColor = Protocol::STONE_COLOR::NONE;
	m_bMyTurn = false;
	m_bGameEnd = false;
	for (int y = 0; y < BOARD_SCALE::HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_SCALE::WIDTH; ++x)
		{
			m_Board[y][x].Reset();
		}
	}
}

void GameBoard::Update()
{
	if (!is_Activated() || m_bGameEnd)
		return;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(GameCore::GetInstance()->GethWnd(), &ptMouse);

	if (m_bMyTurn )
	{
		StoneCheck(ptMouse, InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::UP);
	}
}

void GameBoard::StoneCheck(POINT _pt, bool _isClick)
{
	for (int y = 0; y < BOARD_SCALE::HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_SCALE::WIDTH; ++x)
		{
			if (m_Board[y][x].isCursorInRect(_pt))
			{
				_isClick ? m_Board[y][x].Click(m_eMyColor) : m_Board[y][x].CursorOn(m_eMyColor);
				return;
			}
		}
	}
}

void GameBoard::WinCheck(Vector2 _pos)
{
	if (m_bMyTurn)
	{
		Protocol::C_MYTURN packet;
		packet.set_x(_pos.x);
		packet.set_y(_pos.y);
		NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
	}
	
	m_bMyTurn = !m_bMyTurn;
}

int GameBoard::LineCheck(Vector2 _pos, Vector2 _dir)
{
	int cnt = 0;
	for (Vector2 pos = _pos + _dir; pos.x >= 0 && pos.y >= 0 && pos.x < WIDTH && pos.y < HEIGHT; pos += _dir)
	{
		if (m_Board[pos.y][pos.x].GetStoneColor() != m_eMyColor)
			return cnt;
		cnt++;

	}

	return cnt;
}

void GameBoard::EnemyInput(Vector2 _pos)
{
	m_Board[_pos.y][_pos.x].Click(m_eEnemyColor);
}

void GameBoard::TurnBack(Vector2 _pos)
{
	m_Board[_pos.y][_pos.x].SetStoneColor(Protocol::NONE);
	m_bMyTurn = true;
}



void GameBoard::StartGame(Protocol::STONE_COLOR _color)
{
	m_eMyColor = _color;
	if (_color == Protocol::STONE_COLOR::BLACK)
	{
		m_bMyTurn = true;
		m_eEnemyColor = Protocol::STONE_COLOR::WHITE;
	}
	else
	{
		m_bMyTurn = false;
		m_eEnemyColor = Protocol::STONE_COLOR::BLACK;
	}
}

void GameBoard::Render(HDC _memDC)
{
	DrawBoard(_memDC);
	for (int y = 0; y < BOARD_SCALE::HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_SCALE::WIDTH; ++x)
		{
			m_Board[y][x].Render(_memDC);
		}
	}
}

void GameBoard::DrawBoard(HDC _memDC)
{
	for (int x = 0; x < BOARD_SCALE::WIDTH; x++)
	{
		MoveToEx(_memDC, BOARD_SCALE::START + x * BOARD_SCALE::INTERVAL, BOARD_SCALE::START, NULL);
		LineTo(_memDC, BOARD_SCALE::START + x * BOARD_SCALE::INTERVAL,
			BOARD_SCALE::START + (BOARD_SCALE::WIDTH - 1) * BOARD_SCALE::INTERVAL);
	}
	for (int y = 0; y < BOARD_SCALE::HEIGHT; y++)
	{
		MoveToEx(_memDC, BOARD_SCALE::START, BOARD_SCALE::START + y * BOARD_SCALE::INTERVAL, NULL);
		LineTo(_memDC, BOARD_SCALE::START + (BOARD_SCALE::HEIGHT - 1) * BOARD_SCALE::INTERVAL,
			BOARD_SCALE::START + y * BOARD_SCALE::INTERVAL);
	}
}