#include "AppFrame.h"
#include "Title.h"
#include "ApplicationMain.h"
#include "ModeGame.h"


bool Title::Initialize()
{
	if (!base::Initialize()) return false;
	return true;
}

bool Title::Terminate()
{
	base::Terminate();
	return true;
}

bool Title::Process()
{
	base::Process();

	const int trg = ApplicationMain::GetInstance()->GetTrg();

	if(trg & PAD_INPUT_1)
	{
		ModeBase* game = ModeServer::GetInstance()->Get("game");
		if(game)
		{
			ModeServer::GetInstance()->Del(game); // ÉQÅ[ÉÄÉÇÅ[ÉhÇ‡çÌèúÇµÇƒÉ^ÉCÉgÉãÇ…ñﬂÇÈ
		}

		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
		ModeServer::GetInstance()->Del(this);
	}
}

bool Title::Render()
{
	base::Render();

	int x = UI::TITLR_LOGO_X, y = UI::TITLR_LOGO_Y;
	int w = 0, h = 0;
	int spce = UI::TITLE_SPCE;	
	GetDrawScreenSize(&w, &h);

	// îwåi(ëOñ ìhÇËÇ¬Ç‘Çµ)
	DrawBox(0, 0, w, h, GetColor(10, 15, 30), TRUE);
	
	// ÉçÉS
	SetFontSize(76);
	DrawString(x, y, UI::TITLR_LOGO, GetColor(255, 255, 255));

	// ÉqÉìÉg
	SetFontSize(36);
	DrawString(x - 70, y + spce, UI::TITLR_HINT, GetColor(255, 255, 255));

	return true;
}