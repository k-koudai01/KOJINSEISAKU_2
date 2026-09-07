#include "AudioManager.h"

AudioManager::~AudioManager()
{
	Terminate();
}

bool AudioManager::Initialize()
{
	return true;
}

bool AudioManager::Terminate()
{
	for(auto& pair : _soundHandles)
	{
		DeleteSoundMem(pair.second);
	}
	_soundHandles.clear();
	_currentBGMId = -1;
	return true;
}

bool AudioManager::LoadSound(int soundId, const std::string& filePath)
{
	if(_soundHandles.find(soundId) != _soundHandles.end())
	{
		return true;
	}

	int handle = LoadSoundMem(filePath.c_str());
	if(handle == -1)
	{
		return false;
	}

	_soundHandles[soundId] = handle;
	return true;
}

void AudioManager::PlayBGM(int soundId, bool isLoop)
{
	if(_currentBGMId == soundId && CheckSoundMem(_soundHandles[soundId]))
	{
		return;
	}

	StopBGM();

	auto it = _soundHandles.find(soundId);
	if(it != _soundHandles.end())
	{
		int playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
		ChangeVolumeSoundMem(_bgmVolume, it->second);
		PlaySoundMem(it->second, playType);	
		_currentBGMId = soundId;
	}

}

void AudioManager::StopBGM()
{
	if(_currentBGMId != -1)
	{
		auto it = _soundHandles.find(_currentBGMId);
		if(it != _soundHandles.end())
		{
			StopSoundMem(it->second);
		}
		_currentBGMId = -1;
	}
}

void AudioManager::PlaySE(int soundId)
{
	auto it = _soundHandles.find(soundId);
	if(it != _soundHandles.end())
	{
		ChangeVolumeSoundMem(_seVolume, it->second);
		PlaySoundMem(it->second, DX_PLAYTYPE_BACK, TRUE);
	}
}

void AudioManager::SetBGMVolume(int volume)
{
	_bgmVolume = volume;
	if(_currentBGMId != -1 && _soundHandles.count(_currentBGMId))
	{
		ChangeVolumeSoundMem(_bgmVolume, _soundHandles[_currentBGMId]);
	}
}

void AudioManager::SetSEVolume(int volume)
{
	_seVolume = volume;
}