#include "MinionBase.h"
#include "Player.h"

bool MinionBase::Initialize()
{
	if(!base::Initialize()){ return false; }
	return true;
}

bool MinionBase::Process()
{
	base::Process();
	return true;
}