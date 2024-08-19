#pragma once
#include "Session.h"
struct PacketHeader
{
	//전달된 Buff크기
	unsigned short m_iSize;
	//전달된 속성의 종류
	unsigned short m_iProtocolID;
};

//TCP통신은 데이터의 Boundary가 없어 다른 속성의 데이터까지 같이 넘어올 수 있으므로
//어디까지가 하나의 속성인지를 구분하기위해 PacketHeader를 전송될 Buffer에 추가하여
//전송된 한 속성의 Buffer크기를 함께 전달하여 사용한다.
class PacketSession : public Session
{
protected:
	virtual int OnRecv(Byte* _buffer, int _length) final;
	virtual void OnRecvPacket(Byte* _buffer, int _length) abstract;
public:
	PacketSession();
	virtual ~PacketSession();
	std::shared_ptr<PacketSession> GetPacketSessionPtr() { return static_pointer_cast<PacketSession>(shared_from_this()); }
};

