#include "pch.h"
#include "PacketSession.h"

int PacketSession::OnRecv(Byte* _buffer, int _length)
{
    //���޹��� ���� ����
    int iProcessLength = 0;
    while (true)
    {
        int iDataSize = _length - iProcessLength;
        //���� �����Ͱ� �ּ� ����� ũ�� �̻��� �Ǿ�� �Ѵ�.
        if (sizeof(PacketHeader) > iDataSize)
            break;
        //Buff�� PacketHeader�� ��ȯ �ѵ� ���� ���޵� �����;��� ���� �о�帱 �����ͷ� ���� �۰ų� ���ƾ��Ѵ�.
        PacketHeader* Header = reinterpret_cast<PacketHeader*>(&_buffer[iProcessLength]);
        if (Header->m_iSize > iDataSize)
            break;
        //Header�� ��ϵ� ��ŭ�� Recv �Ѵ�.
        OnRecvPacket(&_buffer[iProcessLength], Header->m_iSize);
        //�о�帰 ��ŭ ���� ���� ��ġ�� �̵� �Ѵ�.
        iProcessLength += Header->m_iSize;
    }
    return iProcessLength;
}

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}
