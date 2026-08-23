#pragma once
#include "appframe.h"

struct Particle
{
	VECTOR position    { 0.0f, 0.0f, 0.0f };
	VECTOR velocity    { 0.0f, 0.0f, 0.0f };
	VECTOR scale	   { 1.0f, 1.0f, 1.0f };
	float rotation	   { 0.0f };
	float rotSpeed	   { 0.0f };
	float life		   { 0.0f };
	float maxLife	   { 1.0f };
	bool active		   { false };
	int handle		   { -1 };
};
