#include "charabase.h"
#include "appframe.h"


bool CharaBase::Initialize()
{
	if (!base::Initialize()) { return false; }
	_handle = -1;
	
	_vPos    = VGet(0.0f, 0.0f, 0.0f);
	_vOldPos = VGet(0.0f, 0.0f, 0.0f);
	_vDir    = VGet(0.0f, 0.0f, 0.0f);
	_fColSubY = 0.0f;
	_fCollisionR = 0.0f;
	_fCollisionWeight = 0.0f;

	_status = STATUS::NONE;
	_isAlive = true; // 生存フラグを初期化

	//TextUtil::GetInstance()->GetConfig(_config, "HP", _fHp);// 初期体力設定
	//TextUtil::GetInstance()->GetConfig(_config, "speed", _fMvSpeed);// 移動速度設定

	return true;
}

bool CharaBase::Terminate()
{
	base::Terminate();
	return true;
}	

bool CharaBase::Process()
{
	return true;
}

bool CharaBase::Render()
{
	base::Render();
	return true;
}

bool CharaBase::Damage(float damage)
{
	// ダメージを受ける
	_hp -= damage;

	// 0以下になっていないか確認
	if(_hp <= 0.0f)
	{ 
		_hp = 0.0f; 
		_isAlive = false; // HPが0になったら生存フラグをfalseにする
	}

	return true;
}

// アニメーション再生
int CharaBase::PlayAnimation(std::string name, bool loop, float speed)
{
	if (_animId != -1)
	{
		AnimationManager::GetInstance()->Stop(_animId);
		_animId = -1;
	}

	if (_handle == -1 || name.empty())
	{
		return -1;
	}

	_animId = AnimationManager::GetInstance()->Play(_handle, name, loop, speed);
	if (_animId != -1)
	{
		AnimationManager::GetInstance()->SetTime(_animId, 0.0f);
	}
	return _animId;
}


// アニメーション停止
void CharaBase::StopAnimation()
{
	if (_animId != -1)
	{
		AnimationManager::GetInstance()->Stop(_animId);
		_animId = -1;
	}
}

bool CharaBase::IsAnimationPlaying() const
{
	if (_animId == -1)
	{
		return false;
	}
	return AnimationManager::GetInstance()->IsPlaying(_animId);
}

// アニメーションが停止している場合、アニメーションIDをクリアする
void CharaBase::ClearAnimIdIfStopped()
{
	if (_animId == -1)
	{
		return;
	}

	if (!AnimationManager::GetInstance()->IsPlaying(_animId))
	{
		_animId = -1;
	}
}
