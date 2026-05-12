#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "CharaBase.h"
#include <cmath>

bool CharaBase::Initialize()
{
	return true;
}

bool CharaBase::Terminate()
{
	return true;
}	

bool CharaBase::Process()
{
	return true;
}

bool CharaBase::Render()
{
	return true;
}

bool CharaBase::IsHitCircle(CharaBase* c1, CharaBase* c2)
{
	if(!c1 || !c2) return false;

	// 水平（X/Z）距離チェック
	float dx = c1->_vPos.x - c2->_vPos.x;
	float dz = c1->_vPos.z - c2->_vPos.z;
	float dist2 = dx * dx + dz * dz;

	float sumR = c1->_collision_r + c2->_collision_r;
	if(dist2 > sumR * sumR)
	{
		// 水平に離れている -> 衝突なし
		return false;
	}

	// 垂直（Y）方向の重なりチェック（高さを考慮）
	float dy = std::fabs(c1->_vPos.y - c2->_vPos.y);
	float halfH1 = c1->_collision_h * 0.5f;
	float halfH2 = c2->_collision_h * 0.5f;
	if(dy > (halfH1 + halfH2))
	{
		// Y方向に重なりがない -> 衝突なし
		return false;
	}

	// 水平・垂直ともに重なっていれば当たり
	return true;
}
