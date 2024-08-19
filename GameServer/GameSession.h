#pragma once
#include "test.pb.h"

struct RoomInfo
{
	int m_iRoomID = -1;
	int m_iCount = -1;

	void Reset()
	{
		m_iRoomID = -1;
		m_iCount = 0;
	}
};
class GameSession : public PacketSession
{
private:
	int	m_iID;
	int m_iCurRoomID;
	bool m_bReady;
	Protocol::STONE_COLOR m_eColor;
public:
	GameSession();
	~GameSession();

	int GetID() { return m_iID; };
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(Byte* _buff, int _size) override;
	virtual void OnSend(int Length) override;

	void CreateRoom();
	void EnterLobby();
	void EnterRoom(int _roomID);
	void ExitRoom();
	void EndGame(int _roomID);
	void MyTurn(int _x, int _y);
	void Ready(bool _ready);
	void SetPacket(Protocol::Player* _player);
	bool isReady() { return m_bReady; }
	void SetColor(Protocol::STONE_COLOR _color) { m_eColor = _color; }
	void ResetRoom();
	void ResetPlayer();
	void Regame();

};

