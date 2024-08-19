#include "pch.h"
#include "NetworkManager.h"
#include "ServerSession.h"

NetworkManager::NetworkManager()
{

}
NetworkManager::~NetworkManager()
{

}

void NetworkManager::Init()
{
    ServerPacketHandler::Init();
    //SocketUtils Init
    Core::GetInstance();
    //ClientService Ω√¿€
    m_ClientService = std::make_shared<ClientService>(
        NetworkAddress(L"127.0.0.1", 5000),
        std::make_shared<IOCPCore>(),
        [=]() {return CreateSession(); },
        1);
    assert(m_ClientService->Start() == true);
}

void NetworkManager::Update()
{
    m_ClientService->GetIOCPCore()->Dispatch(0);
}

std::shared_ptr<ServerSession> NetworkManager::CreateSession()
{
	m_ServerSession = std::make_shared<ServerSession>();
	return m_ServerSession;
}

void NetworkManager::SendPacket(std::shared_ptr<SendBuffer> _sendBuff)
{
	if (m_ServerSession != nullptr)
		m_ServerSession->Send(_sendBuff);
}
