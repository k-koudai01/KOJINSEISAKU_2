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
		IDLE,
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

	// Getter
	int  GetHP()   const        { return _hp;      }
	bool IsAlive() const        { return _isAlive; }
	const VECTOR& GetInputVector() const { return _vInput; }
	float GetMoveSpeed() const  { return _mvSpeed; }
	// 当たり判定
	float GetCollisionRadius() const { return _fCollisionR; }
	VECTOR GetCollisionCenter() const 
	{
		VECTOR c = _vPos;
		c.y += _fColSubY;
		return c;
	}

	// Setter
	void SetAlive(bool alive) { _isAlive = alive; }

	// ダメージ
	bool Damage(float damage);

	// アニメーション関連
	int   PlayAnimation(std::string name, bool loop = false, float speed = 1.0f); // アニメーション再生。
	void  StopAnimation();									  // アニメーション停止。
	int   GetAnimId() const { return _animId; }				  // 現在のアニメーションIDを取得
	bool  IsAnimationPlaying() const;						  // アニメーションが再生中かどうかを確認
	void  ClearAnimIdIfStopped();							  // アニメーションが停止している場合、アニメーションIDをクリアする
	float GetAnimPlayTime() const { return _playTime; }       // 現在のアニメーションの再生時間を取得するゲッター
	float GetAnimTotalTime() const { return _totalTime; }     // 現在のアニメーションの総時間を取得するゲッター


protected:

	//void UpdateAnimation(STATUS oldStatus);

	// 2Dビルボード用
	enum class Facing { Down = 0, Left = 1, Right = 2, Up = 3 };
	Facing _facing = Facing::Down;

	static constexpr int SpriteCols = 4;
	static constexpr int SpriteRows = 4;
	std::array<int, SpriteCols* SpriteRows> _spriteHandles{};

	// スプライト
	int _sprite{ 0 };
	const char* _spritePath = "";

	// アニメーション管理
	int _frameW { 0 };
	int _frameH { 0 };
	int _frameIndex { 0 };
	float _spriteScale { 1.0f };
	float _attachIndex { 0.0f };
	float _totalTime   { 0.0f };
	float _playTime    { 0.0f };

	//コリジョン判定
	float _fColSubY { 0.0f };         // コリジョン判定用のYオフセット
	float _fCollisionR { 0.0f };      // 円の半径
	float _fCollisionWeight { 0.0f }; // キャラの重さ

	// 体力関係
	float _hp { 0.0f };       
	bool _isAlive { true }; // 生死フラグ

	// 移動関係
	float _mvSpeed; // 移動速度
	VECTOR _vInput; // 移動ベクトル

};

