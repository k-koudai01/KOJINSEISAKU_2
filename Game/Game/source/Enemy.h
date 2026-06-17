#pragma once
#include "appframe.h"
#include "SpriteCharaBase.h"

class Player;

class Enemy : public SpriteCharaBase
{
	typedef SpriteCharaBase base;
public:

	// ボスの行動状態
	enum class BossState
	{
		IDLE,
		RUSH_PREP,
		RUSH_ATTACK,
	};

	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual void UpdateRotation();
	virtual bool Damage(float damage) override;
	void SetPlayer(Player* player) { _player = player; }

protected:
	//void UpdateIdle(const VECTOR& playerPos);
	void UpdateRushPrep(const VECTOR& playerPos);
	void UpdateRushAttack();                        
	      


	Player* _player = nullptr;

	BossState _bossState = BossState::IDLE;
	float     _stateTimer = 0.0f;
	VECTOR    _targetDir = VGet(0, 0, 0);
};

