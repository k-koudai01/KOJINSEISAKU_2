#pragma once
#include "CharaBase.h"
#include "appframe.h"

class SpriteCharaBase :public CharaBase
{
	typedef CharaBase base;
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Render() override;

	
	/// @brief 向きの更新
	void UpdateFacing(const VECTOR& dir);

	/// @brief スプライトアニメーションの更新
	virtual void UpdateSpriteAnimation(STATUS oldStatus);

protected:

	struct SpriteAnimDef
	{
		int frames;
		float fps;
		bool loop;
	};
	std::unordered_map<STATUS, SpriteAnimDef> _spriteAnimTable;

	struct SpriteSheet
	{
		int cols = 0;   // 列数
		int rows = 0;   // 行数
		int frameW = 0;
		int frameH = 0;
		std::vector<int> handles;
	};
	std::unordered_map<STATUS, SpriteSheet> _spriteSheets;

	enum class Facing { Down = 0, Left = 1, Right = 2, Up = 3 };
	Facing _facing = Facing::Down;

	///
	/// @brief スプライトシートを設定する
	/// @param status ステータス
	/// @param path 画像パス
	/// @param cols 列数
	/// @param rows 行数
	/// 
	void SetSpriteSheet(STATUS status, const char* path, int cols, int rows);

	///
	/// @brief スプライトアニメーションテーブルを設定する
	/// @param animTable アニメーションテーブル
	///
	void SetSpriteAnimTable(const std::unordered_map<STATUS, SpriteAnimDef>& animTable);

	// アニメーションが終了したか
	bool IsSpriteAnimationFinished() const;

	int _frameIndex{ 0 };
	int _spriteAnimId{ -1 };
	int _playCount{ 0 };
};

