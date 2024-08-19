#pragma once
#include "test.pb.h"

extern std::function<bool(std::shared_ptr<PacketSession>&, Byte*, int)> s_PacketHandlerList[UINT16_MAX];
enum SERVER_PROTOCOL
{
	C_REGAME = 100,
	C_EXITROOM = 101,
	C_READY = 102,
	C_ENDGAME = 103,
	C_ENTER = 104,
	C_MYTURN = 105,
	C_CREATEROOM = 106,
	C_ENTERLOBBY = 107,
	S_ENDGAME = 108,
	S_CREATEROOM = 109,
	S_ENTERLOBBY = 110,
	S_DISCONNECT = 111,
	S_CONNECT = 112,
	S_ENTER = 113,
	S_STARTGAME = 114,
	S_ENEMYTURN = 115,
	S_GAMEOVER = 116,
	S_TURNBACK = 117,
	S_ENEMYENTER = 118,
	S_ENEMYREADY = 119,
};
bool Handle_Invalid(std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size);
bool Handle_S_ENDGAME(std::shared_ptr<PacketSession>& _session, Protocol::S_ENDGAME& packet);
bool Handle_S_CREATEROOM(std::shared_ptr<PacketSession>& _session, Protocol::S_CREATEROOM& packet);
bool Handle_S_ENTERLOBBY(std::shared_ptr<PacketSession>& _session, Protocol::S_ENTERLOBBY& packet);
bool Handle_S_DISCONNECT(std::shared_ptr<PacketSession>& _session, Protocol::S_DISCONNECT& packet);
bool Handle_S_CONNECT(std::shared_ptr<PacketSession>& _session, Protocol::S_CONNECT& packet);
bool Handle_S_ENTER(std::shared_ptr<PacketSession>& _session, Protocol::S_ENTER& packet);
bool Handle_S_STARTGAME(std::shared_ptr<PacketSession>& _session, Protocol::S_STARTGAME& packet);
bool Handle_S_ENEMYTURN(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYTURN& packet);
bool Handle_S_GAMEOVER(std::shared_ptr<PacketSession>& _session, Protocol::S_GAMEOVER& packet);
bool Handle_S_TURNBACK(std::shared_ptr<PacketSession>& _session, Protocol::S_TURNBACK& packet);
bool Handle_S_ENEMYENTER(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYENTER& packet);
bool Handle_S_ENEMYREADY(std::shared_ptr<PacketSession>& _session, Protocol::S_ENEMYREADY& packet);
class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			s_PacketHandlerList[i] = Handle_Invalid;
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENDGAME] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENDGAME >(Handle_S_ENDGAME, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_CREATEROOM] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_CREATEROOM >(Handle_S_CREATEROOM, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENTERLOBBY] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENTERLOBBY >(Handle_S_ENTERLOBBY, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_DISCONNECT] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_DISCONNECT >(Handle_S_DISCONNECT, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_CONNECT] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_CONNECT >(Handle_S_CONNECT, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENTER] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENTER >(Handle_S_ENTER, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_STARTGAME] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_STARTGAME >(Handle_S_STARTGAME, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENEMYTURN] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENEMYTURN >(Handle_S_ENEMYTURN, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_GAMEOVER] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_GAMEOVER >(Handle_S_GAMEOVER, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_TURNBACK] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_TURNBACK >(Handle_S_TURNBACK, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENEMYENTER] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENEMYENTER >(Handle_S_ENEMYENTER, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::S_ENEMYREADY] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::S_ENEMYREADY >(Handle_S_ENEMYREADY, _session, _buff, _size);
		};
	}
	static bool PacketHandle(std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buff);
		return s_PacketHandlerList[header->m_iProtocolID](_session, _buff, _size);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_REGAME& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_REGAME);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_EXITROOM& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_EXITROOM);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_READY& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_READY);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_ENDGAME& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_ENDGAME);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_ENTER& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_ENTER);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_MYTURN& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_MYTURN);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_CREATEROOM& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_CREATEROOM);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::C_ENTERLOBBY& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::C_ENTERLOBBY);
	}
private:
	template<typename PacketType, typename CallBackFunc>
	static bool PacketHandle(CallBackFunc _func, std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
	{
		PacketType packet;
		if (packet.ParseFromArray(_buff + sizeof(PacketHeader), _size - sizeof(PacketHeader)) == false)
			return false;
		return _func(_session, packet);
	}

	template<typename type>
	static std::shared_ptr<SendBuffer> CreateSendBuff(type& _packet, unsigned short _packetID)
	{
		//buff크기
		unsigned short buffSize = static_cast<unsigned short>(_packet.ByteSizeLong());
		//header포함 총 크기
		unsigned short packetSize = buffSize + sizeof(PacketHeader);

		//sendbuffer 메모리 할당
		std::shared_ptr<SendBuffer> sendBuffer = Core::GetInstance().GetSendBufferManager()->GetSendBuffer(packetSize);

		//header 셋팅
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->GetBuff());
		header->m_iProtocolID = _packetID;
		header->m_iSize = packetSize;
		//&header[1] : header주소에 PacketHeader크기만큼의 주소 덧셈을 한번 하면 본문이 나온다.
		assert(_packet.SerializeToArray(&header[1], buffSize));

		sendBuffer->UseBuff(packetSize);

		return sendBuffer;
	}
};
