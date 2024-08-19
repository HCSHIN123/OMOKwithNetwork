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
	//이미 연결되어있으면 패스
	if (IsConnected() == true)
		return false;
	//Client가 아닐경우 패스
	if (GetService()->GetType() != SERVICE_TYPE::CLIENT)
		return false;

	//ReUse On : 사용했던 Socket을 다시 Binding할 수 있게 해준다
	if (SocketUtils::SetReUseAddress(m_Socket, true) == false)
		return false;

	//Socket 과 Address 연결
	if (SocketUtils::BindAnyAddress(m_Socket,/*남는 Port 임의 지정*/ 0) == false)
		return false;

	m_ConnectEvent.Init();
	//ReferenceCount 1증가
	m_ConnectEvent.SetOwner(shared_from_this());

	DWORD Bytes = 0;
	//연결될 Server 주소
	SOCKADDR_IN SocketAddress = GetService()->GetNetworkAddress().GetSocketAddress();
	bool Result = SocketUtils::s_Connect(m_Socket,reinterpret_cast<SOCKADDR*>(&SocketAddress),sizeof(SocketAddress),nullptr,0,&Bytes,&m_ConnectEvent);
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			//ReferenceCount 1감소
			m_ConnectEvent.SetOwner(nullptr);
			return false;
		}
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	m_DisconnectEvent.Init();
	//ReferenceCount 1증가
	m_DisconnectEvent.SetOwner(shared_from_this());

	bool Result = SocketUtils::s_Disconnect(m_Socket, &m_DisconnectEvent, TF_REUSE_SOCKET, 0);
	if (Result == false)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			//ReferenceCount 1감소
			m_DisconnectEvent.SetOwner(nullptr);
			return false;
		}
	}
	return true;
}

void Session::RegisterRecv()
{
	//연결된 상태인지 확인
	if (IsConnected() == false)
		return;

	WSABUF WSABuff;
	WSABuff.buf = reinterpret_cast<CHAR*>(m_RecvBuffer.GetWriteAddress());//쓰기 시작할 버퍼 시작주소
	WSABuff.len = m_RecvBuffer.GetFreeSize();//남은 최대 메모리량

	DWORD Bytes = 0;
	DWORD Flags = 0;

	m_RecvEvent.Init();
	//ReferenceCount 1증가
	m_RecvEvent.SetOwner(shared_from_this());
	//데이터 수신 신호 발생
	int Result = WSARecv(m_Socket, &WSABuff, 1, &Bytes, &Flags, &m_RecvEvent, nullptr);
	if (Result == SOCKET_ERROR)
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			//ReferenceCount 1감소
			m_RecvEvent.SetOwner(nullptr);
		}
	}
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	m_SendEvent.Init();
	//ReferenceCount 1증가
	m_SendEvent.SetOwner(shared_from_this());

	{
		LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
		while (m_SendBufferQueue.empty() == false)
		{
			//Session에있는 SendBuffer를 SendEvent로 옮긴다.
			std::shared_ptr<SendBuffer> sendBuffer = m_SendBufferQueue.front();
			m_SendBufferQueue.pop();
			m_SendEvent.GetBuff().push_back(sendBuffer);
		}
	}

	//SendEvent에 등록된 SendBuff들을 들고와서 WSABuff에 옮겨 담는다.
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
			//RefereceCount 1 감소
			m_SendEvent.SetOwner(nullptr);
			//RefereceCount 초기화
			m_SendEvent.GetBuff().clear();
			m_bSendRegistered = false;
		}
	}

}

void Session::ProcessConnect()
{
	m_ConnectEvent.SetOwner(nullptr);
	m_Connected = true;
	
	//Session 등록
	GetService()->AddSession(GetSession());

	OnConnected();

	//수신 등록
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
	//ReferenceCount 1감소
	m_RecvEvent.SetOwner(nullptr);

	//전달받은 정보가 없음
	if (_bytes == 0)
	{
		Disconnect(L"Recv Zero!");
		return;
	}
	//저장한 만큼 쓰기Index 증가
	if (m_RecvBuffer.Write(_bytes) == false)
	{
		Disconnect(L"Overflow Recv Write");
		return;
	}

	//읽을 수 있는 메모리량 
	int DataSize = m_RecvBuffer.GetDataSize();
	//실제 전송받은 메모리 사용 후 사용한만큼의 메모리량 반환
	int ReadLength = OnRecv(m_RecvBuffer.GetReadAddress(), DataSize);
	//사용한 메모리량이 0보다 작거나 전달받은양보다 클경우 잘못된상황
	//아닐경우 읽은만큼의 메모리Index 증가
	if (ReadLength < 0 || DataSize < ReadLength || m_RecvBuffer.Read(ReadLength) == false)
	{
		Disconnect(L"Overflow Recv Read");
		return;
	}
	//메모리Index 정리
	m_RecvBuffer.Clean();
	//Recv 재등록
	RegisterRecv();
}

void Session::ProcessSend(int _bytes)
{
	//ReferenceCount 1감소
	m_SendEvent.SetOwner(nullptr);

	//ReferenceCount 초기화
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
