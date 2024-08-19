#pragma once


enum class SERVICE_TYPE
{
	SERVER,
	CLIENT,
};

class Service : public std::enable_shared_from_this<Service>
{
protected:
	Lock m_Lock;
	SERVICE_TYPE m_eServiceType;
	NetworkAddress m_NetworkAddress;
	std::shared_ptr<IOCPCore> m_IocpCore;
	std::set<std::shared_ptr<Session>> m_SessionSet;
	int m_iSessionCount;
	int m_iMaxSessionCount;
	std::function<std::shared_ptr<Session>(void)> m_CreateSessionFunc;
public:
	Service(SERVICE_TYPE _eServiceType, NetworkAddress _networkAddress, std::shared_ptr<IOCPCore> _core,
		std::function<std::shared_ptr<Session>(void)> _createSessionFunc, int _maxCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	virtual void CloseService();

	std::shared_ptr<Session> CreateSession();
	void AddSession(std::shared_ptr<Session> _session);
	void ReleaseSession(std::shared_ptr<Session> _session);

	int GetCurSessionCount() { return m_iSessionCount; }
	int GetMaxSessionCount() { return m_iMaxSessionCount; }
	SERVICE_TYPE GetType() { return m_eServiceType; }
	NetworkAddress GetNetworkAddress() { return m_NetworkAddress; }
	std::shared_ptr<IOCPCore>& GetIOCPCore() { return m_IocpCore; }
	bool CanStart() { return m_CreateSessionFunc != nullptr; }
	void SetCreateSessionFunc(std::function<std::shared_ptr<Session>(void)> _createSessionFunc)
	{
		m_CreateSessionFunc = _createSessionFunc;
	}


};

