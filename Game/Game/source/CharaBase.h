#pragma once
#include "appframe.h"
#include "objectbase.h"
#include "camera.h"

class CharaBase : public ObjectBase
{
	typedef ObjectBase base;
public:
	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		JUMP,
		SECOND_JUMP,
		FALL,
		ATTACK,
		DASHING,
	};
	STATUS _status;

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//　Getter
	int  GetHP()   const        { return _hp;      }
	bool IsAlive() const        { return _isAlive; }
	const VECTOR& GetInputVector() const { return _vInput; }
	float GetMoveSpeed() const  { return _mvSpeed; }
	//　Setter
	void SetAlive(bool alive) { _isAlive = alive; }

	// ダメージ
	bool Damage(float damage);

	// アニメーション関連
	int   PlayAnimation(std::string name, bool loop = false); // アニメーション再生。
	void  StopAnimation();									  // アニメーション停止。
	int   GetAnimId() const { return _animId; }				  // 現在のアニメーションIDを取得
	bool  IsAnimationPlaying() const;						  // アニメーションが再生中かどうかを確認
	void  ClearAnimIdIfStopped();							  // アニメーションが停止している場合、アニメーションIDをクリアする
	float GetAnimPlayTime() const { return _playTime; }       // 現在のアニメーションの再生時間を取得するゲッター
	float GetAnimTotalTime() const { return _totalTime; }     // 現在のアニメーションの総時間を取得するゲッター


protected:
	
	// アニメーション管理
	float _attachIndex;
	float _totalTime;
	float _playTime;

	//コリジョン判定
	float _fColSubY; // コリジョン判定用のYオフセット
	float _fCollisionR; // 円の半径
	float _fCollisionWeight; // キャラの重さ

	// 体力関係
	float _hp;       
	bool _isAlive; // 生死フラグ

	// 移動関係
	float _mvSpeed; // 移動速度
	VECTOR _vInput; // 移動ベクトル

};

