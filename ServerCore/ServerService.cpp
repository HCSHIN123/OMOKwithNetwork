#include "pch.h"
#include "ServerService.h"
#include <memory>

ServerService::ServerService(NetworkAddress _selfAddress, std::shared_ptr<IOCPCore> _core, std::function<std::shared_ptr<Session>()> _createSessionFunc, int _maxCount)
	:Service(SERVICE_TYPE::SERVER, _selfAddress, _core, _createSessionFunc, _maxCount)
{
	m_Listener = nullptr;
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	//서비스 실행 가능한지 확인
	if (CanStart() == false)
		return false;

	//Listener 생성
	m_Listener = std::make_shared<Listener>();
	if (m_Listener == nullptr)
		return false;
	//static_pointer_cast : shared_ptr 형으로 형변환 해준다(C++20)
	std::shared_ptr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	//Listener에 Service 전달하며 시작
	if(m_Listener->StartAccept(service) == false)
		return false;
	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}
