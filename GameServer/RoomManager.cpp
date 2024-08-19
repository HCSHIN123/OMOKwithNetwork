#include "pch.h"
#include "RoomManager.h"
#include "SessionManager.h"
#include "ClientPacketHandler.h"

RoomManager::RoomManager()
{
	for (int i = 0; i < ROOM_MAX; ++i)
		m_qRoomId.push(i);
}

RoomManager::~RoomManager()
{
}

void RoomManager::CreateRoom()
{
	if (m_qRoomId.empty())
		return;

	int ID = m_qRoomId.front();
	m_qRoomId.pop();
	m_mapRoom.insert(std::pair(ID, GameRoom()));
	Protocol::S_CREATEROOM packet;
	Protocol::Room* room = packet.mutable_room();
	room->set_roomnumber(ID);
	room->set_count(0);
	SessionManager::GetInstance().BroadCast(ClientPacketHandler::SendPacketTest(packet));
}

bool RoomManager::EnterRoom(int _roomId, std::shared_ptr<GameSession> _pPlayer)
{
	bool isEntered = m_mapRoom[_roomId].Enter(_pPlayer);

	Protocol::S_ENTER enterpacket;

	if (isEntered)
	{
		enterpacket.set_cangame(true);
		SessionManager::GetInstance().Send(_pPlayer, ClientPacketHandler::SendPacketTest(enterpacket));
		if (m_mapRoom[_roomId].GetPlayer(Protocol::WHITE) != nullptr)
		{
			Protocol::S_ENEMYENTER enemypacket;
			enemypacket.set_enter(true);
			SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(enemypacket));
			SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(enemypacket));
		}
	}
	else
	{
		enterpacket.set_cangame(false);
		SessionManager::GetInstance().Send(_pPlayer, ClientPacketHandler::SendPacketTest(enterpacket));
	}

	return isEntered;
}

void RoomManager::ExitRoom(int _roomId, std::shared_ptr<GameSession> _pPlayer)
{
	if (m_mapRoom[_roomId].isStarted())
	{
		Protocol::S_GAMEOVER losepacket;
		losepacket.set_result(Protocol::GAME_OVER::LOSE);
		SessionManager::GetInstance().Send(_pPlayer, ClientPacketHandler::SendPacketTest(losepacket));
		m_mapRoom[_roomId].Exit(_pPlayer);

		Protocol::S_ENEMYENTER enemypacket;
		enemypacket.set_enter(false);
		SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(enemypacket));
		Protocol::S_GAMEOVER winpacket;
		winpacket.set_result(Protocol::GAME_OVER::WIN);
		SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(winpacket));

		m_mapRoom[_roomId].Reset();
	}
	else 
	{
		Protocol::S_GAMEOVER packet;
		packet.set_result(Protocol::GAME_OVER::EXIT);
		SessionManager::GetInstance().Send(_pPlayer, ClientPacketHandler::SendPacketTest(packet));
		m_mapRoom[_roomId].Exit(_pPlayer);
		if (m_mapRoom[_roomId].Count() == 0) // 방에 아무도 남아있지않으면 방파괴
			RemoveRoom(_roomId);
		else // 남은 상대에게 상대가 나갔음을 알림
		{
			m_mapRoom[_roomId].GetPlayer(Protocol::BLACK)->ResetPlayer();
			Protocol::S_ENEMYENTER enemypacket;
			enemypacket.set_enter(false);
			SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(enemypacket));
		}
	}

	BroadCastAllRooms();
}

void RoomManager::RemoveRoom(int _id)
{
	m_qRoomId.push(_id);
	m_mapRoom.erase(_id);
}

void RoomManager::Ready(int _roomId, std::shared_ptr<GameSession> _pPlayer)
{
	if (m_mapRoom[_roomId].CanStartGame())
	{
		Protocol::S_STARTGAME packet1;
		packet1.set_color(Protocol::STONE_COLOR::BLACK);
		m_mapRoom[_roomId].GetPlayer(Protocol::BLACK)->SetColor(Protocol::BLACK);

		Protocol::S_STARTGAME packet2;
		packet2.set_color(Protocol::STONE_COLOR::WHITE);
		m_mapRoom[_roomId].GetPlayer(Protocol::WHITE)->SetColor(Protocol::WHITE);

		SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(packet1));
		SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(packet2));
	}
	else
	{
		Protocol::S_ENEMYREADY packet;

		_pPlayer == m_mapRoom[_roomId].GetPlayer(Protocol::WHITE) ? 
			SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(packet)):
			SessionManager::GetInstance().Send(m_mapRoom[_roomId].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(packet));
	}
}

void RoomManager::PutStone(int _roomID, int _x, int _y, Protocol::STONE_COLOR _color)
{
	if (m_mapRoom.count(_roomID) == 0)
		return;

	bool bGameOver = false;
	bGameOver = m_mapRoom[_roomID].WinCheck({ _x,_y }, _color);
	
	if (bGameOver) // 게임오버
	{
		Protocol::S_GAMEOVER winpacket;
		winpacket.set_result(Protocol::GAME_OVER::WIN);
		Protocol::S_GAMEOVER losepacket;
		losepacket.set_x(_x);
		losepacket.set_y(_y);
		losepacket.set_result(Protocol::GAME_OVER::LOSE);
		if (_color == Protocol::STONE_COLOR::WHITE)
		{
			SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(losepacket));
			SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(winpacket));
		}
		else
		{
			SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(winpacket));
			SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(losepacket));
		}
		m_mapRoom[_roomID].Reset();
	}
	else
	{
		if (m_mapRoom[_roomID].PutStone(_x, _y, _color)) // 돌놓기
		{
			Protocol::S_ENEMYTURN packet;
			packet.set_x(_x);
			packet.set_y(_y);

			_color == Protocol::WHITE ?
				SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::BLACK), ClientPacketHandler::SendPacketTest(packet)) :
				SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(Protocol::WHITE), ClientPacketHandler::SendPacketTest(packet));
		}
		else // 비정상적인 위치에 돌을 놓았을때 턴백
		{
			Protocol::S_TURNBACK packet;
			packet.set_x(_x);
			packet.set_y(_y);
			SessionManager::GetInstance().Send(m_mapRoom[_roomID].GetPlayer(_color), ClientPacketHandler::SendPacketTest(packet));
		}
	}
}

bool RoomManager::Regame(int _roomId)
{
	if (m_mapRoom[_roomId].Count() == 2)
	{
		
		return true;
	}

	return false;
}

void RoomManager::BroadCastAllRooms()
{
	Protocol::S_ENTERLOBBY packet;
	std::map<int, GameRoom>::iterator iter;
	for (iter = m_mapRoom.begin(); iter != m_mapRoom.end(); iter++)
	{
		Protocol::Room* room = packet.add_rooms();
		room->set_roomnumber(iter->first);
		room->set_count(iter->second.Count());
	}

	SessionManager::GetInstance().BroadCast(ClientPacketHandler::SendPacketTest(packet));
}
