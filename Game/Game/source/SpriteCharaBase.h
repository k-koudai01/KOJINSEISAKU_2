#pragma once
#include "CharaBase.h"
#include "appframe.h"

class SpriteCharaBase :public CharaBase
{
	typedef CharaBase base;
public:
	virtual bool Initialize() override;
	virtual bool Render() override;

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
		int cols = 0;
		int rows = 0;
		int frameW = 0;
		int frameH = 0;
		std::vector<int> handles;
	};
	std::unordered_map<STATUS, SpriteSheet> _spriteSheets;

	enum class Facing { Down = 0, Left = 1, Right = 2, Up = 3 };
	Facing _facing = Facing::Down;

	// スプライト
	void SetSpriteSheet(STATUS status, const char* path, int cols, int rows);
	void SetSpriteAnimTable(const std::unordered_map<STATUS, SpriteAnimDef>& animTable);

	// 2Dビルボード用
	void UpdateFacing(const VECTOR& input);
	void UpdateSpriteAnimation(STATUS oldStatus);

	int _frameIndex{ 0 };
	int _spriteAnimId{ -1 };
};

