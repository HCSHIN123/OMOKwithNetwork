#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameSession.h"

std::function<bool(std::shared_ptr<PacketSession>&, Byte*, int)> s_PacketHandlerList[UINT16_MAX];

bool Handle_Invalid(std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
{
	return false;
}


bool Handle_C_REGAME(std::shared_ptr<PacketSession>& _session, Protocol::C_REGAME& packet)
{
	static_pointer_cast<GameSession>(_session)->Regame();
	return true;
}

bool Handle_C_ENDGAME(std::shared_ptr<PacketSession>& _session, Protocol::C_ENDGAME& packet)
{
	static_pointer_cast<GameSession>(_session)->EndGame(packet.roomid());
	return true;
}

bool Handle_C_CREATEROOM(std::shared_ptr<PacketSession>& _session, Protocol::C_CREATEROOM& packet)
{
	static_pointer_cast<GameSession>(_session)->CreateRoom();

	return true;
}

bool Handle_C_ENTERLOBBY(std::shared_ptr<PacketSession>& _session, Protocol::C_ENTERLOBBY& packet)
{
	static_pointer_cast<GameSession>(_session)->EnterLobby();

	return true;
}

bool Handle_C_ENTER(std::shared_ptr<PacketSession>& _session, Protocol::C_ENTER& packet)
{
	static_pointer_cast<GameSession>(_session)->EnterRoom(packet.roomid());
	return true;
}

bool Handle_C_MYTURN(std::shared_ptr<PacketSession>& _session, Protocol::C_MYTURN& packet)
{
	static_pointer_cast<GameSession>(_session)->MyTurn(packet.x(), packet.y());
	return false;
}

bool Handle_C_READY(std::shared_ptr<PacketSession>& _session, Protocol::C_READY& packet)
{
	static_pointer_cast<GameSession>(_session)->Ready(packet.ready());
	return false;
}

bool Handle_C_EXITROOM(std::shared_ptr<PacketSession>& _session, Protocol::C_EXITROOM& packet)
{
	static_pointer_cast<GameSession>(_session)->ExitRoom();
	return false;
}

