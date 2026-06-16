#include "CollisionMath.h"

namespace CollisionMath
{
	float Clamp(float v, float minV, float maxV)
	{
		if (v < minV) return minV;
		if (v > maxV) return maxV;
		return v;
	}

	float SegmentSegmentDistSq(const VECTOR& p1, const VECTOR& q1, const VECTOR& p2, const VECTOR& q2)
	{
		//　方向ベクトル　
		const VECTOR d1 = VSub(q1, p1);
		const VECTOR d2 = VSub(q2, p2);

		//　始点同士の差を作る
		const VECTOR r  = VSub(p1, p2);

		// 内積で係数を作る
		const float a = VDot(d1, d1);
		const float e = VDot(d2, d2);
		const float f = VDot(d2, r);

		// パラメータ s と t を求める
		float s = 0.0f; 
		float t = 0.0f;

		// どちらも線分が点になっている場合は、始点同士の距離を返す
		if (a <= 1e-6f && e <= 1e-6f)
		{
			return VDot(r, r);
		}

		// Aは固定点なので、B上の最近点だけを探す。
		if (a <= 1e-6f)
		{
			s = 0.0f;
			t = Clamp(f / e, 0.0f, 1.0f);
		}
		else
		{
			const float c = VDot(d1, r); 

			// Bは固定点なので、A上の最近点だけを探す。
			if(e <= 1e-6f)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else
			{
				const float b = VDot(d1, d2);
				const float denom = a * e - b * b;

				// もし線分が平行でなければ、s と t を求める
				if(denom != 0.0f)
				{
					s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else
				{
					s = 0.0f;
				}

				t = (b * s + f) / e;

				// もし t が線分の範囲外なら、t をクランプして、s を再計算する
				if(t < 0.0f)
				{
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if(t > 1.0f)
				{
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
			}
		}

		const VECTOR c1 = VGet(p1.x + d1.x * s, p1.y + d1.y * s, p1.z + d1.z * s);
		const VECTOR c2 = VGet(p2.x + d2.x * t, p2.y + d2.y * t, p2.z + d2.z * t);

		const VECTOR diff = VSub(c1, c2);
		return VDot(diff, diff);
	}
}