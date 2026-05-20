#include "UIBase.h"

bool UIBase::Initialize()
{
	if (!base::Initialize()) { return false; }
	return true;
}

bool UIBase::Terminate()
{
	base::Terminate();
	return true;
}

bool UIBase::Process()
{
	base::Process();
	return true;
}

bool UIBase::Render()
{
	base::Render();
	return true;
}

int UIBase::ScreenW() const
{
	return ApplicationBase::GetInstance()->DispSizeW();
}

int UIBase::ScreenH() const
{
	return ApplicationBase::GetInstance()->DispSizeH();
}
