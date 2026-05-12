
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGameOver.h"
#include "ModeGame.h"
#include "Title.h"	

bool ModeGameOver::Initialize()
{
	if (!base::Initialize()) return false;
	_fontSize = 48;
	return true;
}

bool ModeGameOver::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeGameOver::Process()
{
	base::Process();

	// 下のレイヤーを動かさない（モードオーバーレイ）
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// 決定でこのモードを閉じる（下位レイヤーは再開されます）
	if (trg & PAD_INPUT_1)
	{
		ModeBase* game = ModeServer::GetInstance()->Get("game");
		if(game)
		{
			ModeServer::GetInstance()->Del(game); // ゲームモードも削除してタイトルに戻る
		}

		// 新しいゲーム本体を追加予約
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");

		// ゲームモードを削除
		ModeServer::GetInstance()->Del(this);
	}
	else if(trg & PAD_INPUT_2)
	{
		ModeBase* game = ModeServer::GetInstance()->Get("game");
		if(game)
		{
			ModeServer::GetInstance()->Del(game);
		}
		// タイトルが無ければ追加（重複防止）
		ModeBase* title = ModeServer::GetInstance()->Get("title");
		if(!title)
		{
			ModeServer::GetInstance()->Add(new Title(), 1, "title");
		}
		// ゲームオーバーモードを削除
		ModeServer::GetInstance()->Del(this);
	}

	return true;
}

bool ModeGameOver::Render()
{
	base::Render();

	// 半透明の背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(200, 150, 1000, 600, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int spce = UI::GAMEOVER_SPCE;
	// ゲームクリアロゴの描画
	SetFontSize(72);
	DrawString(350, 300, UI::GAMEOVER_LOGO, GetColor(255, 255, 255));

	// リスタート
	SetFontSize(36);
	DrawString(350, 500, UI::GAMEOVER_RESUME, GetColor(200, 200, 200));

	// タイトルに戻る
	SetFontSize(36);
	DrawString(350 + spce, 500, UI::GAMEOVER_TITLE, GetColor(200, 200, 200));

	return true;
}