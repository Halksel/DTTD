#include "S_Result.h"

S_Result::S_Result(ISceneChanger* changer) : BaseScene(changer) {}


void S_Result::Initialize(){
}

void S_Result::Finalize(){}

void S_Result::Update(){
}

void S_Result::Draw(){
	BaseScene::Draw();
}
