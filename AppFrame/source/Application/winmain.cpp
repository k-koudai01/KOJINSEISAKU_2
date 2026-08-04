/*
** WinMain
*/

//
// include 部
//

#include "../appframe.h"



//
// WinMain(). プログラム起動関数
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	ApplicationBase *appBase = ApplicationBase::GetInstance();
	if(!appBase) { return 0; }

	if(!appBase->Initialize(hInstance))
	{
		return 0;
	}

	auto frameRateController = appBase->GetFrameRateController();

	// 1フレームループを組む ----------------------------------------------------------
	while(ProcessMessage() == 0)		// プログラムが終了するまでループ
	{
		frameRateController->BeginFrame();	// フレーム開始

		appBase->Input();
		appBase->Process();

		ClearDrawScreen();		// 画面を初期化する
		appBase->Render();
		frameRateController->EndFrame();	// フレーム終了
		ScreenFlip();			// 裏画面の内容を表画面に反映させる
	}

	appBase->Terminate();

	return 0;
}
