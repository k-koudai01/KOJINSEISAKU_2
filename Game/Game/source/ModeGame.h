#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "ObjectFactry.h"
#include "ObjectManager.h"
#include "Player.h" 
#include "Enemy.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "CameraManager.h"
#include "mymath.h"
#include "ModeGameClear.h"
#include "ModeGameOver.h"
#include "ModeHUD.h"
#include "MenuController.h"
#include "StageLoader.h"


constexpr float GAMECLEAR_TIMESEC = 30.0f;  // クリアまでの秒数

class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:

	// ゲームの進行状態
	enum class GamePhase
	{
		Playing,      // プレイ中
		GameOverAnim, // ゲームオーバー演出
		GameOverUI,   // ゲームオーバーUI表示中
	};

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	// キャラクターとマップの当たり判定
	void CheckCharaMapCollision();

	Player* GetPlayer() const { return _player.get(); }
protected:
	// オブジェクト
	ObjectFactry  _objFtr;           // オブジェクトファクトリー
	ObjectManager _objMgr;           // オブジェクトマネージャー

	std::unique_ptr<Camera> _cam;    
	std::unique_ptr<Player> _player;
	std::unique_ptr<Enemy> _enemy;

	// 当たり判定
	CollisionManager _collision;

	// ゲームオーバー表示済みフラグ
	bool _gameOverShown = false;
	
private:
	void AddHUD();
	void DelHUD();

	void UpdatePhase();
	void UpdatePlaying();      // 通常プレイ中の処理
	void UpdateGameOverAnim(); // 死亡演出中の処理
	void UpdateGameLogic();    // ゲーム自体の更新処理(通常ゲーム処理の時だけ呼ばれる)

	// 描画用ヘルパー関数
	void SetupRenderState(); // 3D/ライト等の基本設定
	void SetupCamera();		 // カメラ適用&デバック軸線描画
	void Render3DObjects();	 // 各オブジェクトの描画

private:
	MenuController _menuCtrl;

	GamePhase _phase     = GamePhase::Playing;
	float _gameOverTimer = 0.0f; 

};
