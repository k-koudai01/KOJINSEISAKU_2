#pragma once
#include "appframe.h"
#include "SoundDefine.h"

class SoundRegistry
{
public:
	struct SoundAsset
	{
		SoundID id;
		std::string filePath;
	};

	static bool LoadAll(IAudioService& audio)
	{
		const std::vector<SoundAsset> assets = 
		{
			{ SoundID::BGM_TITLE, "res/Sound/BGM/Game.mp3" },
			{ SoundID::BGM_GAME , "res/Sound/BGM/Game.mp3" }
		};

		bool isSuccess = true;

		// 音声アセットをロードする
		for(const auto& asset : assets)
		{
			if(!audio.LoadSound(static_cast<int>(asset.id), asset.filePath))
			{
				isSuccess = false;
			}
		}
		return isSuccess;	
	}


};