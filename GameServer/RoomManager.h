#pragma once
#include "pch.h"
#include "test.pb.h"
#include "GameSession.h"

struct GameRoom
{
	std::shared_ptr<GameSession> m_Players[2]; 
	Protocol::STONE_COLOR  m_eBoard[BOARD_HEIGHT][BOARD_WIDTH];
	int idx = 0;

	std::shared_ptr<GameSession> GetPlayer(Protocol::STONE_COLOR _color)
	{
		if (m_Players[_color] != nullptr)
			return m_Players[_color];

		return nullptr;
	}

	bool Enter(std::shared_ptr<GameSession> _player)
	{
		if (Count() == 2)
			return false;
		m_Players[idx++] = (_player);
		return true;
	}

	void Exit(std::shared_ptr<GameSession> _player)
	{
		if (m_Players[Protocol::STONE_COLOR::BLACK] == _player)
		{
			if (m_Players[Protocol::STONE_COLOR::WHITE] != nullptr)
			{
				m_Players[Protocol::STONE_COLOR::BLACK] = m_Players[Protocol::STONE_COLOR::WHITE];
				m_Players[Protocol::STONE_COLOR::WHITE] = nullptr;
			}
			else
				m_Players[Protocol::STONE_COLOR::BLACK] = nullptr;
			
			idx--;
		}
		else
		{
			m_Players[Protocol::STONE_COLOR::WHITE] = nullptr;
			idx--;
		}
		_player->ResetPlayer();
	}

	bool CanStartGame()
	{
		if (Count() < 2)
			return false;
		return m_Players[Protocol::STONE_COLOR::BLACK]->isReady() && m_Players[Protocol::STONE_COLOR::WHITE]->isReady();
	}

	bool PutStone(int _x, int _y, Protocol::STONE_COLOR _color)
	{
		if (m_eBoard[_y][_x] != Protocol::STONE_COLOR::NONE)
			return false;
		
		m_eBoard[_y][_x] = _color;
		return true;
	}

	bool isStarted()
	{
		return (Count() == 2 && m_Players[Protocol::STONE_COLOR::BLACK]->isReady() && m_Players[Protocol::STONE_COLOR::WHITE]->isReady());
	}

	bool WinCheck(Vector2 _pos, Protocol::STONE_COLOR _color)
	{
		if (m_eBoard[_pos.y][_pos.x] != Protocol::STONE_COLOR::NONE)
			return false;

		if (LineCheck(_pos, { 0, 1 }, _color) + LineCheck(_pos, { 0, -1 }, _color) == 4)
			return true;
		else if (LineCheck(_pos, { -1, 1 }, _color) + LineCheck(_pos, { 1, -1 }, _color) == 4)
			return true;
		else if (LineCheck(_pos, { -1, 0 }, _color) + LineCheck(_pos, { 1, 0 }, _color) == 4)
			return true;
		else if (LineCheck(_pos, { -1, -1 }, _color) + LineCheck(_pos, { 1, 1 }, _color) == 4)
			return true;

		return false;
	}
	int LineCheck(Vector2 _pos, Vector2 _dir, Protocol::STONE_COLOR _color)
	{
		int cnt = 0;
		for (Vector2 pos = _pos + _dir; pos.x >= 0 && pos.y >= 0 && pos.x < BOARD_WIDTH && pos.y < BOARD_HEIGHT; pos += _dir)
		{
			if (m_eBoard[pos.y][pos.x] != _color)
				return cnt;
			cnt++;
		}

		return cnt;
	}

	int Count()
	{
		return idx;
	}

	GameRoom()
	{
		Init();
	}

	void Reset()
	{
		if (m_Players[Protocol::BLACK] != nullptr)
			m_Players[Protocol::BLACK]->ResetPlayer();

		if (m_Players[Protocol::WHITE] != nullptr)
			m_Players[Protocol::WHITE]->ResetPlayer();

		for (int y = 0; y < BOARD_HEIGHT; ++y)
		{
			for (int x = 0; x < BOARD_WIDTH; ++x)
			{
				m_eBoard[y][x] = Protocol::STONE_COLOR::NONE;
			}
		}
	}

	void Init()
	{
		idx = 0;
		Reset();
	}
};

class RoomManager
{
private:
	std::map<int, GameRoom> m_mapRoom; // 积己等 规 甘
	std::queue<int> m_qRoomId;
public:
	RoomManager();
	~RoomManager();
	void CreateRoom();
	bool EnterRoom(int _roomId, std::shared_ptr<GameSession> _pPlayer);
	void ExitRoom(int _roomId, std::shared_ptr<GameSession> _pPlayer);
	void RemoveRoom(int _id);
	void Ready(int _roomId, std::shared_ptr<GameSession> _pPlayer);
	void PutStone(int _roomID, int _x, int _y, Protocol::STONE_COLOR _color);
	bool Regame(int _roomId);

	void BroadCastAllRooms();

	static RoomManager& GetInstance()
	{
		static RoomManager Instance;
		return Instance;
	}

};

