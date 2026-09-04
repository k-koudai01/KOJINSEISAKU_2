#pragma once
#include "MinionBase.h"

/**
 * @brief プレイヤーを追跡せず直進して通り抜ける雑魚エネミー
 */
class MinionRunner : public MinionBase
{
	typedef MinionBase base;
public:
	MinionRunner() = default;
	~MinionRunner() = default;

	virtual bool Initialize() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	// @brief Runnerの移動処理
	void UpdateMove();

	bool _hasTargetDir = false;
};

