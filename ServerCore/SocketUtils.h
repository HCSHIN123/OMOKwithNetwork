#pragma once
#include"NetworkAddress.h"

class SocketUtils
{
private:

public:
	static LPFN_CONNECTEX s_Connect;
	static LPFN_DISCONNECTEX s_Disconnect;
	static LPFN_ACCEPTEX s_Accept;
	static void Init();
	static void Clear();
	static SOCKET CreateSocket();
	static bool BindWindows(SOCKET _socket, GUID _guid, LPVOID* _function);
	static void Close(SOCKET& _socket);
	static bool Bind(SOCKET _socket, NetworkAddress _networkAddress);
	static bool BindAnyAddress(SOCKET _socket, unsigned short _port);
	static bool Listen(SOCKET _socket, int _backLog = SOMAXCONN);
	static bool SetLinger(SOCKET _socket, unsigned short _onOff, unsigned short _linger);
	static bool SetReUseAddress(SOCKET _socket, bool flag);
	static bool SetRecvBuffSize(SOCKET _socket, int size);
	static bool SetSendBuffSize(SOCKET _socket, int size);
	static bool SetTCPNoDelay(SOCKET _socket, bool _flag);
	static bool SetUpdateAcceptSocket(SOCKET _socket, SOCKET _listenSocket);
};



template<typename type>
static bool SetSocketOption(SOCKET _socket, int _level, int _OptionName, type OptionValue)
{
	int Result = setsockopt(_socket, _level, _OptionName, reinterpret_cast<char*>(&OptionValue), sizeof(type));
	return Result != SOCKET_ERROR;
}
