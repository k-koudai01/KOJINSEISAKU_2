#include "MyMath.h"

namespace
{
	// ランダムの精度
	constexpr float RANDOM_PRECISION = 10000.0f; 
}

float MyMath::GetRandomFloat(float min, float max)
{
	if(min >= max) return min;

	// 0.0f～1.0fの範囲でランダムな小数を生成
	float rate = static_cast<float>(GetRand(RANDOM_PRECISION)) / RANDOM_PRECISION;

	return min + (max - min) * rate;
}

VECTOR MyMath::GetRandomPosInRange(const VECTOR& center, const VECTOR& range)
{
	// 指定した中心位置と範囲からランダムな座標を返す
	return VGet
	(
		center.x + GetRandomFloat(-range.x * 0.5f, range.x * 0.5f),
		center.y + GetRandomFloat(-range.y * 0.5f, range.y * 0.5f),
		center.z + GetRandomFloat(-range.z * 0.5f, range.z * 0.5f)
	);
}