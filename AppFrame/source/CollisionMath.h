#pragma once
#include "../source/container.h"

namespace CollisionMath
{
	constexpr int   AABB_SAMPLING_COUNT = 8; // AABBのサンプリング数
	constexpr float INF_DIST_SQ = 1e10f;     // 無限の代わり

	/**
	 * @brief 数値を一定の範囲内に収める関数
	 */
	float Clamp(float v, float minV, float maxV);

	/**
	 * @brief 線分と線分の最短距離を求める関数
	 */
	float SegmentSegmentDistSq(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2);

	/**
	 * @brief 線分とAABBの最短距離を求める関数
	 */
	float SegmentAABBDistSq(const VECTOR& p0,const VECTOR& p1, const VECTOR& boxMin, const VECTOR& boxMax, VECTOR* outSegPos, VECTOR* outBoxPos);

	/**
	 * @brief 線分と点の最短距離の2乗を求める
	 */
	inline float SegmentPointDistSq(const VECTOR& p0, const VECTOR& p1, const VECTOR& point)
	{
		VECTOR ab = VSub(p1, p0);
		VECTOR ap = VSub(point, p0);
		VECTOR bp = VSub(point, p1);

		// 内積を使って「点Pがどの領域にあるか」を判定
		float e = VDot(ap, ab);
		// 点Pが始点Aよりも外側にある
		if(e <= 0.0f) return VDot(ap, ap); // AとPの距離の2乗を返す

		float f = VDot(ab, ab); // 線分ABの長さの2乗
		// 点Pが終点Bよりも外側にある
		if(e >= f) return VDot(bp, bp);

		// 点が線分の垂直範囲内にある場合
		return VDot(ap, ap) - (e * e) / f;
	}

	/**
	 * @brief 2点間の距離の2乗計算(球と球)
	 */
	inline float PointPointDistSq(const VECTOR& p1, const VECTOR& p2)
	{
		VECTOR diff = VSub(p1, p2);
		return VDot(diff, diff);
	}
	
};

