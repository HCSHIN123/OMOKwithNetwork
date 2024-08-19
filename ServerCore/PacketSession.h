#pragma once
#include "Session.h"
struct PacketHeader
{
	//���޵� Buffũ��
	unsigned short m_iSize;
	//���޵� �Ӽ��� ����
	unsigned short m_iProtocolID;
};

//TCP����� �������� Boundary�� ���� �ٸ� �Ӽ��� �����ͱ��� ���� �Ѿ�� �� �����Ƿ�
//�������� �ϳ��� �Ӽ������� �����ϱ����� PacketHeader�� ���۵� Buffer�� �߰��Ͽ�
//���۵� �� �Ӽ��� Bufferũ�⸦ �Բ� �����Ͽ� ����Ѵ�.
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

