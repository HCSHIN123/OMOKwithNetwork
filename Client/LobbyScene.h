#pragma once
#include"Scene.h"
#include"ResourceManager.h"
#include "RoomButton.h"

enum
{
	ALL_ROOM = 5,
};

class LobbyScene : public Scene
{
private:
	Texture* m_pBackGround;
	std::list<Room> m_RoomList;
	std::vector<RoomButton*> m_vecRoomButton;
public:
	LobbyScene(std::string _strName);
	~LobbyScene();
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	void InitRoomButton();
	virtual void Update() override;
	virtual void Render(HDC _memDC) override;

	void EnterLobby();
	void CreateRoom();
	void EnterRoom(bool _canplay);
	void SendCheckEnterRoom(int _num);
	void ApplyNerRoom(Room _room);
	void AddRoom(Room _room);
	void ShowRooms();
	void UpdateRoom(std::list<Room> _rooms);
};


