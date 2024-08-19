#include "pch.h"
#include "GameSession.h"
#include "SessionManager.h"
#include "ClientPacketHandler.h"
#include "RoomManager.h"

GameSession::GameSession()
{
	m_iID = -1;
	m_iCurRoomID = -1;
	m_bReady = false;
	m_eColor = Protocol::STONE_COLOR::NONE;
}

GameSession::~GameSession()
{
	std::cout << "Destructor Game Session" << std::endl;
	Protocol::S_DISCONNECT packet;
	packet.set_id(m_iID);
	SessionManager::GetInstance().BroadCast(ClientPacketHandler::SendPacketTest(packet));
}

void GameSession::OnConnected()
{
	static int ID = 0;
	SessionManager::GetInstance().Insert(static_pointer_cast<GameSession>(shared_from_this()));

	m_iID = ID++;

	Protocol::S_CONNECT packet;
	packet.set_id(m_iID);
	for (auto session : SessionManager::GetInstance().GetSessionSet())
	{
		Protocol::Player* player = packet.add_players();
		session->SetPacket(player);
	}
	SessionManager::GetInstance().BroadCast(ClientPacketHandler::SendPacketTest(packet));
}

void GameSession::OnDisconnected()
{
	Protocol::S_ENDGAME packet;
	
	ExitRoom();
	SessionManager::GetInstance().Erase(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(Byte* _buff, int _size)
{
	std::shared_ptr<PacketSession> session = GetPacketSessionPtr();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_buff);
	ClientPacketHandler::PacketHandle(session, _buff, _size);
}

void GameSession::OnSend(int Length)
{
	std::cout << "OnSend Length : " << Length << std::endl;
}

void GameSession::CreateRoom()
{
	RoomManager::GetInstance().CreateRoom();
}

void GameSession::EnterLobby()
{
	RoomManager::GetInstance().BroadCastAllRooms();
}

void GameSession::EnterRoom(int _roomID)
{
	
	if (RoomManager::GetInstance().EnterRoom(_roomID, static_pointer_cast<GameSession>(shared_from_this())))
	{
		RoomManager::GetInstance().BroadCastAllRooms();
		m_iCurRoomID = _roomID;
		m_bReady = false;
	}
}

void GameSession::ExitRoom()
{
	if (m_iCurRoomID == -1)
		return;
	RoomManager::GetInstance().ExitRoom(m_iCurRoomID, static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::EndGame(int _roomID)
{
	Protocol::S_ENDGAME packet;
	packet.set_roomid(_roomID);
	ExitRoom();
}

void GameSession::MyTurn(int _x, int _y)
{
	RoomManager::GetInstance().PutStone(m_iCurRoomID, _x, _y, m_eColor);
}

void GameSession::Ready(bool _ready)
{  
	m_bReady = _ready;
	RoomManager::GetInstance().Ready(m_iCurRoomID, static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::SetPacket(Protocol::Player* _player)
{
	_player->set_id(m_iID);
}

void GameSession::ResetRoom()
{
	m_iCurRoomID = -1;
	m_bReady = false;
	m_eColor = Protocol::NONE;
}

void GameSession::ResetPlayer()
{
	m_bReady = false;
	m_eColor = Protocol::NONE;
}

void GameSession::Regame()
{
	Protocol::S_ENEMYENTER packet;
	if (!RoomManager::GetInstance().Regame(m_iCurRoomID))
	{
		packet.set_enter(false);
		SessionManager::GetInstance().Send(static_pointer_cast<GameSession>(shared_from_this()), ClientPacketHandler::SendPacketTest(packet));
	}
	else
	{
		packet.set_enter(true);
		SessionManager::GetInstance().Send(static_pointer_cast<GameSession>(shared_from_this()), ClientPacketHandler::SendPacketTest(packet));
	}
}


