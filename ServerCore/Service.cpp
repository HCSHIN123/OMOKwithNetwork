#include "pch.h"
#include "Service.h"

Service::Service(SERVICE_TYPE _eServiceType, NetworkAddress _networkAddress, std::shared_ptr<IOCPCore> _core, std::function<std::shared_ptr<Session>(void)> _createSessionFunc, int _maxCount)
{
	m_iSessionCount = 0;
	m_NetworkAddress = _networkAddress;
	m_eServiceType = _eServiceType;
	m_IocpCore = _core;
	m_CreateSessionFunc = _createSessionFunc;
	m_iMaxSessionCount = _maxCount;

}

Service::~Service()
{
}

void Service::CloseService()
{
}

std::shared_ptr<Session> Service::CreateSession()
{
	//전달받은 Session함수포인터로 Session 생성
	std::shared_ptr<Session> session = m_CreateSessionFunc();
	session->SetService(shared_from_this());
	if (m_IocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(std::shared_ptr<Session> _session)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	m_iSessionCount++;
	m_SessionSet.insert(_session);
}

void Service::ReleaseSession(std::shared_ptr<Session> _session)
{
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	assert(m_SessionSet.erase(_session) != 0);
	m_iSessionCount--;
}
