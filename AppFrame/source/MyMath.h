#pragma once
#include "container.h"

class MyMath
{
public:

	MyMath() = default;

	// ランダム計数用ヘルパー関数
	// min以上max未満の範囲でランダムな小数を返す(時間などに有効)
	static float GetRandomFloat(float min, float max);

	// 指定した中心位置と範囲からランダムな座標を返す
	static VECTOR GetRandomPosInRange(const VECTOR& center, const VECTOR& range);

};

