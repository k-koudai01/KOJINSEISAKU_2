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
	const VECTOR GetOldPos() const { return _vOldPos; }
	const VECTOR GetDir() const { return _vDir; }
	float GetRadius() const { return _radius; }
	

	//　Setter
	const VECTOR SetPos(const VECTOR& vPos) { return _vPos = vPos; }
	const VECTOR SetOldPos(const VECTOR& vOldPos) { return _vOldPos = vOldPos; }
	const VECTOR SetDir(const VECTOR& vDir) { return _vDir = vDir; }
	const float SetRadius(const float radius) { return _radius = radius; }

protected:
	
	// 基本変数
	VECTOR _vPos{};
	VECTOR _vDir{};    
	VECTOR _vOldPos{}; //1フレーム前の位置
	VECTOR _vScale{ 1.0f, 1.0f, 1.0f };
	float _speed  { 0.0f };
	float _radius { 0.0f };
	

	int _handle{ -1 }; // モデルハンドル
	int _animId{ -1 }; // アニメーションID

	std::map<std::string, int> _mModelHandle;             // マップ画像
	std::unordered_map<std::string, std::string> _config; // コンフィグマップ

};

