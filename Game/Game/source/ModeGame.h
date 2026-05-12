#pragma once
#include "appframe.h"
#include "ApplicationMain.h"
#include "Player.h" 
#include "Camera.h"
#include "fpsCamera.h"
#include "Enemy.h"
#include <string>

class Gun;
// モード

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
	// プレイヤーと敵の当たり判定
	void PlayerEnemyCollision(Player* _pl, Enemy* _en);
	//ステルス関係
	bool IsPlayerInEnemyView(Player* pl, const Enemy* en, float viewDistance, float viewAngleDeg);
	void DrawEnemyWithCamera(const Enemy* en, const Camera* cam);

	Player* _player;
	std::vector<Enemy*> _enemies;
	Camera* _camera;
	Camera* _cameraTP;
	fpsCamera* _fCamera;
	Gun* _gun = nullptr;

protected:
	//マップ用
	int _handleMap;
	int _handleSky;
	int _frameMapCollision;
	// デバッグ用
	bool	_bViewCollision;

private:
	// プレイヤーの弾が敵に当たったかどうかチェック
	void CheckPlayerBulletHitsEnemy();
	//　長押しの際にディレイを入れる
	// unsigned long型はms単位の時間計測に使う(負の値を取らない)
	unsigned long _fpsHoldTimerMs = 0;      // 経過時間計測用タイマー
	unsigned long _fpsHoldThresholdMs = 150;// 押し続けてから切り替わるまでの時間
	// 敵の感知後に追跡を続けるためのタイマー
	// プレイヤーが視界に入った瞬間からこの時間
	unsigned long _enemyDetectTimerMs    =     0; 
	unsigned long _enemyDetectDurationMs = 10000; //１０秒

	// ゲーム開始時刻（ms）・クリア表示済みフラグ
	unsigned long _gameStartMs = 0;
	bool _gameClearShown = false; 

	// ゲームオーバー表示済みフラグ（敵とプレイヤーが触れたときに一度だけ表示する）
	bool _gameOverShown = false;

	// クリア判定（経過秒で管理）
	float _gameElapsedSec = 0.0f;     // 開始からの経過秒
	
};
