#include "pch.h"
#include "LobbyScene.h"
#include "Texture.h"
#include"SceneManager.h"
#include "InputManager.h"
#include "GameCore.h"


LobbyScene::LobbyScene(std::string _strName) : Scene(_strName)
{
	m_pBackGround = nullptr;
	m_vecRoomButton.resize(ALL_ROOM);
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::Init()
{
	Protocol::C_ENTERLOBBY packet;
	NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));

	m_pBackGround = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::MAINSCENE);
	Scene::SetWindowSize(500, 800);
	m_vecRoomButton.resize(ALL_ROOM);
	Button* CreateRoomButton = new Button;
	CreateRoomButton->Init({ 15, 600 }, TEXTURE_TYPE::CREATE_ROOM, std::bind(&LobbyScene::CreateRoom, this), "Play");
	Scene::AddObject(CreateRoomButton, OBJECT_GROUP::BUTTON);
	CreateRoomButton->SetActivate(true);
	
	
	Button* RefreshButton = new Button;
	RefreshButton->Init({ 330, 15 }, TEXTURE_TYPE::REFRESH, std::bind(&LobbyScene::EnterLobby, this), "Play");
	Scene::AddObject(RefreshButton, OBJECT_GROUP::BUTTON);
	RefreshButton->SetActivate(true);

	InitRoomButton();
}

void LobbyScene::InitRoomButton()
{
	for (int i = 0; i < ALL_ROOM; ++i)
	{
		RoomButton* roomBtn = new RoomButton;
		roomBtn->Init({ 0,0 }, TEXTURE_TYPE::ENTER_ROOM, std::bind(&LobbyScene::SendCheckEnterRoom, this, std::placeholders::_1), { i,0 });
		roomBtn->SetActivate(false);
		Scene::AddObject(roomBtn, OBJECT_GROUP::BUTTON);
		m_vecRoomButton[i] = roomBtn;
	}
}

void LobbyScene::Update()
{
	Scene::Update();
}

void LobbyScene::Render(HDC _memDC)
{
	BitBlt(_memDC, 0, 0, m_pBackGround->GetWidth(), m_pBackGround->GetHeight(),
		m_pBackGround->GetDC(), 0, 0, SRCCOPY);

	Scene::Render(_memDC);
}

void LobbyScene::EnterLobby()
{
	Protocol::C_ENTERLOBBY packet;
	NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
}

void LobbyScene::CreateRoom()
{
	Protocol::C_CREATEROOM packet;
	NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
}

void LobbyScene::EnterRoom(bool _canplay)
{
	if (_canplay == false)
	{
		MessageBox(GameCore::GetInstance()->GethWnd(), "입장불가", "X", MB_OK);
		return;
	}
	SceneManager::GetInstance()->SceneChange(SCENE_TYPE::GAME_SCENE);
}

void LobbyScene::SendCheckEnterRoom(int _num)
{
	Protocol::C_ENTER packet;
	packet.set_roomid(_num);
	NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
}

void LobbyScene::ApplyNerRoom(Room _room)
{
	AddRoom(_room);
	ShowRooms();
}

void LobbyScene::AddRoom(Room _room)
{
	m_RoomList.push_back(_room);
	m_vecRoomButton[_room.m_iRoomID]->SetActivate(true);
	m_vecRoomButton[_room.m_iRoomID]->SetRoomInfo(_room);
}

void LobbyScene::ShowRooms()
{
	int i = 0;
	for (Room r : m_RoomList)
	{
		int y = 10 + 100 * i++;
		m_vecRoomButton[r.m_iRoomID]->SetRect({ 10, y });
		m_vecRoomButton[r.m_iRoomID]->SetActivate(true);
	}
}

void LobbyScene::UpdateRoom(std::list<Room> _rooms)
{
	for (Room r : m_RoomList)
	{
		m_vecRoomButton[r.m_iRoomID]->SetActivate(false);
	}
	m_RoomList.clear();

	std::list<Room>::iterator iter;

	for (iter = _rooms.begin(); iter != _rooms.end(); iter++)
	{
		AddRoom(*iter);
	}
	ShowRooms();
}


