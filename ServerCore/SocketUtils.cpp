#include "pch.h"
#include "SocketUtils.h"

//�� ������ ���� �Լ�������
LPFN_CONNECTEX SocketUtils::s_Connect = nullptr;
LPFN_DISCONNECTEX SocketUtils::s_Disconnect = nullptr;
LPFN_ACCEPTEX SocketUtils::s_Accept = nullptr;

void SocketUtils::Init()
{
	//WindowSocket ��� ����
	WSADATA WSAData;
	int Result = WSAStartup(MAKEWORD(2, 2), &WSAData);
	assert(Result == 0);

	//�ʱ⼳���� Socket ����
	SOCKET socket = CreateSocket();
	//reinterpret_cast �ٸ� �ڷ������� ����� ���������ʰ� �ڷ����ü�� bit������ ����ȯ�Ͽ� ����Ѵ�.
	//bit�����θ� ����Ͽ� ����ȯ �ϹǷ� �ٸ� ũ���� �����ʹ� ��ȯ�� �Ұ����ϴ�.
	//�״�� bit�� ����ϱ� ������ void*�ε� ��ȯ�� �����ϴٴ� Ư¡�� �����.
	//ConnectEx�Լ� ���
	bool bResult = BindWindows(socket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&s_Connect));
	assert(bResult == true);
	//DisconnectEx�Լ� ���
	bResult = BindWindows(socket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&s_Disconnect));
	assert(bResult == true);
	//AcceptEx�Լ� ���
	bResult = BindWindows(socket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&s_Accept));
	assert(bResult == true);
	//�ʱ⼳���� Socket����
	Close(socket);
}

void SocketUtils::Clear()
{
	//WindowSocket��� ����
	WSACleanup();
}

SOCKET SocketUtils::CreateSocket()
{
	//���ϻ���
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::BindWindows(SOCKET _socket, GUID _guid, LPVOID* _function)
{
	DWORD Bytes;
	//���޹��� �Լ������͸� ����ϱ����� ����ϴ� �Լ�
	int Result = WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &_guid, sizeof(_guid),
		_function, sizeof(*_function), &Bytes, NULL, NULL);
	return Result != SOCKET_ERROR;
}

void SocketUtils::Close(SOCKET& _socket)
{
	if(_socket != INVALID_SOCKET)
		closesocket(_socket);
	_socket = INVALID_SOCKET;
}

bool SocketUtils::Bind(SOCKET _socket, NetworkAddress _networkAddress)
{
	//���޹��� �ּҿ� Socket�� ����
	SOCKADDR_IN& Address = _networkAddress.GetSocketAddress();
	int Result = bind(_socket, reinterpret_cast<const SOCKADDR*>(&Address), sizeof(Address));
	return Result != SOCKET_ERROR;
}

bool SocketUtils::BindAnyAddress(SOCKET _socket, unsigned short _port)
{
	SOCKADDR_IN Address;
	Address.sin_family = AF_INET;
	Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Address.sin_port = htons(_port);
	int Result = bind(_socket, reinterpret_cast<const SOCKADDR*>(&Address), sizeof(Address));
	return Result != SOCKET_ERROR;
}

bool SocketUtils::Listen(SOCKET _socket, int _backLog)
{
	//Client ���Ӵ�� ����
	int Result = listen(_socket, _backLog);
	return Result != SOCKET_ERROR;
}

bool SocketUtils::SetLinger(SOCKET _socket, unsigned short _onOff, unsigned short _linger)
{
	LINGER Option;
	Option.l_onoff = _onOff;
	Option.l_linger = _linger;
	return SetSocketOption(_socket,SOL_SOCKET,SO_LINGER,Option);
}

bool SocketUtils::SetReUseAddress(SOCKET _socket, bool flag)
{
	return SetSocketOption(_socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBuffSize(SOCKET _socket, int size)
{
	return SetSocketOption(_socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBuffSize(SOCKET _socket, int size)
{
	return SetSocketOption(_socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTCPNoDelay(SOCKET _socket, bool _flag)
{
	return SetSocketOption(_socket, SOL_SOCKET, TCP_NODELAY, _flag);
}

//Listen�� �Ӽ��� Client�� �״�� ����
bool SocketUtils::SetUpdateAcceptSocket(SOCKET _socket, SOCKET _listenSocket)
{
	return SetSocketOption(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, _listenSocket);
}
