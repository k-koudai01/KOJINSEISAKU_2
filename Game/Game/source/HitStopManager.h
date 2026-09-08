#pragma once
class HitStopManager
{
public:
	static HitStopManager& GetInstance();

	void Update(float rawDeltaTime);

	///
	/// @brief ヒットストップのリクエスト
	/// @param duration ヒットストップの持続時間
	/// @param scale スケール(ヒットスロー)
	/// 
	void RequestHitStop(float duration, float scale = 0.0f);

	float GetDeltaTime() const;
	bool  IsHitStopping() const; // ヒットストップ中かどうか

private:
	HitStopManager() = default;
	~HitStopManager() = default;

	float _rawDeltaTime    = 0.0f; 
	float _hitStopScale    = 1.0f;
	float _hitStopTimer	   = 0.0f; // ヒットストップの経過時間
	float _hitStopDuration = 0.0f; // ヒットストップの持続時間


};

