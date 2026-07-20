#pragma once
#include "appframe.h"

class ObjectFactry;
class ObjectManager;

class StageLoader
{
public:
	static void LoadStage(const std::string& filename, const ObjectFactry& factory, ObjectManager& objMgr);
};

