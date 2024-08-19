#pragma once
#include "Stone.h"
#include "Object.h"
class GameBoard : public Object
{
private:
	bool m_bMyTurn;
	bool m_bGameEnd;
	Stone m_Board[BOARD_SCALE::HEIGHT][BOARD_SCALE::WIDTH];
	Protocol::STONE_COLOR m_eMyColor;
	Protocol::STONE_COLOR m_eEnemyColor;
public:
	GameBoard();
	~GameBoard();
	void Init();
	void InitStone();
	void Reset();
	virtual void Update();
	void StoneCheck(POINT _pt, bool _isClick);
	void WinCheck(Vector2 _pos);
	int LineCheck(Vector2 _pos, Vector2 _dir);
	bool isMyturn() { return m_bMyTurn; }
	void StartGame(Protocol::STONE_COLOR _color);
	void EnemyInput(Vector2 _pos);
	void TurnBack(Vector2 _pos);
	virtual void Render(HDC _memDC);
	void DrawBoard(HDC _memDC);

	void End() { m_bGameEnd = true; }
	bool isEnd() { return m_bGameEnd; }
};
// 게임매니저 없이, 서버에서도 윈체크랑 돌놓는거체크, 돌 오브젝트로 추가 안하고 보드 오브젝트를 만들어서 보드가 돌의 업데이트를 돌린다
// 게임이 끝나서 방이 없어지는거 broadcast;, 두명 입장시 레디버튼이랑 나가기버튼 기능추가 
