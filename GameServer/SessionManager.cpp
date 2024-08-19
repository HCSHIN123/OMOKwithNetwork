#include "pch.h"
#include "SessionManager.h"
#include "test.pb.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

void SessionManager::Insert(std::shared_ptr<GameSession> _session)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	m_SessionSet.insert(_session);
}

void SessionManager::Erase(std::shared_ptr<GameSession> _session)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	m_SessionSet.erase(_session);
}

void SessionManager::Send(std::shared_ptr<GameSession> _session, std::shared_ptr<SendBuffer> _buff)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	_session->Send(_buff);
}

void SessionManager::BroadCast(std::shared_ptr<SendBuffer> _buff)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	//반복문이 도는중에 Client가 접속을 종료하여 Erase가 발생할경우 
	//반복을도는중 삭제가일어나 문제가 발생할 수 있다.
	//그러므로 반복문이 도는중에 삭제가 안되게끔
	//Session class의 Disconnect에 있던 OnDisconnect를 ProcessDisconnect 로 옮겨 사용한다.
	for (std::shared_ptr<GameSession> session : m_SessionSet)
		session->Send(_buff);
	
}
