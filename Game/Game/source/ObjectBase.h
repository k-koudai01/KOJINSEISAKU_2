#pragma once
#include "appframe.h"
#include "Camera.h"

class ObjectBase
{
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//　Getter
	const VECTOR GetPos() const { return _vPos; }
	const VECTOR GetDir() const { return _vDir; }
	float GetRadius() const { return _radius; }

	//　Setter
	const VECTOR SetPos(const VECTOR& vPos) { return _vPos = vPos; }
	const VECTOR SetDir(const VECTOR& vDir) { return _vDir = vDir; }
	const float SetRadius(const float radius) { return _radius = radius; }

protected:
	
	// 基本変数
	VECTOR _vPos;	
	VECTOR _vDir;    
	VECTOR _vOldPos; //1フレーム前の位置
	float _speed;
	float _radius;
	VECTOR _vScale;	// スケール

	int _handle;				// モデルハンドル
	int _animId;				// アニメーションID

	std::map<std::string, int> _mModelHandle;             // マップ画像
	std::unordered_map<std::string, std::string> _config; // コンフィグマップ

};

