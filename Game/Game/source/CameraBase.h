#pragma once
#include "appframe.h"
class CameraBase
{
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	// Getter
	VECTOR GetTarget() const    { return _vTarget  ; }
	VECTOR GetPos()    const    { return _vPos     ; }
	float  GetClipNear() const  { return _clipNear ; }
	float  GetClipFar()  const  { return _clipFar  ; }
	float  GetForvScale() const { return _forvScale; }
	// Setter
	void SetPos(const VECTOR& Pos)       { _vPos    = Pos   ; }
	void SetTarget(const VECTOR& Target) { _vTarget = Target; }

protected:
	VECTOR _vPos;
	VECTOR _vTarget;
	float _clipNear;
	float _clipFar;
	float _forvScale;
};

