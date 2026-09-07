#pragma once
#include "IAudioService.h"
#include "container.h"

///
/// @brief 音声の再生・停止を行うクラス  
///

class AudioManager : public IAudioService
{
public:
	AudioManager() = default;
	~AudioManager() override;

	bool Initialize();
	bool Terminate();

	/// 
	/// @brief IAudioService の実装
	/// 
	bool LoadSound(int soundId, const std::string& filePath) override;
	void PlayBGM(int soundId, bool isLoop = true) override;
	void StopBGM() override;
	void PlaySE(int soundId) override;

	/// @brief 音量設定
	// @param volume 音量（0～255）
	void SetBGMVolume(int volume) override;
	void SetSEVolume(int volume) override;

private:
	std::unordered_map<int, int> _soundHandles; // ID->DxLib音源ハンドル
	int _currentBGMId = -1;
	int _bgmVolume	  = 128;
	int _seVolume	  = 128;

};

