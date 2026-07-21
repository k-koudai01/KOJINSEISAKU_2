#pragma once
#include "../source/container.h"

namespace CollisionMath
{
	constexpr int   AABB_SAMPLING_COUNT = 8; // AABBのサンプリング数
	constexpr float INF_DIST_SQ = 1e10f;     // 無限の代わり

	// 数値を一定の範囲内に収める関数
	float Clamp(float v, float minV, float maxV);

	// 線分と線分の最短距離を求める関数
	float SegmentSegmentDistSq(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2);

	// 線分とAABBの最短距離を求める関数
	float SegmentAABBDistSq(const VECTOR& p0,const VECTOR& p1, const VECTOR& boxMin, const VECTOR& boxMax, VECTOR* outSegPos, VECTOR* outBoxPos);
};

