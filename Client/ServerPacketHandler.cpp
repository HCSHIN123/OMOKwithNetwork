#include "pch.h"
#include "ServerPacketHandler.h"
#include "GameCore.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "SceneManager.h"

template<typename type>
bool HandleGameEvent(std::function<void(type*)> _func)
{
	type* curScene = nullptr;
	curScene = dynamic_cast<type*>(SceneManager::GetInstance()->GetCurScene());
	if (curScene == nullptr)
		return false;
	_func(curScene);
	return true;
}

std::function<bool(std::shared_ptr<PacketSession>&, Byte*, int)> s_PacketHandlerList[UINT16_MAX];

bool Handle_Invalid(std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
{
	return false;
}

bool Handle_S_ENDGAME(std::shared_ptr<PacketSession>& _session, Protocol::S_ENDGAME& packet)
{
	return false;
}


bool Handle_S_DISCONNECT(std::shared_ptr<PacketSession>& _session, Protocol::S_DISCONNECT& packet)
{
	GameCore::GetInstance()->DisConnectPlayer(packet.id());
	return false;
}
bool Handle_S_CONNECT(std::shared_ptr<PacketSession>& _session, Protocol::S_CONNECT& packet)
{
	GameCore::GetInstance()->RegistID(packet.id());
	return true;
}

bool Handle_S_ENTER(std::shared_ptr<PacketSession>& _session, Protocol::S_ENTER& packet)
{
	return HandleGameEvent<LobbyScene>([&packet](LobbyScene* lobbyScene)
		{
			lobbyScene->EnterRoom(packet.cangame());
		});
}
bool Handle_S_CREATEROOM(std::shared_ptr<PacketSession>& _session, Protocol::S_CREATEROOM& packet)
{
	return HandleGameEvent<LobbyScene>([&packet](LobbyScene* lobbyScene)
		{
			Room room;
			room.m_iRoomID = packet.room().roomnumber();
			room.m_iCount = packet.room().count();

			lobbyScene->ApplyNerRoom(room);
		});
}

bool Handle_S_ENTERLOBBY(std::shared_ptr<PacketSession>& _session, Protocol::S_ENTERLOBBY& packet)
{
	return HandleGameEvent<LobbyScene>([&packet](LobbyScene* lobbyScene)
		{
			std::list<Room> rooms;

			for (int i = 0; i < packet.rooms().size(); i++)
			{
				Room room;
				room.m_iRoomID = packet.rooms()[i].roomnumber();
				room.m_iCount = packet.rooms()[i].count();
				rooms.push_back(room);
			}

			lobbyScene->UpdateRoom(rooms);
		});
}

bool Handle_S_STARTGAME(std::shared_ptr<PacketSession>& _session, Protocol::S_STARTGAME& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->StartGame(packet.color());
		});
}

bool Handle_S_ENEMYTURN(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYTURN& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->EnemyInput({ packet.x(), packet.y() });
		});
}

bool Handle_S_GAMEOVER(std::shared_ptr<PacketSession>& _session, Protocol::S_GAMEOVER& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->EndGame(packet.result(),{packet.x(), packet.y()});
		});
}

bool Handle_S_TURNBACK(std::shared_ptr<PacketSession>& _session, Protocol::S_TURNBACK& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->TurnBack({ packet.x(), packet.y() });
		});
}

bool Handle_S_ENEMYENTER(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYENTER& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->EnemyEnter({ packet.enter()});
		});
}

bool Handle_S_ENEMYREADY(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYREADY& packet)
{
	return HandleGameEvent<GameScene>([&packet](GameScene* gameScene)
		{
			gameScene->EnemyReady();
		});
}




