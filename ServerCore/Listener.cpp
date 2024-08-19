#include "pch.h"
#include "Listener.h"
#include "ServerService.h"

void Listener::RegisterAccept(AcceptEvent* _acceptEvent)
{
	//Session ����
	std::shared_ptr<Session> session = m_Service->CreateSession();
	
	//���޹��� Event�� Session���
	_acceptEvent->Init();
	_acceptEvent->SetSession(session);
	
	//Socket�� ���޹��� ��ȣ�� �����ϱ� ���� Socket�� Event���
	DWORD RecvBytes;
	bool Result = SocketUtils::s_Accept(m_Socket, session->GetSocket(), session->m_RecvBuffer.GetWriteAddress(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&RecvBytes, static_cast<LPOVERLAPPED>(_acceptEvent));
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		//WSA_IO_PENDING : ����۾��� �ٷ� �Ϸ���� ���� ����
		if (ErrorCode != WSA_IO_PENDING)
		{
			//��Ͽ� �����ߴٸ� ��õ�
			RegisterAccept(_acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* _acceptEvent)
{
	//�ش�Event�� Session��������
	std::shared_ptr<Session> session = _acceptEvent->GetSession();

	//����� Client������ �Ӽ��� Listen�Ӽ����� �����ϱ�
	if (SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), m_Socket) == false)
	{
		//���� �� ����
		RegisterAccept(_acceptEvent);
		return;
	}

	SOCKADDR_IN SocketAddress;
	int SocketAddresLength = sizeof(SocketAddress);
	//Client Socket���� ����� Address ����
	int Result = getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&SocketAddress), &SocketAddresLength);
	if (Result == SOCKET_ERROR)
	{
		//���� �� ����
		RegisterAccept(_acceptEvent);
		return;
	}
	//������ �ּ� Session�� ����
	session->SetNetworkAddress(NetworkAddress(SocketAddress));

	std::cout << "Client Connected" << std::endl;
	session->ProcessConnect();
	//���� �� ����
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
	//Listen Socket����
	m_Socket = SocketUtils::CreateSocket();
	if (m_Socket == INVALID_SOCKET)
		return false;
	//Listen������ �񵿱�� �۾��Ϸ� ���θ� �˸� ��� ����Ѵ�.
	//m_Service�� ���ؼ� IOCPCore�� ����Ѵ�.
	if (m_Service->GetIOCPCore()->Register(shared_from_this()) == false)
		return false;

	//ReUse On : ����ߴ� Socket�� �ٽ� Binding�� �� �ְ� ���ش�
	if (SocketUtils::SetReUseAddress(m_Socket, true) == false)
		return false;
	//Linger Off : ��Ĺ�� ������ ���� �� ��ó ���������� ���� ���۸� ��� ���� �� ��������
	if (SocketUtils::SetLinger(m_Socket, 0, 0) == false)
		return false;
	//���ϰ� �ּ� ����
	//m_Service�� ���ؼ� NetworkAddress�� ����Ѵ�.
	if (SocketUtils::Bind(m_Socket, m_Service->GetNetworkAddress()) == false)
		return false;
	//���Ӵ�� ����
	if (SocketUtils::Listen(m_Socket) == false)
		return false;
	//�ִ� �������� Client �� 
	const int AcceptCount = m_Service->GetMaxSessionCount();
	for (int i = 0; i < 1; i++)
	{
		//���ӿϷ� Event ����
		AcceptEvent* acceptEvent = new AcceptEvent();
		//IOCPEvent�� IOCPObject ����
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
