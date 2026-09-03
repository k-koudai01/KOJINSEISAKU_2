#pragma once
#include "EnemyBase.h"
#include "appframe.h"

class MinionBase : public EnemyBase
{
	typedef EnemyBase base;
public:
	MinionBase()		   = default;
	virtual ~MinionBase()  = default;

	virtual bool Initialize() override;
	virtual bool Process() override;
};

