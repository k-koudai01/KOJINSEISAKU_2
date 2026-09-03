#pragma once
#include "MinionBase.h"

/**
 * @brief プレイヤー追従型の雑魚エネミー
 */
class MinionChaser : public MinionBase
{
	typedef MinionBase base;
public:
	MinionChaser() = default;
	virtual ~MinionChaser() = default;

	virtual bool Initialize() override;
	virtual bool Process() override;

private:
	void UpdateMove();
};

