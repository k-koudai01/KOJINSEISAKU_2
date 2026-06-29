#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGameClear.h"
#include "ModeGame.h"
#include "Title.h"
#include "BulletManager.h"

bool ModeGameClear::Initialize()
{
	if (!base::Initialize()) return false;

	return true;
}

bool ModeGameClear::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeGameClear::Process()
{
	base::Process();

	auto modeServer = ModeServer::GetInstance();

	// 下のレイヤーを動かさない
	modeServer->SkipProcessUnderLayer();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// 決定ボタンでこのモードを閉じる
	if(trg & PAD_INPUT_1)
	{
		ModeBase* game = modeServer->Get("game");
		if(game)
		{
			modeServer->Del(game); // ゲームモードも削除してタイトルに戻る
		}

		using namespace UI;

		// 新しいゲーム本体を追加予約
		modeServer->Add(new ModeGame(), 1, "game");

		// ゲームモードを削除
		modeServer->Del(this);
	}
	// PAD_INPUT_2: タイトルへ戻る
	else if(trg & PAD_INPUT_2)
	{
		ModeBase* game = modeServer->Get("game");
		if(game)
		{
			modeServer->Del(game);
		}

		// タイトルが無ければ追加（重複防止）
		ModeBase* title = modeServer->Get("title");
		if(!title)
		{
			modeServer->Add(new Title(), 1, "title");
		}

		modeServer->Del(this);
	}
	return true;
}

bool ModeGameClear::Render()
{
	base::Render();

	// 半透明の背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(200, 150, 1000, 600, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int spce = UI::GAMECLEAR_SPCE;
	// ゲームクリアロゴの描画
	SetFontSize(72);
	DrawString(350, 300, UI::GAMECLEAR_LOGO, GetColor(255, 255, 255));
	
	// リスタート
	SetFontSize(36);
	DrawString(350, 500, UI::GAMECLEAR_RESUME, GetColor(200, 200, 200));

	// タイトルに戻る
	SetFontSize(36);
	DrawString(350 + spce, 500, UI::GAMECLEAR_TITLE, GetColor(200, 200, 200));

	return true;
}