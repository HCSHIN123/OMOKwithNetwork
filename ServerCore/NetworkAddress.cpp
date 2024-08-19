#include "pch.h"
#include "NetworkAddress.h"

NetworkAddress::NetworkAddress()
	:m_SockAddress{}
{
}

NetworkAddress::NetworkAddress(SOCKADDR_IN _socketAddress)
	:m_SockAddress(_socketAddress)
{
}

NetworkAddress::NetworkAddress(std::wstring ip, unsigned short port)
{
	memset(&m_SockAddress, 0, sizeof(m_SockAddress));
	//IPv4주소체계로 지정
	m_SockAddress.sin_family = AF_INET;
	//IPv4를 이진주소로 변환한다.
	m_SockAddress.sin_addr = IPv4ToBinaryAddress(ip.c_str());
	//shot형 port번호를 Host방식에서 Network방식으로 변경한다.
	//Host방식 : Little Endian or Big Endian
	//Network방식 : Big Endian
	m_SockAddress.sin_port = htons(port);
}

std::wstring NetworkAddress::GetIPAddress()
{
	WCHAR Buff[256];
	InetNtopW(AF_INET, &m_SockAddress.sin_addr, Buff, sizeof(Buff) / sizeof(Buff[0]));
	return std::wstring(Buff);
}

IN_ADDR NetworkAddress::IPv4ToBinaryAddress(const WCHAR* ip)
{
	IN_ADDR Address;
	InetPtonW(AF_INET, ip, &Address);
	return Address;
}
