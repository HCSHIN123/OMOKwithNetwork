#pragma once
#include "Service.h"
class ClientService : public Service
{
public:
	ClientService(NetworkAddress _targetAddress, std::shared_ptr<IOCPCore> _core, std::function<std::shared_ptr<Session>()> _createSessionFunc, int _maxCount = 1);
	virtual ~ClientService();

	virtual bool Start() override;
};

