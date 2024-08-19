#include "pch.h"
#include "Session.h"

Session::Session()
	: m_NetworkAddress{},m_Connected(false), m_RecvBuffer(BUFFER_SIZE), m_bSendRegistered(false)
{
	
	m_Socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(m_Socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_Socket);
}

void Session::Dispatch(IOCPEvent* _iocpEvent, int Bytes)
{
	switch (_iocpEvent->GetEventType())
	{
	case EVENT_TYPE::CONNECT:
		ProcessConnect();
		break;
	case EVENT_TYPE::DISCONNECT:
		ProcessDisconnect();
		break;
	case EVENT_TYPE::RECV:
		ProcessRecv(Bytes);
		break;
	case EVENT_TYPE::SEND:
		ProcessSend(Bytes);
		break;
	}
}

void Session::Disconnect(const WCHAR* message)
{
	if (m_Connected.exchange(false) == false)
		return;

	std::wcout << "Disconnected(" << message << ")" << std::endl;
	//OnDisconnected();
	//SocketUtils::Close(m_Socket);
	//GetService()->ReleaseSession(GetSession());
	RegisterDisconnect();
}



void Session::Send(std::shared_ptr<SendBuffer> _buff)
{
	if (IsConnected() == false)
		return;

	bool bRegisterSend = false;
	{
		LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
		m_SendBufferQueue.push(_buff);
		 
		if (m_bSendRegistered.exchange(true) == false)
			bRegisterSend = true;
	}
	if (bRegisterSend == true)
		RegisterSend();
}

bool Session::Connect()
{
	return RegisterConnect();
}

bool Session::RegisterConnect()
{
	//�̹� ����Ǿ������� �н�
	if (IsConnected() == true)
		return false;
	//Client�� �ƴҰ�� �н�
	if (GetService()->GetType() != SERVICE_TYPE::CLIENT)
		return false;

	//ReUse On : ����ߴ� Socket�� �ٽ� Binding�� �� �ְ� ���ش�
	if (SocketUtils::SetReUseAddress(m_Socket, true) == false)
		return false;

	//Socket �� Address ����
	if (SocketUtils::BindAnyAddress(m_Socket,/*���� Port ���� ����*/ 0) == false)
		return false;

	m_ConnectEvent.Init();
	//ReferenceCount 1����
	m_ConnectEvent.SetOwner(shared_from_this());

	DWORD Bytes = 0;
	//����� Server �ּ�
	SOCKADDR_IN SocketAddress = GetService()->GetNetworkAddress().GetSocketAddress();
	bool Result = SocketUtils::s_Connect(m_Socket,reinterpret_cast<SOCKADDR*>(&SocketAddress),sizeof(SocketAddress),nullptr,0,&Bytes,&m_ConnectEvent);
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			//ReferenceCount 1����
			m_ConnectEvent.SetOwner(nullptr);
			return false;
		}
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	m_DisconnectEvent.Init();
	//ReferenceCount 1����
	m_DisconnectEvent.SetOwner(shared_from_this());

	bool Result = SocketUtils::s_Disconnect(m_Socket, &m_DisconnectEvent, TF_REUSE_SOCKET, 0);
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			//ReferenceCount 1����
			m_DisconnectEvent.SetOwner(nullptr);
			return false;
		}
	}
	return true;
}

void Session::RegisterRecv()
{
	//����� �������� Ȯ��
	if (IsConnected() == false)
		return;

	WSABUF WSABuff;
	WSABuff.buf = reinterpret_cast<CHAR*>(m_RecvBuffer.GetWriteAddress());//���� ������ ���� �����ּ�
	WSABuff.len = m_RecvBuffer.GetFreeSize();//���� �ִ� �޸𸮷�

	DWORD Bytes = 0;
	DWORD Flags = 0;

	m_RecvEvent.Init();
	//ReferenceCount 1����
	m_RecvEvent.SetOwner(shared_from_this());
	//������ ���� ��ȣ �߻�
	int Result = WSARecv(m_Socket, &WSABuff, 1, &Bytes, &Flags, &m_RecvEvent, nullptr);
	if (Result == SOCKET_ERROR)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			//ReferenceCount 1����
			m_RecvEvent.SetOwner(nullptr);
		}
	}
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	m_SendEvent.Init();
	//ReferenceCount 1����
	m_SendEvent.SetOwner(shared_from_this());

	{
		LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
		while (m_SendBufferQueue.empty() == false)
		{
			//Session���ִ� SendBuffer�� SendEvent�� �ű��.
			std::shared_ptr<SendBuffer> sendBuffer = m_SendBufferQueue.front();
			m_SendBufferQueue.pop();
			m_SendEvent.GetBuff().push_back(sendBuffer);
		}
	}

	//SendEvent�� ��ϵ� SendBuff���� ���ͼ� WSABuff�� �Ű� ��´�.
	std::vector<std::shared_ptr<SendBuffer>>& vecSendBuff = m_SendEvent.GetBuff();
	std::vector<WSABUF> vecWSABuff;
	vecWSABuff.reserve(vecSendBuff.size());
	for (std::shared_ptr<SendBuffer> buff : vecSendBuff)
	{
		WSABUF WSABuff;
		WSABuff.buf = reinterpret_cast<char*>(buff->GetBuff());
		WSABuff.len = static_cast<ULONG>(buff->GetSize());
		vecWSABuff.push_back(WSABuff);
	}

	DWORD Bytes = 0;
	int Result = WSASend(m_Socket, vecWSABuff.data(), static_cast<DWORD>(vecWSABuff.size()), &Bytes, 0, &m_SendEvent, nullptr);

	if (Result == SOCKET_ERROR)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			//RefereceCount 1 ����
			m_SendEvent.SetOwner(nullptr);
			//RefereceCount �ʱ�ȭ
			m_SendEvent.GetBuff().clear();
			m_bSendRegistered = false;
		}
	}

}

void Session::ProcessConnect()
{
	m_ConnectEvent.SetOwner(nullptr);
	m_Connected = true;
	
	//Session ���
	GetService()->AddSession(GetSession());

	OnConnected();

	//���� ���
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	m_DisconnectEvent.SetOwner(nullptr);	
	OnDisconnected();
	SocketUtils::Close(m_Socket);
	GetService()->ReleaseSession(GetSession());
}

void Session::ProcessRecv(int _bytes)
{
	//ReferenceCount 1����
	m_RecvEvent.SetOwner(nullptr);

	//���޹��� ������ ����
	if (_bytes == 0)
	{
		Disconnect(L"Recv Zero!");
		return;
	}
	//������ ��ŭ ����Index ����
	if (m_RecvBuffer.Write(_bytes) == false)
	{
		Disconnect(L"Overflow Recv Write");
		return;
	}

	//���� �� �ִ� �޸𸮷� 
	int DataSize = m_RecvBuffer.GetDataSize();
	//���� ���۹��� �޸� ��� �� ����Ѹ�ŭ�� �޸𸮷� ��ȯ
	int ReadLength = OnRecv(m_RecvBuffer.GetReadAddress(), DataSize);
	//����� �޸𸮷��� 0���� �۰ų� ���޹����纸�� Ŭ��� �߸��Ȼ�Ȳ
	//�ƴҰ�� ������ŭ�� �޸�Index ����
	if (ReadLength < 0 || DataSize < ReadLength || m_RecvBuffer.Read(ReadLength) == false)
	{
		Disconnect(L"Overflow Recv Read");
		return;
	}
	//�޸�Index ����
	m_RecvBuffer.Clean();
	//Recv ����
	RegisterRecv();
}

void Session::ProcessSend(int _bytes)
{
	//ReferenceCount 1����
	m_SendEvent.SetOwner(nullptr);

	//ReferenceCount �ʱ�ȭ
	m_SendEvent.GetBuff().clear();

	if (_bytes == 0)
	{
		Disconnect(L"Recv Zero!");
		return;
	}

	OnSend(_bytes);

	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	if (m_SendBufferQueue.empty())
		m_bSendRegistered = false;
	else
		RegisterSend();
}

void Session::HandleError(int _errorCode)
{
	switch (_errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"Handle Error");
		break;
	default:
		std::cout << "Hadle Error : " << _errorCode << std::endl;
		break;
	}
}
