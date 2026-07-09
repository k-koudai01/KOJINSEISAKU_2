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

private:
	// 基本変数
	VECTOR _vPos;
	VECTOR _vDir;
	VECTOR _vScale;	
};

