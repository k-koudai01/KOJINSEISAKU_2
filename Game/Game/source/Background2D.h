#pragma once
#include "appframe.h"

class Background2D
{
public:
	Background2D()  = default;
	~Background2D() { Terminate(); };

	bool Initialize(const std::string& filepath);
	bool Terminate();
	bool Render() const;

private: 
	int _handle = -1;
};

