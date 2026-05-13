#include "CameraBase.h"

bool CameraBase::Initialize()
{
	_vPos    = VGet(0.0f, 0.0f, 0.0f);
	_vTarget = VGet(0.0f, 0.0f, 0.0f);
	_clipNear  = 0.0f;
	_clipFar   = 0.0f;
	_forvScale = 0.0f;

	return true;
}

bool CameraBase::Terminate()
{
	_vPos    = VGet(0.0f, 0.0f, 0.0f);
	_vTarget = VGet(0.0f, 0.0f, 0.0f);
	_clipNear  = 0.0f;
	_clipFar   = 0.0f;
	_forvScale = 0.0f;

	return true;
}

bool CameraBase::Process()
{
	return true;
}

bool CameraBase::Render()
{
	return true;
}
