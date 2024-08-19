#include "pch.h"
#include "MainScene.h"
#include "Texture.h"
#include "Button.h"
#include"SceneManager.h"
#include "InputManager.h"
 

MainScene::MainScene(std::string _strName) : Scene(_strName)
{
	m_pBackGround = nullptr;
}

MainScene::~MainScene()
{
}

void MainScene::Init()
{
	m_pBackGround = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::MAINSCENE);
	m_pTitle = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::TITLE);
	Scene::SetWindowSize(m_pBackGround->GetWidth(), m_pBackGround->GetHeight());

	Button* PlayButton = new Button;
	PlayButton->Init({ m_pBackGround->GetWidth() / 4 - 30 , m_pBackGround->GetHeight()/ 4}, TEXTURE_TYPE::BUTTON_PLAY, std::bind(&MainScene::SelectStage, this), "Play");
	PlayButton->SetActivate(true);
	Scene::AddObject(PlayButton, OBJECT_GROUP::BUTTON);
	
}

void MainScene::Update()
{
	Scene::Update();
}

void MainScene::Render(HDC _memDC)
{
	BitBlt(_memDC, 0, 0, m_pBackGround->GetWidth(), m_pBackGround->GetHeight(),
		m_pBackGround->GetDC(), 0, 0, SRCCOPY);
	TransparentBlt(_memDC, 80, 20, m_pTitle->GetWidth(), m_pTitle->GetHeight(), m_pTitle->GetDC(),
		0, 0, m_pTitle->GetWidth(), m_pTitle->GetHeight(), RGB(255, 0, 255));
	//TransparentBlt(_memDC, 480, 200, m_pPlayButton->GetWidth(), m_pPlayButton->GetHeight(),m_pPlayButton->GetDC(), 0, 0, m_pPlayButton->GetWidth(), m_pPlayButton->GetHeight(), RGB(255, 0, 255));
	Scene::Render(_memDC);
}

void MainScene::SelectStage()
{
	SceneManager::GetInstance()->SceneChange(SCENE_TYPE::LOBBY);
}
