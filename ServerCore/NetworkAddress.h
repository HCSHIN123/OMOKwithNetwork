#pragma once
class NetworkAddress
{
private:
	SOCKADDR_IN m_SockAddress;
public:
	NetworkAddress();
	NetworkAddress(SOCKADDR_IN _socketAddress);
	NetworkAddress(std::wstring ip,unsigned short port);

	SOCKADDR_IN& GetSocketAddress() { return m_SockAddress; }
	std::wstring GetIPAddress();
	unsigned short GetPort() { return ntohs(m_SockAddress.sin_port); }

	static IN_ADDR IPv4ToBinaryAddress(const WCHAR* ip);
};

