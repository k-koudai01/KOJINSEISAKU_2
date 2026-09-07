#pragma once
#include "appframe.h"
#include "AudioManager.h"

class ApplicationMain : public ApplicationBase
{
	typedef ApplicationBase base;
public:
	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 1920; }
	virtual int DispSizeH() { return 1080; }

	IAudioService& GetAudioService() { return _audioManager; }

protected:
	AudioManager _audioManager;
};

extern ApplicationMain g_oApplicationMain;