#include "pch.h"
#include "GameSession.h"
#include "SessionManager.h"
#include "ClientPacketHandler.h"
#include "test.pb.h"


void main()
{
	//PacketHandler �ʱ�ȭ
	ClientPacketHandler::Init();
	//SocketUtils Init
	Core::GetInstance();
	//ServerService ����
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
					//��ȣ�� �ִ��� Ȯ��
					serverService->GetIOCPCore()->Dispatch();
				}
			});
	}
	Core::GetInstance().GetThreadManager()->Join();
}
