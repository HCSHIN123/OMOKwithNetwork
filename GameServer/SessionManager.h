#pragma once
#include"GameSession.h"
#include "SendBuffer.h"
class SessionManager
{
private:
	Lock m_Lock;
	std::set<std::shared_ptr<GameSession>> m_SessionSet;
public:
	static SessionManager& GetInstance()
	{
		static SessionManager Instance;
		return Instance;
	}
	SessionManager();
	~SessionManager();
	std::set<std::shared_ptr<GameSession>>& GetSessionSet() { return m_SessionSet; }
	void Insert(std::shared_ptr<GameSession> _session);
	void Erase(std::shared_ptr<GameSession> _session);
	void Send(std::shared_ptr<GameSession> _session, std::shared_ptr<SendBuffer> _buff);
	void BroadCast(std::shared_ptr<SendBuffer> _buff);
};

