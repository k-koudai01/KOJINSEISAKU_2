#pragma once
#include "CharaBase.h"
#include "Bullet.h"
#include <vector>

class Player;
class Enemy :public CharaBase
{
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;
	//セッター
	void SetTargetPlayer(Player* p) { _target = p; }
	void SetSpeed(float s) { speed = s; }
	// プレイヤーに気づいたかどうか
	void SetDetected(bool Detected) { playerDetected = Detected; }
	bool IsDetected()         const { return playerDetected; }	

	// ダメージ / 生存判定
	void ApplyDamage(int dmg);
	bool IsAlive() const { return _alive; }

	// 死亡時の処理
	void OnDeath();
	// リスポーン処理
	void Respawn();
protected:
	int _handle = -1;
	int _attach_index = -1;
	float _total_time = 0.f;
	float _play_time = 0.f;
	//ステータス
	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		_EOT_
	};
	STATUS _status = STATUS::NONE;
	// 基礎変数
	float _colSubY = 40.0f;
	VECTOR _v = VGet(0, 0, 0);
	float speed = 1.5f;
	Player* _target = nullptr;
	// プレイヤー発見フラグ
	bool playerDetected = false;
	// 弾関連
	std::vector<Bullet*> _bullets;
	float   _shootCooldown = 1.5f; // 発射間隔
	float   _shootTimer = 0.0f;    // 発射タイマー
	unsigned long _lastTimeMs = 0;

	// 体力・生存フラグ
	int _hp     = 100;
	int _maxHp  = 100;
	bool _alive = true;

	// リスポーン関連
	float _respawnDelay   = 5.0f;    // 死亡からリスポーンまでの時間（秒）
	float _respawnTimer   = 0.0f;    // カウントダウン用
	float _respawnMinDist = 300.0f;    // プレイヤーから最低この距離以上離して出現
	float _respawnMaxDist = 600.0f; // プレイヤーから最大この距離以内でランダムに出現
};

