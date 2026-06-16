#pragma once
#include "appframe.h"
#include "SpriteCharaBase.h"

class Enemy : public SpriteCharaBase
{
	typedef SpriteCharaBase base;
public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

	virtual void Update

	virtual bool Damage(float damage) override;


};

