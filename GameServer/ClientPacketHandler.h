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
bool Handle_C_REGAME(std::shared_ptr<PacketSession>& _session, Protocol::C_REGAME& packet);
bool Handle_C_EXITROOM(std::shared_ptr<PacketSession>& _session, Protocol::C_EXITROOM& packet);
bool Handle_C_READY(std::shared_ptr<PacketSession>& _session, Protocol::C_READY& packet);
bool Handle_C_ENDGAME(std::shared_ptr<PacketSession>& _session, Protocol::C_ENDGAME& packet);
bool Handle_C_ENTER(std::shared_ptr<PacketSession>& _session, Protocol::C_ENTER& packet);
bool Handle_C_MYTURN(std::shared_ptr<PacketSession>& _session, Protocol::C_MYTURN& packet);
bool Handle_C_CREATEROOM(std::shared_ptr<PacketSession>& _session, Protocol::C_CREATEROOM& packet);
bool Handle_C_ENTERLOBBY(std::shared_ptr<PacketSession>& _session, Protocol::C_ENTERLOBBY& packet);
class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			s_PacketHandlerList[i] = Handle_Invalid;
		s_PacketHandlerList[SERVER_PROTOCOL::C_REGAME] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_REGAME >(Handle_C_REGAME, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_EXITROOM] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_EXITROOM >(Handle_C_EXITROOM, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_READY] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_READY >(Handle_C_READY, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_ENDGAME] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_ENDGAME >(Handle_C_ENDGAME, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_ENTER] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_ENTER >(Handle_C_ENTER, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_MYTURN] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_MYTURN >(Handle_C_MYTURN, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_CREATEROOM] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_CREATEROOM >(Handle_C_CREATEROOM, _session, _buff, _size);
		};
		s_PacketHandlerList[SERVER_PROTOCOL::C_ENTERLOBBY] =
			[](std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
		{
			return PacketHandle<Protocol::C_ENTERLOBBY >(Handle_C_ENTERLOBBY, _session, _buff, _size);
		};
	}
	static bool PacketHandle(std::shared_ptr<PacketSession>& _session, Byte* _buff, int _size)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buff);
		return s_PacketHandlerList[header->m_iProtocolID](_session, _buff, _size);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENDGAME& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENDGAME);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_CREATEROOM& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_CREATEROOM);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENTERLOBBY& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENTERLOBBY);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_DISCONNECT& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_DISCONNECT);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_CONNECT& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_CONNECT);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENTER& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENTER);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_STARTGAME& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_STARTGAME);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENEMYTURN& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENEMYTURN);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_GAMEOVER& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_GAMEOVER);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_TURNBACK& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_TURNBACK);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENEMYENTER& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENEMYENTER);
	}
	static std::shared_ptr<SendBuffer> SendPacketTest(Protocol::S_ENEMYREADY& _packet)
	{
		return CreateSendBuff(_packet, SERVER_PROTOCOL::S_ENEMYREADY);
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
