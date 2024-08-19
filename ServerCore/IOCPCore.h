#pragma once

//enable_shared_from_this : 자기자신의 Shared Point를 반환하기위한 부모 Class 상속
class IOCPObject : public std::enable_shared_from_this<IOCPObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IOCPEvent* _iocpEvent,int Bytes = 0) abstract;
};

class IOCPCore
{
private:
	HANDLE m_hIOCP;
public:
	IOCPCore();
	~IOCPCore();

	bool Register(std::shared_ptr<IOCPObject> _iocpObject);
	bool Dispatch(unsigned int _TimeOut = INFINITE);

	HANDLE GetHandle() { return m_hIOCP; }
};
