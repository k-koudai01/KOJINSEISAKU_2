#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "ObjectFactry.h"
#include "ObjectManager.h"
#include "Player.h" 
#include "Enemy.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "mymath.h"
#include "ModeGameClear.h"
#include "ModeGameOver.h"
#include "ModeHUD.h"
#include "MenuController.h"

constexpr float GAMECLEAR_TIMESEC = 30.0f;  // クリアまでの秒数

class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	// プレイヤーとマップの当たり判定
	void PlayerCollision();

	Player* GetPlayer() const { return _player.get(); }
protected:
	// オブジェクト
	ObjectFactry  _objFtr;           // オブジェクトファクトリー
	ObjectManager _objMgr;           // オブジェクトマネージャー

	std::unique_ptr<Camera> _cam;    
	std::unique_ptr<Player> _player;
	std::unique_ptr<Enemy> _enemy;

	CollisionManager _collision;

	// ゲーム開始時刻（ms）・クリア表示済みフラグ
	unsigned long _gameStartMs = 0;
	bool _gameClearShown = false; 

	// ゲームオーバー表示済みフラグ
	bool _gameOverShown = false;

	// クリア判定
	float _gameElapsedSec = 0.0f;     // 開始からの経過秒
	
private:
	void AddHUD();

	MenuController _menuCtrl;
};
