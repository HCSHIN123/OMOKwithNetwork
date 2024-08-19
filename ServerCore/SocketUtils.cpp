#include "pch.h"
#include "SocketUtils.h"

//각 목적에 따른 함수포인터
LPFN_CONNECTEX SocketUtils::s_Connect = nullptr;
LPFN_DISCONNECTEX SocketUtils::s_Disconnect = nullptr;
LPFN_ACCEPTEX SocketUtils::s_Accept = nullptr;

void SocketUtils::Init()
{
	//WindowSocket 사용 시작
	WSADATA WSAData;
	int Result = WSAStartup(MAKEWORD(2, 2), &WSAData);
	assert(Result == 0);

	//초기설정용 Socket 생성
	SOCKET socket = CreateSocket();
	//reinterpret_cast 다른 자료형간의 관계는 생각하지않고 자료그자체를 bit단위로 형변환하여 사용한다.
	//bit단위로만 고려하여 형변환 하므로 다른 크기의 데이터는 변환이 불가능하다.
	//그대신 bit만 고려하기 때문에 void*로도 전환이 가능하다는 특징이 생긴다.
	//ConnectEx함수 등록
	bool bResult = BindWindows(socket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&s_Connect));
	assert(bResult == true);
	//DisconnectEx함수 등록
	bResult = BindWindows(socket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&s_Disconnect));
	assert(bResult == true);
	//AcceptEx함수 등록
	bResult = BindWindows(socket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&s_Accept));
	assert(bResult == true);
	//초기설정용 Socket제거
	Close(socket);
}

void SocketUtils::Clear()
{
	//WindowSocket사용 종료
	WSACleanup();
}

SOCKET SocketUtils::CreateSocket()
{
	//소켓생성
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::BindWindows(SOCKET _socket, GUID _guid, LPVOID* _function)
{
	DWORD Bytes;
	//전달받은 함수포인터를 사용하기위해 등록하는 함수
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
	//전달받은 주소와 Socket을 연결
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
	//Client 접속대기 시작
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

//Listen의 속성을 Client에 그대로 적용
bool SocketUtils::SetUpdateAcceptSocket(SOCKET _socket, SOCKET _listenSocket)
{
	return SetSocketOption(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, _listenSocket);
}
