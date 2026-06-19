#pragma once

#include "appframe.h"
#include "applicationmain.h"
#include "CameraBase.h"

class Player;
class Enemy;

class Camera : public CameraBase
{
public:
	typedef CameraBase base;

	// 基本関数
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	void MoveBy(const VECTOR& delta);            // カメラ位置とターゲットを同量だけ移動（パン）
	void ZoomTowardsTarget(float amount);        // ターゲット方向にカメラ位置を移動（プラスで近づく、マイナスで遠ざかる）
	void RotateAroundTarget(float deltaRad);     // ターゲットを中心にY軸回転（ラジアン）
	bool UseStick();							 // スティック入力を使用する

	// プレイヤー追従
	void SetFollowTarget(Player* player) { _player = player; }
	Player* GetFollowTarget() const      { return _player;   }

	// プレイヤー追従更新
    void FollowUpdate();

	// カメラ演出
	void Shake(float strength, float duration); // カメラシェイク

	// ボス戦用の固定カメラモード
	void SetFixedMode(bool enable) { _isFixedMode = enable; }
protected:
	Player* _player = nullptr; 
	Enemy* _boss    = nullptr;

	bool _isFixedMode = false;

	// カメラシェイク用
	float _shakeTimer    { 0.0f };
	float _shakeStrength { 0.0f }; 
	float _shakeDuration { 0.0f };

	// ボス戦用のカメラ更新
	void BossBattleUpdate();
private:
	void UpdateShake();
};

