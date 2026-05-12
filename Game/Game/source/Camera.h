#pragma once

#pragma once
#include "appframe.h"
#include "applicationmain.h"

class Camera
{
public:
	
	// 基本関数
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	//パン（位置・ターゲットを移動）・ターゲット方向にズーム
	void MoveBy(const VECTOR& delta);         // カメラ位置とターゲットを同量だけ移動（パン）
	void ZoomTowardsTarget(float amount);        // ターゲット方向にカメラ位置を移動（プラスで近づく、マイナスで遠ざかる）
	void RotateAroundTarget(float deltaRad);     // ターゲットを中心にY軸回転（ラジアン）
	bool UseStick(); // スティック入力を使用する
	// カメラ追従更新

	virtual void FollowUpdate();

	// Getter
	VECTOR GetTarget() const    { return _vTarget;  }
	VECTOR GetPos()    const    { return _vPos;     }
	float  GetClipNear() const  { return _clipNear;  }
	float  GetClipFar()  const  { return _clipFar;   }
	float  GetForvScale() const { return _forvScale; }
	// Setter
	void SetPos(const VECTOR& Pos)       { _vPos = Pos;       }
	void SetTarget(const VECTOR& Target) { _vTarget = Target; }

	Player* _player = nullptr; // カメラが追従するプレイヤー

protected:
	VECTOR _vPos;    
	VECTOR _vTarget; 
	float _clipNear; 
	float _clipFar;   
	float _forvScale; 

};

