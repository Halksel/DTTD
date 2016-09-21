#include "S_Fail.h"

S_Fail::S_Fail(ISceneChanger* changer) : BaseScene(changer) {}
S_Fail::S_Fail(ISceneChanger* changer,Data *_data) : BaseScene(changer),data(_data) {}

void S_Fail::Initialize(){
	failHandle = LoadGraph("Picture/Screen/Stage_Failed.png");
	retry = UI(Pos(295, 300), "Picture/UI/Button_Retry.png",true,false,false);
}

void S_Fail::Finalize(){}

void S_Fail::Update(){
	if (retry.InMouseClick()){
		mSceneChanger->ChangeScene(eScene_S_Game);
	}
}

void S_Fail::Draw(){
	BaseScene::Draw();
	DrawGraph(0, 0, failHandle, FALSE);
	retry.Draw(&retry);
}
