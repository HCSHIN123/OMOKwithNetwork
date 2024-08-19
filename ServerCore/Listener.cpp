#include "pch.h"
#include "Listener.h"
#include "ServerService.h"

void Listener::RegisterAccept(AcceptEvent* _acceptEvent)
{
	//Session 생성
	std::shared_ptr<Session> session = m_Service->CreateSession();
	
	//전달받은 Event에 Session등록
	_acceptEvent->Init();
	_acceptEvent->SetSession(session);
	
	//Socket에 전달받을 신호를 구분하기 위한 Socket과 Event등록
	DWORD RecvBytes;
	bool Result = SocketUtils::s_Accept(m_Socket, session->GetSocket(), session->m_RecvBuffer.GetWriteAddress(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&RecvBytes, static_cast<LPOVERLAPPED>(_acceptEvent));
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		//WSA_IO_PENDING : 등록작업이 바로 완료되지 않은 상태
		if (ErrorCode != WSA_IO_PENDING)
		{
			//등록에 실패했다면 재시도
			RegisterAccept(_acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* _acceptEvent)
{
	//해당Event의 Session가져오기
	std::shared_ptr<Session> session = _acceptEvent->GetSession();

	//연결된 Client소켓의 속성을 Listen속성으로 적용하기
	if (SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), m_Socket) == false)
	{
		//실패 시 재등록
		RegisterAccept(_acceptEvent);
		return;
	}

	SOCKADDR_IN SocketAddress;
	int SocketAddresLength = sizeof(SocketAddress);
	//Client Socket으로 연결된 Address 복원
	int Result = getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&SocketAddress), &SocketAddresLength);
	if (Result == SOCKET_ERROR)
	{
		//실패 시 재등록
		RegisterAccept(_acceptEvent);
		return;
	}
	//복원된 주소 Session에 연결
	session->SetNetworkAddress(NetworkAddress(SocketAddress));

	std::cout << "Client Connected" << std::endl;
	session->ProcessConnect();
	//성공 후 재등록
	RegisterAccept(_acceptEvent);
}

Listener::Listener()
	:m_Socket(INVALID_SOCKET)
{
}

Listener::~Listener()
{
	SocketUtils::Close(m_Socket);

	for (AcceptEvent* Event : m_vecAcceptEvents)
	{
		delete Event;
	}
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_Socket);
}

void Listener::Dispatch(IOCPEvent* _iocpEvent, int Bytes)
{
	assert(_iocpEvent->GetEventType() == EVENT_TYPE::ACCEPT);

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(_iocpEvent);
	ProcessAccept(acceptEvent);
}

bool Listener::StartAccept(std::shared_ptr<ServerService> _service)
{
	m_Service = _service;
	if (m_Service == nullptr)
		return false;
	//Listen Socket생성
	m_Socket = SocketUtils::CreateSocket();
	if (m_Socket == INVALID_SOCKET)
		return false;
	//Listen동작을 비동기로 작업완료 여부를 알림 대기 등록한다.
	//m_Service를 통해서 IOCPCore를 사용한다.
	if (m_Service->GetIOCPCore()->Register(shared_from_this()) == false)
		return false;

	//ReUse On : 사용했던 Socket을 다시 Binding할 수 있게 해준다
	if (SocketUtils::SetReUseAddress(m_Socket, true) == false)
		return false;
	//Linger Off : 소캣의 연결이 끊을 때 미처 보내지못한 남은 버퍼를 모두 보낸 후 연결종료
	if (SocketUtils::SetLinger(m_Socket, 0, 0) == false)
		return false;
	//소켓과 주소 연결
	//m_Service를 통해서 NetworkAddress를 사용한다.
	if (SocketUtils::Bind(m_Socket, m_Service->GetNetworkAddress()) == false)
		return false;
	//접속대기 시작
	if (SocketUtils::Listen(m_Socket) == false)
		return false;
	//최대 동시접속 Client 수 
	const int AcceptCount = m_Service->GetMaxSessionCount();
	for (int i = 0; i < 1; i++)
	{
		//접속완료 Event 생성
		AcceptEvent* acceptEvent = new AcceptEvent();
		//IOCPEvent에 IOCPObject 연결
		acceptEvent->SetOwner(shared_from_this());
		RegisterAccept(acceptEvent);
		m_vecAcceptEvents.push_back(acceptEvent);
	}
	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(m_Socket);
}
