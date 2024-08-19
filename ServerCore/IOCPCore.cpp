#include "pch.h"
#include "IOCPCore.h"


IOCPCore::IOCPCore()
{
	//CompletionPort 생성
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
	assert(m_hIOCP != INVALID_HANDLE_VALUE);
}

IOCPCore::~IOCPCore()
{
	//CompletionPort 제거
	CloseHandle(m_hIOCP);
}

bool IOCPCore::Register(std::shared_ptr<IOCPObject> _iocpObject)
{
	//CompletionPort에 특정 IOCPObject를 비동기로 작업완료 여부를 알림 대기 등록한다.
	SOCKET socket;
	return CreateIoCompletionPort(_iocpObject->GetHandle(), m_hIOCP, 0, 0);
}

bool IOCPCore::Dispatch(unsigned int _TimeOut)
{
	DWORD Bytes = 0;
	ULONG_PTR Key;
	IOCPEvent* iocpEvent = nullptr;
	//등록해놓았던 신호결과가 생기면 해당 신호의 IOCPObject와 IOCPEvent복원 후 처리
	bool Result = GetQueuedCompletionStatus(m_hIOCP, &Bytes, &Key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), _TimeOut);
	
	if (Result == true)
	{
		//IOCPEvent에 연결해뒀던 IOCPObject 복원
		std::shared_ptr<IOCPObject> iocpObject = iocpEvent->GetOwner();
		//작업이 신호가 전달되면 해당 IOCPObject 작업 실행
		iocpObject->Dispatch(iocpEvent, Bytes);
	}
	else
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode == WAIT_TIMEOUT)
			return false;
		else
		{
			//IOCPEvent에 연결해뒀던 IOCPObject 복원
			std::shared_ptr<IOCPObject> iocpObject = iocpEvent->GetOwner();
			iocpObject->Dispatch(iocpEvent, Bytes);
		}
	}

	return true;
}
