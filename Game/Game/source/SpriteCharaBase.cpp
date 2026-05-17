#include "SpriteCharaBase.h"

bool SpriteCharaBase::Initialize()
{
	base::Initialize();
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

	float cx = sheet.frameW * 0.5f;
	float cy = sheet.frameH * 0.5f;

	float size = _spriteScale;

	DrawGraph(0, 0, handle, TRUE);

	float halfH = (sheet.frameH * size) * 0.5f;

	float offsetX = -945.0f; //　オフセット
	VECTOR drawPos = VAdd(_vPos, VGet(offsetX, halfH, 0.0f));


	DrawBillboard3D(drawPos, cx, cy, size, 0.0f, handle, TRUE);

	return true;
}

void SpriteCharaBase::SetSpriteSheet(STATUS status, const char* path, int cols, int rows)
{
	SpriteSheet sheet;
	sheet.cols = cols;
	sheet.rows = rows;
	sheet.handles.assign(cols * rows, -1);

	int temp = LoadGraph(path);
	if (temp == -1) { return; }

	int w = 0, h = 0;
	GetGraphSize(temp, &w, &h);
	DeleteGraph(temp);

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

void SpriteCharaBase::UpdateFacing(const VECTOR& input)
{
	if(VSize(_vInput) == 0.0f) return;

	if(fabs(_vInput.x) > fabs(_vInput.z))
	{
		_facing = (_vInput.x > 0.0f) ? Facing::Right : Facing::Left;
	}
	else
	{
		_facing = (_vInput.z > 0.0f) ? Facing::Down : Facing::Up;
	}
}

void SpriteCharaBase::UpdateSpriteAnimation(STATUS oldStatus)
{
	if (oldStatus != _status)
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