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
	static constexpr float BASE_SCREEN_W = 1280.0f;
	static constexpr float BASE_SCREEN_H = 720.0f;

	int ScreenW() const;
	int ScreenH() const;

	// スケール取得関数
	float GetScaleX() const {return static_cast<float>(ScreenW()) / BASE_SCREEN_W;}
	float GetScaleY() const {return static_cast<float>(ScreenH()) / BASE_SCREEN_H;}

	// アスペクト比を維持する関数
	float GetScale()const
	{
		return (GetScaleX() < GetScaleY()) ? GetScaleX() : GetScaleY();
	}

};

