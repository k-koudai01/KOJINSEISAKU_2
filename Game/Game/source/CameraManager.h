#pragma once
#include "Camera.h"
#include "appframe.h"
class CameraManager
{
public:
	
	// カメラの取得
	static CameraManager* GetInstance()
	{
		static CameraManager instance;
		return &instance;
	}

	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	// アクティブなカメラをゲット/セッター
	void SetActiveCamera(Camera* camera) { _activeCamera = camera; }
	Camera* GetActiveCamera() const { return _activeCamera; }

	void SetFixedMode(bool enable)
	{
		if(_activeCamera != nullptr)
		{
			_activeCamera->SetFixedMode(enable);
		}
	}

	void Process()
	{
		if(_activeCamera)
		{
			_activeCamera->Process();
		}
	}

	void Shake(float strength, float duration)
	{
		if(_activeCamera)
        {
            _activeCamera->Shake(strength, duration);
        }
	}
private:
	CameraManager() = default;
	~CameraManager() = default;

	Camera* _activeCamera = nullptr;
};

