#include "pch.h"
#include "GameScene.h"
#include"SceneManager.h"
#include"ResourceManager.h"
#include "GameCore.h"

GameScene::GameScene(std::string _strName) : Scene(_strName)
{
	m_pBackGround = nullptr;
	
}

GameScene::~GameScene()
{

}

void GameScene::Init()
{
	m_pBackGround = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::GAMEBOARD);
	Scene::SetWindowSize(575, 650);

	m_ReadyButton = new Button;
	m_ExitButton = new Button;
	m_RegameButton = new Button;
	Scene::AddObject(m_ReadyButton, OBJECT_GROUP::BUTTON);
	Scene::AddObject(m_ExitButton, OBJECT_GROUP::BUTTON);
	Scene::AddObject(m_RegameButton, OBJECT_GROUP::BUTTON);

	m_ReadyButton->Init({ 160, 200 }, TEXTURE_TYPE::BUTTON_READY,
		[=](){
			Protocol::C_READY packet;
			packet.set_ready(true);
			NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
			m_strCurState = "준비완료.. 상대 : 준비중..";
			m_ReadyButton->SetActivate(false);
		}, "ready");
	m_RegameButton->Init({ 160, 200 }, TEXTURE_TYPE::BUTTON_REGAME, [=]() {
		GameScene::Reset();
		Protocol::C_REGAME packet;
		NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
		}, "regame");

	m_ExitButton->Init({ 525, 590 }, TEXTURE_TYPE::BUTTON_OUT, std::bind(&GameScene::ExitRoom, this), "exit");
	m_RegameButton->SetActivate(false);
	m_GameBoard = new GameBoard;
	Scene::AddObject(m_GameBoard, OBJECT_GROUP::BOARD);
	m_GameBoard->SetActivate(false);
	Reset();
}

void GameScene::Update()
{
	
	Scene::Update();
}


void GameScene::Render(HDC _memDC)
{
	BitBlt(_memDC, 0, 0, m_pBackGround->GetWidth(), m_pBackGround->GetHeight(),
		m_pBackGround->GetDC(), 0, 0, SRCCOPY);

	std::string strErase = "                                                                                    ";
	TextOut(_memDC, 200, 580, strErase.c_str(), strErase.length());
	TextOut(_memDC, 200, 580, m_strCurState.c_str(), m_strCurState.length());
	if (m_GameBoard->is_Activated())
	{
		if (m_GameBoard->isMyturn())
			m_strTurn = "◀◀◀";
		else
			m_strTurn = "▶▶▶";
		TextOut(_memDC, 282, 580, m_strTurn.c_str(), m_strTurn.length());
	}

	Scene::Render(_memDC);
}

void GameScene::Reset()
{
	m_strCurState = "상대 : 입장대기중";
	m_ReadyButton->SetActivate(false);
	m_ExitButton->SetActivate(true);
	m_RegameButton->SetActivate(false);

	m_GameBoard->Reset();
	m_GameBoard->SetActivate(false);
}

void GameScene::StartGame(Protocol::STONE_COLOR _color)
{
	m_GameBoard->Reset();
	m_GameBoard->SetActivate(true);

	if (_color == Protocol::STONE_COLOR::BLACK)
	{
		m_strCurState = "나 : BLACK                  상대 : WHITE";
	}
	else
	{
		m_strCurState = "나 : WHITE                  상대 : BLACK";
	}
	m_GameBoard->StartGame(_color);
	m_GameBoard->Init();
}



void GameScene::ExitRoom()
{
	Protocol::C_EXITROOM packet;

	NetworkManager::GetInstance()->SendPacket(ServerPacketHandler::SendPacketTest(packet));
	SceneManager::GetInstance()->SceneChange(SCENE_TYPE::LOBBY);
}

void GameScene::EnemyInput(Vector2 _pos)
{	
	m_GameBoard->EnemyInput(_pos);
}

void GameScene::EndGame(Protocol::GAME_OVER _result, Vector2 _pos)
{
	m_strCurState.clear();
	switch (_result)
	{
	case Protocol::GAME_OVER::WIN:
		m_strCurState = "승리!!";
		break;
	case Protocol::GAME_OVER::LOSE:
		m_strCurState = "패배";
		m_GameBoard->EnemyInput(_pos);
		break;
	case Protocol::GAME_OVER::EXIT:
		m_strCurState = "방을 나갑니다";
		break;
	}
	m_GameBoard->End();
	m_GameBoard->SetActivate(false);
	m_RegameButton->SetActivate(true);
	
}

void GameScene::TurnBack(Vector2 _pos)
{
	m_GameBoard->TurnBack(_pos);
}

void GameScene::EnemyEnter(bool _enter)
{
	if (_enter)
	{
		m_strCurState = "상대 : 입장";
		m_ReadyButton->SetActivate(true);
		m_RegameButton->SetActivate(false);
	}
	else
	{
		m_strCurState = "상대 : 입장대기중";
		m_ReadyButton->SetActivate(false);
	}
}

void GameScene::EnemyReady()
{
	m_strCurState = "상대 : 준비완료";
}
