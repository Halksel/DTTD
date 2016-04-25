#include "S_Clear.h"

S_Clear::S_Clear(ISceneChanger* changer) : BaseScene(changer) {
	stageclear = false;
}

S_Clear::S_Clear(ISceneChanger* changer, Data* _d) : BaseScene(changer), d(*_d){
	fontH = CreateFontToHandle(NULL,32,3);
	font2H = CreateFontToHandle(NULL,24,2);
	if (d.first == -1) {
		stageclear = true;
	}
	else {
		stageclear = false;
	}
}

void S_Clear::Initialize(){
	clearHandle = LoadGraph("Picture/Screen/Stage_Clear.png");
}

void S_Clear::Finalize(){}

void S_Clear::Update(){
	if (getMouseLeftPress(PRESS_ONCE)){
		if (stageclear) {
			mSceneChanger->ChangeScene(eScene_S_Start);
		}
		else {
			mSceneChanger->ChangeScene(eScene_S_Game);
		}
	}
}

void S_Clear::Draw(){
	BaseScene::Draw();
	DrawGraph(0,0,clearHandle,FALSE);
	if (!stageclear) {
		DrawFormatStringToHandle(320, 220, Black, fontH, "%d", d.first + 1);
		DrawFormatStringToHandle(200, 300, Black, font2H, "Life %d -> %d", d.six, d.third);
		DrawFormatStringToHandle(200, 326, Black, font2H, "Money %d -> %d", d.five, d.second);
	}
	else {
		DrawFormatStringToHandle(240, 260, Black, fontH, "%dStage Cleared!", d.second);
		for (int i = 0; i < static_cast<int>(d.s1.size()); ++i) {
			DrawFormatStringToHandle(200,(300 + i * 25), static_cast<unsigned int>( Black), font2H, "Wave %d   Score:%s", (i + 1), d.s1.c_str());
		}
	}
}
