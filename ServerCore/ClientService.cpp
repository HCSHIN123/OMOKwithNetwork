#include "pch.h"
#include "ClientService.h"

ClientService::ClientService(NetworkAddress _targetAddress, std::shared_ptr<IOCPCore> _core, std::function<std::shared_ptr<Session>()> _createSessionFunc, int _maxCount)
	:Service(SERVICE_TYPE::CLIENT,_targetAddress,_core,_createSessionFunc,_maxCount)
{
}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;
	const int SessionCount = GetMaxSessionCount();
	for (int i = 0; i < SessionCount; i++)
	{
		std::shared_ptr<Session> session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}
