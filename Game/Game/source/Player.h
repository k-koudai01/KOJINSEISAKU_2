#pragma once

#include "SpriteCharaBase.h"
#include "Camera.h"
#include "Bullet.h"
#include "BulletManager.h"


class Player: public SpriteCharaBase
{	
	typedef SpriteCharaBase base;
public:
	// ----------------------------------------------------
	// 基本関数
	// ----------------------------------------------------
	virtual bool Initialize()override;
	virtual bool Terminate() override;
	virtual bool Process()   override;
	virtual bool Render()    override;

	// カメラ設定
	Camera* GetCamera() const   { return _cam; }
	void SetCamera(Camera* cam) { _cam = cam; }

	//　アクション関連
	bool IsAttacking() const { return _isAttacking; } 
	bool IsAttackActive() const;
	virtual bool Damage(float damage) override;
	bool IsParryWindow() const { return (_status == STATUS::ATTACK || _status == STATUS::RUNATTACK); }
	void OnHitEnemy(); 

	// ヒットフラグの判定と設定
	bool HasHitEnemy() const { return _hasHitEnemy; }
	void SetHasHitEnemy(bool hit) { _hasHitEnemy = hit; }

	// 自動移動フラグ
	void SetAutoMove(bool enable) { _isAutoMove = enable; }

	// 外部やステートから移動軸の制限を切り替える設定関数
	void SetAllowMoveVertical(bool allow)   { _allowMoveVertical = allow; } 
	void SetAllowMoveHorizontal(bool allow) { _allowMoveHorizontal = allow; }

	// 移動可能かどうかの判定関数
	bool CanMoveVertical() const   { return _allowMoveVertical; }
	bool CanMoveHorizontal() const { return _allowMoveHorizontal; }

	// 操作可能フラグ
	void SetCanControl(bool enable);
	bool CanControl() const { return _canControl; }

	void SetCanJump(bool allow) { _canJump = allow; }
	bool CanJump() const { return _canJump; }
protected:
	void UpdateDamage();
	VECTOR CalculateInputVector();
	void UpdateMovement();
	void UpdateRotation();
	void UpdateJump();
	void UpdateAttack();

	// ダメージ点滅
	bool ShouldDraw() const;
private:
	// ----------------------------------------------------
	// メイン変数
	// ----------------------------------------------------
	
	// 外部参照・コンポーネント
	Camera* _cam = nullptr;                 // カメラ参照

	// 移動・入力関連
	VECTOR _v = VGet(0.0f, 0.0f, 0.0f);     // 移動方向ベクトル
	float  _analogDeadZone = 0.2f;          // アナログスティックのデッドゾーン
	float  lx = 0.0f;                       // アナログスティック入力X
	float  lz = 0.0f;                       // アナログスティック入力Z
										    
	// 状態・操作制限フラグ				  
	bool _canControl{ true };			    // 操作許可
	bool _canJump{ true };				    // ジャンプ許可
	bool _allowMoveVertical{ false };       // 縦移動制限
	bool _allowMoveHorizontal{ true };      // 横移動制限
	bool _isAutoMove{ false };			    // 自動移動
										    
	// 戦闘・ダメージ関連				    
	float _damageCounter = 0.0f;            // ダメージカウンター
	bool  _canAttack{ true };			    // 攻撃許可
	bool  _isInvincible{ false };		    // 無敵フラグ
	bool  _hasHitEnemy{ false };		    // 攻撃ヒットフラグ
										   
	// チャージショット関連			       
	float _chargeTime = 0.0f;               // チャージ蓄積時間
	bool  _isCharging = false;              // ボタン長押し中フラグ
	bool  _isCharge = false;                // 最大チャージ完了フラグ

	// デバッグ用
	STATUS _debugOldStatus = STATUS::NONE;  // デバッグ用の過去ステータス
};

