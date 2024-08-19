#pragma once

class ServerService;
class AcceptEvent;
class Listener : public IOCPObject
{
private:
	void RegisterAccept(AcceptEvent* _acceptEvent);
	void ProcessAccept(AcceptEvent* _acceptEvent);
protected:
	SOCKET m_Socket;
	std::vector<AcceptEvent*> m_vecAcceptEvents;
	std::shared_ptr<ServerService> m_Service;
public:
	Listener();
	~Listener();
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IOCPEvent* _iocpEvent, int Bytes = 0) override;

	bool StartAccept(std::shared_ptr<ServerService> _service);
	void CloseSocket();


};

