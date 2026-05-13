#include "Player.h"

#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "mymath.h"


bool Player::Initialize() 
{
	// 基底クラスの初期化
	if(!base::Initialize()) { return false; }

	// モデルデータのロード
	_handle = MV1LoadModel("res/SDChar/SDChar.mv1");

	if(_handle == -1) { return false; }
	
	_animId = -1;
	// ステータスを「無し」に設定
	_status = STATUS::NONE;

	// 位置、向きの初期化
	_vPos = VGet(0.0f, 0.0f, 0.0f); 
	_vDir = VGet(0.0f, 0.0f, -1.0f);

	// 腰位置の設定
	_fColSubY = 40.0f;

	// コリジョン半径の設定
	_fCollisionR = 30.0f;
	_fCollisionWeight = 20.0f;

	// カメラの初期化
	_cam = nullptr;

	// 移動速度設定
	_mvSpeed = 5.0f;

	// 初期体力設定
	_hp = 5.0f;

	return true;
}

bool Player::Terminate()
{
	base::Terminate();

	return true;
}

bool Player::Process() 
{
	int key = ApplicationBase::GetInstance()->GetKey();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	base::Process();

	// 処理前の位置を保存
	_vOldPos = _vPos; 

	// 処理前のステータスを保存しておく
	CharaBase::STATUS old_status = _status;
	// 移動方向を決める
	_v = { 0,0,0 };

	//　処理前のステータスを保存しておく
	STATUS oldStatus = _status;

	// カメラの向いている角度を取得
	float sx = _cam->GetPos().x - _cam->GetTarget().x;
	float sz = _cam->GetPos().z - _cam->GetTarget().z;
	float camrad = atan2(sz, sx);
	float rad = 0.0f;

	//左スティック値
	lStickX = lx;
	lStickZ = lz;

	// ローカル入力ベクトル
	VECTOR inputLocal = VGet(0.0f, 0.0f, 0.0f);

	// 操作（キーボード）
	if(key & PAD_INPUT_UP)
	{
		inputLocal.x = -1.0f;
	}
	if(key & PAD_INPUT_DOWN)
	{
		inputLocal.x = 1.0f;
	}
	if(key & PAD_INPUT_LEFT)
	{
		inputLocal.z = -1.0f;
	}
	if(key & PAD_INPUT_RIGHT)
	{
		inputLocal.z = 1.0f;
	}

	// アナログ入力の長さ/角度
	float length   = sqrt(lStickX * lStickX + lStickZ * lStickZ);
	float radStick = atan2(lStickX, lStickZ);

	// アナログ左スティック用
	if(length < _analogDeadZone)
	{
		length = 0.f;
	}
	else
	{
		length = _mvSpeed;
	}

	// 移動ベクトル
	_v.x = cos(radStick + camrad) * length;
	_v.z = sin(radStick + camrad) * length;

	// 優先順：アナログ > キーボード
	if (length > 0.0f)
	{
		_vInput = VGet(lStickZ, 0.0f, lStickX);
		if (VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput); 
	}
	else
	{
		_vInput = inputLocal;
		if (VSize(_vInput) > 0.0f) _vInput = VNorm(_vInput);
	}

	// 地上移動
	if (VSize(_v) > 0.0f)
	{
		_status = STATUS::WALK;
	}
	else
	{
		_status = STATUS::WAIT;
	}

	// ステータスが変わっていたらアニメーションを変更
	if(oldStatus != _status)
	{
		auto it = _AnimTable.find(_status);
		if(it != _AnimTable.end())
		{
			PlayAnimation(it->second.name, it->second.loop);
		}
	}
	return true;
}

bool Player::Render()
{
	base::Render();

	//位置反映
	MV1SetPosition(_handle, _vPos);

	// スケール
	MV1SetScale(_handle, VGet(1.0f, 1.0f, 1.0f));

	//描画
	MV1DrawModel(_handle);
	return true;
}

