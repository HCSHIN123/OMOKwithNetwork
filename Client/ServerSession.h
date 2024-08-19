#pragma once
#include"pch.h"
#include"ServerPacketHandler.h"

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		std::cout << "Destructor Server Session" << std::endl;
	}
	virtual void OnConnected()override
	{
		//char Buff[] = "Hello Server!!";
		//std::cout << "Connected Server!" << std::endl;
		//std::shared_ptr<SendBuffer> sendBuffer = Core::GetInstance().GetSendBufferManager()->GetSendBuffer(reinterpret_cast<Byte*>(Buff), sizeof(Buff));
		//Send(sendBuffer);
	}
	virtual void OnDisconnected() override
	{
		std::cout << "Disconnected" << std::endl;
	}
	virtual void OnRecvPacket(Byte* _buff, int _size) override
	{
		std::shared_ptr<PacketSession> session = GetPacketSessionPtr();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buff);
		ServerPacketHandler::PacketHandle(session, _buff, _size);
	}
	virtual void OnSend(int Length) override
	{
		std::cout << "OnSend Length : " << Length << std::endl;
	}
};