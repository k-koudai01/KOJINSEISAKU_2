#include "SpriteCharaBase.h"

bool SpriteCharaBase::Initialize()
{
	base::Initialize();
	return true;
}

bool SpriteCharaBase::Terminate()
{
	for(auto& pair : _spriteSheets)
	{
		for(int handle : pair.second.handles)
		{
			if(handle != -1)
			{
				DeleteGraph(handle);
			}
		}
	}
	_spriteSheets.clear();

	base::Terminate();
	return true;
}

bool SpriteCharaBase::Render()
{
	base::Render();

	// 現在の状態のシートを取得
	auto sheetIt = _spriteSheets.find(_status);
	if(sheetIt == _spriteSheets.end()) return true;

	const SpriteSheet& sheet = sheetIt->second;
	if(sheet.handles.empty()) return true;

	// 向きとフレーム列からインデックスを計算
	int row = static_cast<int>(_facing); 
	int col = (_frameIndex % sheet.cols); 
	int index = row * sheet.cols + col; 

	int handle = sheet.handles[index];
	if (handle == -1) return true;

	float cx = 0.5f;
	float cy = 0.5f;

	float halfH = sheet.frameH * 0.5f;

	DrawBillboard3D(_vPos, cx, 0.5f, _spriteScale, 0.0f, handle, TRUE);

	return true;
}

void SpriteCharaBase::SetSpriteSheet(STATUS status, const char* path, int cols, int rows)
{
	SpriteSheet sheet;
	sheet.cols = cols;
	sheet.rows = rows;
	sheet.handles.assign(cols * rows, -1); // ハンドルを初期化

	int temp = LoadGraph(path);
	if (temp == -1) { return; }

	int w = 0, h = 0;
	GetGraphSize(temp, &w, &h);
	DeleteGraph(temp);

	// 画像サイズがフレーム数で割り切れない場合はエラー
	if (w % cols != 0 || h % rows != 0) { return; }	

	sheet.frameW = w / cols;
	sheet.frameH = h / rows;

	if (LoadDivGraph(path, cols * rows, cols, rows, sheet.frameW, sheet.frameH, sheet.handles.data()) == -1)
	{
		return;
	}

	_spriteSheets[status] = sheet;
}

void SpriteCharaBase::SetSpriteAnimTable(const std::unordered_map<STATUS, SpriteAnimDef>& table)
{
	_spriteAnimTable = table;
}

void SpriteCharaBase::UpdateFacing(const VECTOR& dir)
{
	if (VSize(dir) == 0.0f) return;

	if (fabs(dir.x) > fabs(dir.z))
	{
		_facing = (dir.x > 0.0f) ? Facing::Right : Facing::Left;
	}
	else
	{
		_facing = (dir.z > 0.0f) ? Facing::Up : Facing::Down;
	}
}

void SpriteCharaBase::UpdateSpriteAnimation(STATUS oldStatus)
{
	// ステータスが変わった場合、またはアニメーションIDが無効な場合に新しいアニメーションを再生
	if(oldStatus != _status || _spriteAnimId == -1) 
	{
		auto it = _spriteAnimTable.find(_status);
		if (it != _spriteAnimTable.end())
		{
			_spriteAnimId = SpriteAnimationManager::GetInstance()->Play(
				it->second.frames, it->second.fps, it->second.loop
			);
		}
	}

	if (_spriteAnimId != -1)
	{
		_frameIndex = SpriteAnimationManager::GetInstance()->GetFrame(_spriteAnimId);
	}
}

bool SpriteCharaBase::IsSpriteAnimationFinished()const
{
	// アニメーションが設定されていない場合は終了とみなす
	if(_spriteAnimId == -1)
	{
		return true;
	}

	return !SpriteAnimationManager::GetInstance()->IsPlaying(_spriteAnimId);
}