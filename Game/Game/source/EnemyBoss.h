#pragma once
#include "EnemyBase.h"
class EnemyBoss : public EnemyBase
{
	typedef EnemyBase base;
public:
	EnemyBoss() = default;
	virtual ~EnemyBoss() = default;

	virtual bool Initialize() override;
};

