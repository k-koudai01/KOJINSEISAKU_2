#pragma once
#include "appframe.h"
class UIBase : public ModeBase
{
	typedef ModeBase base;

public: 
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

protected:
	int ScreenW() const;
	int ScreenH() const;
};

