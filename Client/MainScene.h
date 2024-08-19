#pragma once
#include"Scene.h"
#include"ResourceManager.h"
class MainScene : public Scene
{
private:
	Texture* m_pBackGround;
	Texture* m_pTitle;
	Vector2 m_vec2BackGroundPosition;
public:
	MainScene(std::string _strName);
	~MainScene();
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _memDC) override;

	void SelectStage();
};

