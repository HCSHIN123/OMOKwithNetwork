#include "pch.h"
#include "GameSession.h"
#include "SessionManager.h"
#include "ClientPacketHandler.h"
#include "test.pb.h"


void main()
{
	//PacketHandler 초기화
	ClientPacketHandler::Init();
	//SocketUtils Init
	Core::GetInstance();
	//ServerService 시작
	std::shared_ptr<ServerService> serverService = std::make_shared<ServerService>(
		NetworkAddress(L"127.0.0.1", 5000),
		std::make_shared<IOCPCore>(), 
		std::make_shared<GameSession>,
		100);
	assert(serverService->Start() == true);
	for (int i = 0; i < 5; i++)
	{
		Core::GetInstance().GetThreadManager()->Add([=]()
			{
				srand(time(NULL));
				while (true)
				{
					//신호가 있는지 확인
					serverService->GetIOCPCore()->Dispatch();
				}
			});
	}
	Core::GetInstance().GetThreadManager()->Join();
}
