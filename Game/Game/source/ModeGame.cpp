#include "ModeGame.h"
#include "ModeMenu.h"
#include "ModeTitle.h"
#include "BulletManager.h"
#include "EnemyBoss.h"
#include "MinionBase.h"


bool ModeGame::Initialize()
{
	if(!base::Initialize()) { return false; }

	// オブジェクト生成
	_cam      = _objFtr.CreateCamera();
	_player   = _objFtr.CreatePlayer();
	auto boss = _objFtr.CreateEnemyBoss(_player.get(), [this](std::unique_ptr<EnemySpawner> newSpawner)
		{
			_spawners.push_back(std::move(newSpawner));
		});

	if(boss)
	{
		_enemies.push_back(std::move(boss));
	}

	BulletManager::GetInstance()->Initialize();

	if(!_cam || !_player || _enemies.empty())
	{
		return false;
	}

	_bg = std::make_unique<Background2D>();
	_bg->Initialize("res/BG/Casul.png");

	CameraManager::GetInstance()->SetActiveCamera(_cam.get());

	// カメラ：プレイヤー追従設定
	_objFtr.SetUpCamera(_cam.get(), _player.get(), true);

	// 敵：プレイヤー追従設定
	for(auto& enemy : _enemies)
	{
		_objFtr.SetUpEnemy(enemy.get(), _player.get());
	}	

	// HUD追加
	AddHUD();

	// ステージ読み込み
	StageLoader::LoadStage("res/Map/Map.json", _objFtr, _objMgr);

	// メニュー初期化
	_menuCtrl.Initialize();

	// ゲームオーバー関連
	_gameOverShown = false;
	return true;
}

bool ModeGame::Terminate()
{
	_menuCtrl.Terminate();

	_objMgr.TerminateAll();

	if(_player) {_player->Terminate(); _player.reset(); }
	for(auto& enemy : _enemies)
	{
		enemy->Terminate();
		enemy.reset();
	}
	if(_cam   ) {_cam->Terminate();    _cam.reset();    }
	if(_bg    ) {_bg->Terminate();     _bg.reset();     }

	BulletManager::GetInstance()->Terminate();

	DelHUD();

	base::Terminate();
	return true;
}

bool ModeGame::Process()
{
	base::Process();

	_menuCtrl.Process();

	// アニメーション更新
	SpriteAnimationManager::GetInstance()->Update(1.0f / 60.0f);

	// ゲーム進行状態更新
	UpdatePhase();
	return true;
}

bool ModeGame::Render()
{
	if(!_cam) { return false; }

	SetupRenderState();
	SetupCamera();
	Render3DObjects();

	return true;
}

void ModeGame::CheckCharaMapCollision()
{
	// マネージャー内のすべてのオブジェクトをループ
	for(auto& obj : _objMgr.GetObjects())
	{
		Cube* cube = dynamic_cast<Cube*>(obj.get());
		if(cube)
		{
			// プレイヤー
			if(_player) { _collision.CheckCharacterCube(_player.get(), cube); }

			// 敵
			for(const auto& enemy : _enemies)
			{
				if(enemy) { _collision.CheckCharacterCube(enemy.get(), cube); }
			}
		}
	}
}

void ModeGame::AddHUD()
{
	// ModeServerからModeHUDを取得
	ModeHUD* hud = dynamic_cast<ModeHUD*>(ModeServer::GetInstance()->Get("ModeHUD"));

	if(!hud)
	{
		hud = new ModeHUD();
		ModeServer::GetInstance()->Add(hud, 200, "ModeHUD");
	}

	if(hud && _player)
	{
		hud->SetPlayer(_player.get());
	}
}

void ModeGame::DelHUD()
{
	ModeHUD* hud = dynamic_cast<ModeHUD*>(ModeServer::GetInstance()->Get("ModeHUD"));
	if(hud)
	{
		ModeServer::GetInstance()->Del(hud);
	}
}

void ModeGame::UpdatePhase()
{
	switch(_phase)
	{
	case GamePhase::Playing:
		UpdatePlaying();
		break;

	case GamePhase::GameOverAnim:
		UpdateGameOverAnim();
		break;

	case GamePhase::GameOverUI:
		// UI表示中は何もしない
		break;
	case GamePhase::GameClearAnim:
		UpdateGameClearAnim();
		break;

	case GamePhase::GameClearUI:
		// UI表示中は何もしない
		break;
	}

}

void ModeGame::UpdatePlaying()
{
	UpdateGameLogic();

	// 死亡検知
	if(_player && _player->IsDead())
	{
		_phase = GamePhase::GameOverAnim;
		_gameOverTimer = 0.0f;

		// プレイヤーを死亡状態に変更
		_player->SetStatus(CharaBase::STATUS::DIE);
	}

	for(const auto& enemy : _enemies)
	{
		if(enemy && enemy->IsDead())
		{
			_phase = GamePhase::GameClearAnim;
			_gameClearTimer = 0.0f;

			// 敵を死亡状態に変更
			enemy->SetStatus(CharaBase::STATUS::DIE);
			return;
		}
	}
}

void ModeGame::UpdateGameOverAnim()
{
	// プレイヤーとカメラの更新だけ行う
	if(_player) { _player->Process(); }
	if(_cam) { _cam->Process(); }

	// 演出タイマー
	_gameOverTimer += 1.0f / 60.0f;
	if(_gameOverTimer >= 2.0f)
	{
		_phase = GamePhase::GameOverUI;

		// UI表示
		ModeServer::GetInstance()->Add(new ModeGameOver(), 300, "ModeGameOver");
	}
}

void ModeGame::UpdateGameClearAnim()
{
	// カメラの更新だけ行う
	if(_player) { _player->Process(); }
	if(_cam) { _cam->Process(); }

	// 演出タイマー
	_gameClearTimer += 1.0f / 60.0f;
	if(_gameClearTimer >= 2.0f)
	{
		_phase = GamePhase::GameClearUI;
		// UI表示
		ModeServer::GetInstance()->Add(new ModeGameClear(), 300, "ModeGameClear");
	}
}

void ModeGame::UpdateGameLogic()
{
	if(_player) { _player->Process(); }
	if(_cam	  ) { _cam->Process();    }

	for(auto it = _spawners.begin(); it != _spawners.end();)
	{
		(*it)->Update(1.0f / 60.0f, [this](std::unique_ptr<EnemyBase> newEnemy)
		{
			if(newEnemy)
			{
				newEnemy->Initialize();
				_objFtr.SetUpEnemy(newEnemy.get(), _player.get());
				_enemies.push_back(std::move(newEnemy));
			}
		});

		if((*it)->IsDead())
		{
			it = _spawners.erase(it);
		}
		else
		{
			++it;
		}
	}

	for(auto& enemy : _enemies)
	{
		if(enemy) { enemy->Process(); }
	}

	BulletManager::GetInstance()->Process();

	// 当たり判定
	for(auto& enemy : _enemies)
	{
		if(!enemy) continue;
		_collision.CheckPlayerEnemy(_player.get(), enemy.get());
		_collision.CheckPlayerAttack(_player.get(), enemy.get());
	}
	CheckCharaMapCollision();

	if(_player && !_enemies.empty())
	{
		auto& bullets = BulletManager::GetInstance()->GetBullets();

		// プレイヤー弾とボスの判定
		for(const auto& bullet : bullets)
		{
			if(!bullet || !bullet->IsActive()) continue;

			// プレイヤーの通常弾とボスの判定を行う
			if(!dynamic_cast<EnemyReflectBullet*>(bullet.get()))
			{
				for(auto& enemy : _enemies)
				{
					if(enemy) { _collision.CheckPlayerBulletEnemy(bullet.get(), enemy.get()); }
				}
			}
		}

		// ラリー弾とプレイヤー / ボス の判定
		for(const auto& bullet : bullets)
		{
			if(!bullet || !bullet->IsActive()) continue;

			auto* reflectBullet = dynamic_cast<EnemyReflectBullet*>(bullet.get());
			if(reflectBullet)
			{
				_collision.CheckPlayerReflectBullet(_player.get(), reflectBullet);
				_collision.CheckPlayerAttackBullet(_player.get(), reflectBullet);

				for(auto& enemy : _enemies)
				{
					if(enemy) { _collision.CheckBulletEnemy(reflectBullet, enemy.get()); }
				}
			}
		}

		// プレイヤー弾とラリー弾の判定
		for(const auto& pBullet : bullets)
		{
			if(!pBullet || !pBullet->IsActive()) continue;
			if(dynamic_cast<EnemyReflectBullet*>(pBullet.get())) continue;

			for(const auto& rBullet : bullets)
			{
				if(!rBullet || !rBullet->IsActive()) continue;
				auto* reflectBullet = dynamic_cast<EnemyReflectBullet*>(rBullet.get());

				if(reflectBullet)
				{
					_collision.CheckPlayerBulletWithReflectBullet(_player.get(), pBullet.get(), reflectBullet);
				}
			}
		}
	}

	// オブジェクトマネージャーの全オブジェクトを処理
	_objMgr.ProcessAll();
}

void ModeGame::SetupRenderState()
{
	// 画面クリア
	SetBackgroundColor(64, 64, 64);
	ClearDrawScreen();

	base::Render();

	if(_bg)
	{
		_bg->Render();
	}

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ライト設定
	SetUseLighting(TRUE);
}

void ModeGame::SetupCamera()
{
	VECTOR pos    = _cam->GetPos();
	VECTOR target = _cam->GetTarget();

	SetCameraPositionAndTarget_UpVecY(pos, target);
	SetCameraNearFar(_cam->GetClipNear(), _cam->GetClipFar());

	// 0,0,0 を中心にしたデバッグ用軸線
	/*float linelength = 1000.f;
	VECTOR v = { 0, 0, 0 };
	DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
	DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
	DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));*/
}

void ModeGame::Render3DObjects()
{
	if(_player) { _player->Render(); }
	for(auto& enemy : _enemies)
	{
		if(enemy) { enemy->Render(); }
	}
	for(auto& spawner : _spawners)
	{
		if(spawner) { spawner->Render(); }
	}

	BulletManager::GetInstance()->Render();

	// _objMgr.RenderAll();

	if(_cam) { _cam->Render(); }

	// デバッグ
	// _collision.DebugRenderCapsule(_player.get(), _enemy.get());
}