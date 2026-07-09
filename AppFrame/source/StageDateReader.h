#pragma once
#include "container.h"

struct RawStageObj_Data
{
	std::string name;
	std::string type;
	VECTOR       pos;
	VECTOR      dir;
	VECTOR     scale;
};

class StageDateReader
{
public:
	static std::vector<RawStageObj_Data> ReadJson(const std::string& filename);
};