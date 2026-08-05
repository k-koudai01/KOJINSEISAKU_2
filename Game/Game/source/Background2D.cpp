#include "Background2D.h"

bool Background2D::Initialize(const std::string& filepath)
{
	_handle = LoadGraph(filepath.c_str());
	return _handle != -1;
}

bool Background2D::Terminate()
{
	if(_handle != -1)
	{
		DeleteGraph(_handle);
		_handle = -1;
	}
	return true;
}

bool Background2D::Render() const
{
	if(_handle == -1) return false;

	int sw = ApplicationBase::GetInstance()->DispSizeW();
	int sh = ApplicationBase::GetInstance()->DispSizeH();

	// Zバッファを切って最背面に2D描画
	SetUseZBuffer3D(FALSE);
	DrawExtendGraph(0, 0, sw, sh, _handle, TRUE);
	SetUseZBuffer3D(TRUE);

	return true;
}