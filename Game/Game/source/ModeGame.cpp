#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "Player.h" 
#include "Camera.h"
#include "Gun.h"
#include "mymath.h"
#include "ModeGameClear.h"
#include "ModeGameOver.h"

bool ModeGame::Initialize()
{
	if(!base::Initialize()) { return false; }
	// プレイヤーキャラを生成
	_player = new Player();
	_player->Initialize();

	_gun = new Gun();
	_gun->Initialize();
	_gun->SetScale(5.5f);

	// 銃をプレイヤーに装備（初期化時に一度だけ）
	_gun->AttachToPlayer(_player);
	

	// マップデータの読み込み
	_handleMap = MV1LoadModel("res/Dungeon/Dungeon.mv1");
	_handleSky = MV1LoadModel("res/SkySphere/skysphere.mv1");

	//コリジョン情報の生成
	_frameMapCollision = MV1SearchFrame(_handleMap, "dungeon_collision");
	MV1SetupCollInfo(_handleMap, _frameMapCollision, 16, 16, 16);
	//コリジョンのフレームを描画しない
	MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);

	_cameraTP = new Camera();
	_cameraTP->Initialize();
	//// カメラを生成
	//_camera = new Camera();
	//_camera->Initialize();
	
	// FPS カメラを生成（実体）
	_fCamera = new fpsCamera();
	_fCamera->Initialize();

	//初期アクティブカメラをTPSカメラに設定	
	_camera = _cameraTP;	

	// ここで両方向をつなぐ
	_camera->_player = _player;
	_cameraTP->_player = _player;
	_fCamera->_player = _player;
	_player->SetCamera(_camera);

	// その他初期化
	_bViewCollision = TRUE;

	//敵の生成
	_enemies.clear();
	for (int i = 0; i < 3; ++i)
	{
		Enemy* e = new Enemy();
		e->Initialize();
		e->SetTargetPlayer(_player);

		// 初期位置を個別に設定（例：中央・右・左）
		if (i == 0) e->_vPos = VGet(1000.0f, 0.0f, 0.0f);
		else if (i == 1) e->_vPos = VGet(1200.0f, 0.0f, 200.0f);
		else e->_vPos = VGet(800.0f, 0.0f, -200.0f);

		_enemies.push_back(e);
	}
	
	// ゲーム開始時刻リセット
	_gameElapsedSec = 0.0f;
	_gameClearShown = false;
	_gameOverShown = false; // 初期化
	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();

	if(_gun)
	{
		_gun->Terminate();
		delete _gun;
		_gun = nullptr;
	}

	if(_player)
	{
		_player->Terminate();
		delete _player;
		_player = nullptr;
	}

	// カメラ解放（実体をTerminateして delete）
	if(_cameraTP) { _cameraTP->Terminate(); delete _cameraTP; _cameraTP = nullptr; }
	if(_fCamera ) { _fCamera->Terminate();  delete _fCamera;  _fCamera  = nullptr; }

	_camera = nullptr;

	// 敵をすべて解放
	for(Enemy* e : _enemies)
	{
		if(e) { e->Terminate(); delete e; }
	}
	_enemies.clear();

	// マップ/空モデル解放
	if(_handleMap != -1)
	{
		MV1DeleteModel(_handleMap);
		_handleMap = -1;
	}
	if(_handleSky != -1)
	{
		MV1DeleteModel(_handleSky);
		_handleSky = -1;
	}
	_frameMapCollision = -1;

	return true;
}

bool ModeGame::Process()
{
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	_player->SetCamera(_camera);
	_player->Process(key, trg);

	if (_gun) { _gun->Process(key, trg);}
	PlayerCollision();

	// プレイヤーと敵の当たり判定
	for (Enemy* e : _enemies)
	{
		if (!e) continue;
		PlayerEnemyCollision(_player, e);
		// ゲームオーバー表示済みなら以降のチェックは不要（任意）
		if (_gameOverShown) break;
	}

	// ステルス処理：敵がプレイヤーを認識しているかチェック
	if(_player && _enemies.empty())
	{
		const float viewDistance = 300.0f;  // 敵がプレイヤーを認識する距離
		const float viewAngleDeg = 90.0f;   // 敵がプレイヤーを認識する視野角
		for (Enemy* e : _enemies)
		{
			if (!e) continue;
			bool seen = IsPlayerInEnemyView(_player, e, viewDistance, viewAngleDeg);
			e->SetDetected(seen);
		}
	}
	// 各敵を更新
	for (Enemy* e : _enemies)
	{
		if (e) e->Process();
	}
	CheckPlayerBulletHitsEnemy();

	// デバッグ機能
	if(trg & PAD_INPUT_6)
	{
		_bViewCollision = !_bViewCollision;
	}
	if(_bViewCollision)
	{
		MV1SetFrameVisible(_handleMap, _frameMapCollision, TRUE);
	}
	else
	{
		MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
	}

	// カメラ切り替え
	if(key & PAD_INPUT_5)
	{
		_fpsHoldTimerMs += GetStepTm();
		if(_fpsHoldTimerMs >= _fpsHoldThresholdMs)
		{
			// ボタンを押し続けている間はFPSカメラにする
			if(_camera != _fCamera)
			{
				_camera = _fCamera;

				// 関連付け更新（状態変化時のみ）
				if(_camera  ) { _camera->_player = _player;    }
				if(_cameraTP) { _player->SetCamera(_cameraTP); } // TPS参照を保持しておく
				if(_fCamera ) { _fCamera->_player = _player;   }
				_player->SetCamera(_camera);

				// プレイヤーに切替通知（必要に応じてアニメ等を切替）
				_player->OnSwitchToFPS();
			}
		}
		
	}
	else
	{
		_fpsHoldTimerMs = 0;
		// ボタンを離している間はTPSに戻す
		if(_camera != _cameraTP)
		{
			_camera = _cameraTP;

			// 関連付け更新（状態変化時のみ）
			if(_camera) { _camera->_player = _player; }
			if(_cameraTP) { _player->SetCamera(_cameraTP); }
			if(_fCamera) { _fCamera->_player = _player; }
			_player->SetCamera(_camera);

			// プレイヤーに切替通知
			_player->OnSwitchToTPS();
		}
	}
	// 両方のカメラを毎フレーム更新する（TPSは常にプレイヤーを追う必要があるため）
	if(_cameraTP)
	{
		if (_camera == _cameraTP) _cameraTP->Process(key, trg);
		else                      _cameraTP->FollowUpdate(); // 非アクティブ時は入力を無視して追従のみ

	}
	if(_fCamera)
	{
		if (_camera == _fCamera)  _fCamera->Process(key, trg);
		else                      _fCamera->FollowUpdate();
	}

	{
		// 1フレームの経過ms → 秒に変換して加算
		_gameElapsedSec += 1.0f / 60.0f;

		if(!_gameClearShown && !_gameOverShown && _gameElapsedSec >= GAMECLEAR_TIMESEC)
		{
			_gameClearShown = true;
			// 高レイヤーで追加してオーバーレイ表示
			ModeServer::GetInstance()->Add(new ModeGameClear(), 255, "ModeGameClear");
		}
	}

	return true;
}

bool ModeGame::Render()
{
	base::Render();
	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	//ライト設定
	SetUseLighting(TRUE);


	//プレイヤーの描画
	_player->Render();
	
	// 敵の描画
	for (Enemy* e : _enemies)
	{
		if (e) e->Render();
	}

	if(_bViewCollision)
	{
		// プレイヤー（既存）
		DrawLine3D(VAdd(_player->_vPos, VGet(0, _player->GetColSubY(), 0)), VAdd(_player->_vPos, VGet(0, -99999.f, 0)), GetColor(255, 0, 0));
		DrawSphere3D(_player->_vPos, _player->_collision_r, 10, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

		// 敵のコリジョン高さを可視化（上下の球＋中心線で円柱イメージ）
		for (Enemy* e : _enemies)
		{
			if (!e || !e->IsAlive()) continue;
			VECTOR top = VGet(e->_vPos.x, e->_vPos.y + e->_collision_h * 0.5f, e->_vPos.z);
			VECTOR bottom = VGet(e->_vPos.x, e->_vPos.y - e->_collision_h * 0.5f, e->_vPos.z);
			DrawSphere3D(top, e->_collision_r, 10, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
			DrawLine3D(top, bottom, GetColor(255, 0, 0));
		}
	}
	
	//マップの描画
	{
		MV1DrawModel(_handleMap);
		MV1DrawModel(_handleSky);
	}

	// 0,0,0を中心に線を引く
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	if(_gun) { _gun->Render(); }
	// カメラの設定（ビュー行列をセット -> これを描画の前に呼ぶ）
	if(_camera) { _camera->Render(); }
	// 敵の視界を個別描画
	for (Enemy* e : _enemies)
	{
		if (e && e->IsAlive())
		{
			DrawEnemyWithCamera(e, _camera);
		}
	}

	return true;
}

// プレイヤーのコリジョン処理
void ModeGame::PlayerCollision()
{
	// プレイヤーの位置を取得
	VECTOR playerPos    = _player->GetPos();
	VECTOR v            = _player->GetMoveV();
	float playercolsubY = _player->GetColSubY();
	VECTOR oldvPos		= playerPos;

	// 移動した先でコリジョン判定
	MV1_COLL_RESULT_POLY hitPoly;

	// 主人公の腰位置から下方向への直線
	hitPoly = MV1CollCheck_Line
	(_handleMap,
	 _frameMapCollision,
	  VAdd(_player->_vPos, VGet(0, _player->_colSubY, 0)),
	  VAdd(playerPos, VGet(0, -99999.f, 0)));

	if(hitPoly.HitFlag)
	{
		// 当たった
		// 当たったY位置をキャラ座標にする
		_player->_vPos.y = hitPoly.HitPosition.y;

		if(_camera)
		{
			// カメラも同じ分移動させる
			if(_cameraTP) { _cameraTP->MoveBy(v); }
			
			// FPSカメラも同じ分移動させる
			if(_fCamera && _fCamera != _cameraTP) { _fCamera->MoveBy(v); }
		}
	}
	else
	{
		// 当たらなかった。元の座標に戻す
		_player->_vPos = _player->oldPos;
	}

}
// プレイヤーと敵の当たり判定
void ModeGame::PlayerEnemyCollision(Player* pl, Enemy* en)
{
	if (!pl || !en) return;
	// 生存チェック（生きていない敵とは当たらない）
	if (!en->IsAlive()) return;

	// 円同士の当たり判定
	if (pl->IsHitCircle(pl, en))
	{
		// ゲームオーバー用オーバーレイを一度だけ表示
		if (!_gameOverShown)
		{
			_gameOverShown = true;
			ModeServer::GetInstance()->Add(new ModeGameOver(), 255, "ModeGameOver");
		}
	}
}

// ステルス関係
bool ModeGame::IsPlayerInEnemyView(Player* pl, const Enemy* en, float viewDistance, float viewAngleDeg)
{
	if(!pl || !en) return false;
	
	// プレイヤーの位置ベクトル
	VECTOR playerPos = pl->GetPos();
	// 敵の位置ベクトル
	VECTOR enemyPos  = en->GetPos();
	float dx		 = playerPos.x - enemyPos.x;
	float dz		 = playerPos.z - enemyPos.z;
	float dist2		 = dx * dx + dz * dz;
	//距離のチェック
	const float viewDist2 = viewDistance * viewDistance;
	if(dist2 > viewDist2) return false;
	//敵の前方ベクトル
	VECTOR enemyDir = en->GetDir();
	float  enemy_x  = enemyDir.x;
	float  enemy_z	= enemyDir.z;
	float  enemyLen = sqrtf(enemy_x * enemy_x + enemy_z * enemy_z);
	if(enemyLen < 1e-6f)
	{
		enemy_x = 0.0f; enemy_z = 1.0f; enemyLen = 1.0f;
	}
	enemy_x /= enemyLen;
	enemy_z /= enemyLen;
	// 内積で前方かどうか判定
	float dot = dx * enemy_x + dz * enemy_z;
	if(dot <= 0.0f) return false; // 後ろ側
	// 角度のチェック
	const float halfAngleRad = DEG2RAD(viewAngleDeg * 0.5f);
	const float cosT		 = cosf(halfAngleRad);
	if((dot * dot) < (dist2 * cosT * cosT)) return false;
	// 視線上にマップの障害物があるかチェックはここに

	// ここまで来た見えている見えている
	return true;
}

// 敵の視界を描画する
void ModeGame::DrawEnemyWithCamera(const Enemy* en, const Camera* cam)
{
	if(!en) return;
	// camは将来の拡張用
	(void)cam;
	// 視界パラメータ
	const float viewDistance = 300.0f;             // 敵がプレイヤーを認識する距離
	const float viewAngleDeg = 90.0f;               // 敵がプレイヤーを認識する視野角
	const int   color		 = GetColor(255, 0, 0); // 色
	const int arcDiv = 24;							// 分割数         
	// 敵の位置と描画の高さ
	VECTOR enemyPos = en->GetPos();
	float  drawY    = enemyPos.y + 100.0f;
	// 敵の向きベクトルから角度を算出
	VECTOR enemyDir     = en->GetDir();
	float  baseAngle    = atan2f(enemyDir.z, enemyDir.x);
	float  halfAngleRad = DEG2RAD(viewAngleDeg * 0.5f);
	//視界の円弧を分割して3D線で描画
	VECTOR enemyDrawPos = VGet(enemyPos.x, drawY, enemyPos.z);
	for(int i = 0; i < arcDiv; ++i)
	{
		float a1 = baseAngle - halfAngleRad + (halfAngleRad * 2.0f * i) / arcDiv;
		float a2 = baseAngle - halfAngleRad + (halfAngleRad * 2.0f * (i + 1)) / arcDiv;
		VECTOR point1 = VGet(enemyPos.x + viewDistance * cosf(a1), drawY, enemyPos.z + viewDistance * sinf(a1));
		VECTOR point2 = VGet(enemyPos.x + viewDistance * cosf(a2), drawY, enemyPos.z + viewDistance * sinf(a2));
		DrawLine3D(enemyDrawPos, point1, color);
		// 円弧のセグメント
		if(i == 0)			DrawLine3D(enemyDrawPos, point1, color);
		if(i == arcDiv - 1) DrawLine3D(enemyDrawPos, point2, color);
	}
	// 視界中心線
	VECTOR center = VGet(enemyPos.x + viewDistance * cosf(baseAngle), drawY, enemyPos.z + viewDistance * sinf(baseAngle));
	DrawLine3D(enemyDrawPos, center, color);
}

void ModeGame::CheckPlayerBulletHitsEnemy()
{
	if (!_player) return;
	if (!_player->_bullet || !_player->_bullet->IsActive()) return;

	//弾の線分
	const VECTOR bStart = _player->_bullet->_vPos;
	const VECTOR bEnd = VAdd(bStart, VScale(_player->_bullet->_dir, _player->_bullet->_length));

	// プレイヤーの弾が当たったら1体だけヒットさせる（ヒットしたらループを抜ける）
	for (Enemy* e : _enemies)
	{
		if (!e || !e->IsAlive()) continue;

		const VECTOR enemyPos = e->GetPos();
		const float	 halfHeight = e->_collision_h * 0.5f;
		const float  radius = e->_collision_r;

		const VECTOR centerMid = enemyPos;
		const VECTOR centerTop = VGet(enemyPos.x, enemyPos.y + halfHeight, enemyPos.z);
		const VECTOR centerBottom = VGet(enemyPos.x, enemyPos.y - halfHeight, enemyPos.z);

		if(HitCheck_Line_Sphere(bEnd, bStart, centerMid,    radius) ||
		   HitCheck_Line_Sphere(bEnd, bStart, centerTop,    radius) ||
		   HitCheck_Line_Sphere(bEnd, bStart, centerBottom, radius)
		  )
		{
			//当たった
			e->ApplyDamage(100); //ダメージを与える
			_player->_bullet->Deactive(); //弾を非アクティブにする
			break;
		}
	}
}