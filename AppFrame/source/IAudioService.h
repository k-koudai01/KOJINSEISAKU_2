#pragma once
#include "container.h"

/// 
/// @brief 音声の再生・停止を行うインターフェース
/// 
class IAudioService
{
public:
	virtual ~IAudioService() = default;
	
	virtual bool LoadSound(int soundId, const std::string& filePath) = 0;
	virtual void PlayBGM(int soundId, bool isLoop = true) = 0;
	virtual void StopBGM() = 0;
	virtual void PlaySE(int soundId) = 0;
	virtual void SetBGMVolume(int volume) = 0;
	virtual void SetSEVolume(int volume) = 0;
};

