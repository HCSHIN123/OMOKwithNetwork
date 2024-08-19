#pragma once
#include "Scene.h"
#include "GameBoard.h"
#include "Button.h"
class Button;
class GameBoard;
class Texture;

class GameScene : public Scene
{
private:
	int m_iCurRoomID;
	std::string m_strCurState;
	std::string m_strTurn;
	Texture* m_pBackGround;
	GameBoard* m_GameBoard;
	Button* m_ReadyButton;
	Button* m_ExitButton;
	Button* m_RegameButton;
public:
	GameScene(std::string _strName);
	~GameScene();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _memDC) override;

	void Reset();
	void StartGame(Protocol::STONE_COLOR _color);
	void ExitRoom();
	void EnemyInput(Vector2 _pos);
	void EndGame(Protocol::GAME_OVER _result, Vector2 _pos);
	void TurnBack(Vector2 _pos);
	void EnemyEnter(bool _enter);
	void EnemyReady();
};

