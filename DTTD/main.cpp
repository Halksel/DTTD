#include "DxLib.h"
#include "SceneMgr.h"

MyInput Input;

GlobalTimer* GlobalTimer::_Instance = 0;

GlobalTimer* GT = GlobalTimer::Instance();

bool checkAll(){
	return ScreenFlip() == 0 && ProcessMessage() == 0 && gpUpdateKey() == 0 && gpUpdateMouse() == 0 && CheckHitKey(KEY_INPUT_TAB) == 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	SetMainWindowText("");
	SetGraphMode(WinWidth, WinHeight, 16), ChangeWindowMode(TRUE);
	DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK), ChangeFontType(DX_FONTTYPE_ANTIALIASING);
	SetAlwaysRunFlag(TRUE);
	SetFontSize(20);

	MouseVisual mv;
	mv = MouseVisual("Picture/UI/Mouse.png");
	SceneMgr sceneMgr;
	sceneMgr.Initialize();
	int SkipFrame = 0;
	SetBackgroundColor(255, 255, 255);
	while (checkAll()){
		Input.Update();
		GT->FPS_Update();
		sceneMgr.Update();
		//if (SkipFrame == 0) {
			ClearDrawScreen();
			sceneMgr.Draw();
			GT->FPS_Draw();
			mv.Draw();
			SkipFrame = GT->FPS_GetSkipFrameNum();
		/*}
		else {
			--SkipFrame;
		}
		/*DrawCircle(320, 320, 3, Black, TRUE);
		angle = atan2(*getMouseY()-320,*getMouseX()-320) ;
		DrawFormatString(400, 400, Black, "%lf", angle);
		DrawLine(320, 320, 320 + 40 * cos(angle), 320 + 40 * sin(angle), Black);*/
		GT->FPS_Wait();
	}

	DxLib_End();
	return 0;
}

