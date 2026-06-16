#pragma once
#include "../source/container.h"

namespace CollisionMath
{
	float Clamp(float v, float minV, float maxV);
	float SegmentSegmentDistSq(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2);
};

