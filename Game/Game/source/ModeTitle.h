#pragma once
#include "appframe.h"
#include "ObjectFactry.h"
#include "Player.h"
#include "Camera.h"
#include "UITitleMenu.h"
#include "SakuraEmitter.h"

namespace UI
{
}

class ModeTitle : public ModeBase
{
	typedef ModeBase base;        
public:
	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

protected:
	void InitTitleCamera();
	void UpdateTitlePlayer();
	void RenderBackground3D();
	void ProcessMenuSelection();

	// オブジェクト
	ObjectFactry _objFtr;
	std::unique_ptr<Player> _player;
	std::unique_ptr<Camera> _cam;
	std::unique_ptr<UITitleMenu> _titleUI;

	// ハンドル
	int _bgHandle = -1;

private:
	SakuraEmitter _sakuraEmitter;
};

