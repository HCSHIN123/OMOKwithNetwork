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
	//�ݺ����� �����߿� Client�� ������ �����Ͽ� Erase�� �߻��Ұ�� 
	//�ݺ��������� �������Ͼ ������ �߻��� �� �ִ�.
	//�׷��Ƿ� �ݺ����� �����߿� ������ �ȵǰԲ�
	//Session class�� Disconnect�� �ִ� OnDisconnect�� ProcessDisconnect �� �Ű� ����Ѵ�.
	for (std::shared_ptr<GameSession> session : m_SessionSet)
		session->Send(_buff);
	
}
