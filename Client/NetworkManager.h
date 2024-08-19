#pragma once
class NetworkManager
{
	SINGLETON(NetworkManager)
private:
	std::shared_ptr<ClientService> m_ClientService;

	std::shared_ptr<class ServerSession> m_ServerSession;
public:
	void Init();
	void Update();
	std::shared_ptr<ServerSession> CreateSession();
	void SendPacket(std::shared_ptr<SendBuffer> _sendBuff);
};

