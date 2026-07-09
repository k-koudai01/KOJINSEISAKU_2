#pragma once
#include "appframe.h"

class Cube;

class StageLoader
{
public:
	static void LoadStage(const std::string& filename);
	static void Update();
	static void Draw();
	static void Clear();

private:
	static std::vector<std::shared_ptr<Cube>> _cubes;
};

