#pragma once
#include "Service.h"

class ServerService : public Service
{
private:
	std::shared_ptr<Listener> m_Listener;
public:
	ServerService(NetworkAddress _selfAddress, std::shared_ptr<IOCPCore> _core, std::function<std::shared_ptr<Session>()> _createSessionFunc, int _maxCount = 1);
	virtual ~ServerService();

	virtual bool Start() override;
	virtual void CloseService() override;
};

