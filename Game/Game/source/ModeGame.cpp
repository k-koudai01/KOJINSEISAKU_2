#include "ModeGame.h"
#include "ModeMenu.h"
#include "ModeTitle.h"
#include "BulletManager.h"

bool ModeGame::Initialize()
{
	if(!base::Initialize()) { return false; }

	// オブジェクト生成
	_cam    = _objFtr.CreateCamera();
	_player = _objFtr.CreatePlayer();
	_enemy  = _objFtr.CreateEnemy();
	BulletManager::GetInstance()->Initialize();

	if(!_cam || !_player || !_enemy)
	{
		return false;
	}

	_bg = std::make_unique<Background2D>();
	_bg->Initialize("res/Title/origbig.png");

	CameraManager::GetInstance()->SetActiveCamera(_cam.get());

	// カメラ：プレイヤー追従設定
	_objFtr.SetUpCamera(_cam.get(), _player.get(), true);

	// 敵：プレイヤー追従設定
	_objFtr.SetUpEnemy(_enemy.get(), _player.get());

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
	if(_enemy ) { _enemy->Terminate();  _enemy.reset(); }
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
			if(_enemy) { _collision.CheckCharacterCube(_enemy.get(), cube); }
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

	if(_enemy && _enemy->IsDead())
	{
		_phase = GamePhase::GameClearAnim;
		_gameClearTimer = 0.0f;

		// 敵を死亡状態に変更
		_enemy->SetStatus(CharaBase::STATUS::DIE);
		return;
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
	if(_enemy ) { _enemy->Process();  }
	if(_cam	  ) { _cam->Process();    }
	BulletManager::GetInstance()->Process();

	// 当たり判定
	_collision.CheckPlayerEnemy(_player.get(), _enemy.get());
	_collision.CheckPlayerAttack(_player.get(), _enemy.get());
	CheckCharaMapCollision();

	if(_player && _enemy)
	{
		// BulletManager 内の全弾をループチェック
		for(const auto& bullet : BulletManager::GetInstance()->GetBullets())
		{
			if(!bullet || !bullet->IsActive()) continue;

			// ラリー弾かどうかをキャストで判別
			auto* reflectBullet = dynamic_cast<EnemyReflectBullet*>(bullet.get());
			if(reflectBullet)
			{
				// 敵のラリー弾 vs プレイヤー
				_collision.CheckPlayerReflectBullet(_player.get(), reflectBullet);

				// プレイヤーの近接攻撃 vs ラリー弾
				_collision.CheckPlayerAttackBullet(_player.get(), reflectBullet);

				// 打ち返されたラリー弾 vs ボス
				_collision.CheckBulletEnemy(reflectBullet, _enemy.get());
			}
			// プレイヤーの通常弾・チャージ弾などの場合
			else
			{
				// プレイヤー弾 vs ラリー弾の押し返し判定を行いたい場合
				for(const auto& targetBullet : BulletManager::GetInstance()->GetBullets())
				{
					auto* targetReflect = dynamic_cast<EnemyReflectBullet*>(targetBullet.get());
					if(targetReflect)
					{
						// プレイヤー遠距離弾 vs ラリー弾
						_collision.CheckPlayerBulletWithReflectBullet(bullet.get(), targetReflect);
					}
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
	if(_enemy ) { _enemy->Render();  }

	BulletManager::GetInstance()->Render();

	// _objMgr.RenderAll();

	if(_cam) { _cam->Render(); }

	// デバッグ
	// _collision.DebugRenderCapsule(_player.get(), _enemy.get());
}