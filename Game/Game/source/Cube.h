#pragma once
#include "ObjectBase.h"

class Cube : public ObjectBase
{
public:
	Cube();
	~Cube();
	bool Initialize() override;
	bool Process() override;
	bool Render() override;

	void CubeRender();

	void SetPosition(const VECTOR& pos) { _vPos   = pos;   }
	void SetDirection(const VECTOR& dir) { _vDir = dir; }
	void SetScale(const VECTOR& scale)  { _vScale = scale; }
	VECTOR GetPosition() const { return _vPos; }

	// 当たり判定用のMin/Maxを取得する関数
	VECTOR GetBBMin() const { return VSub(_vPos, VScale(_vScale, 0.5f)); }
	VECTOR GetBBMax() const { return VAdd(_vPos, VScale(_vScale, 0.5f)); }
private:
	// 基本変数
	VECTOR _vPos;
	VECTOR _vDir;
	VECTOR _vScale;	
};

