#include "pch.h"
#include "PacketSession.h"

int PacketSession::OnRecv(Byte* _buffer, int _length)
{
    //전달받은 버퍼 조립
    int iProcessLength = 0;
    while (true)
    {
        int iDataSize = _length - iProcessLength;
        //받은 데이터가 최소 헤더의 크기 이상은 되어야 한다.
        if (sizeof(PacketHeader) > iDataSize)
            break;
        //Buff를 PacketHeader로 변환 한뒤 실제 전달된 데이터양이 지금 읽어드릴 데이터량 보다 작거나 같아야한다.
        PacketHeader* Header = reinterpret_cast<PacketHeader*>(&_buffer[iProcessLength]);
        if (Header->m_iSize > iDataSize)
            break;
        //Header에 기록된 만큼만 Recv 한다.
        OnRecvPacket(&_buffer[iProcessLength], Header->m_iSize);
        //읽어드린 만큼 다음 읽을 위치로 이동 한다.
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
