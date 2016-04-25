#include "DxLib.h"
#include "SceneMgr.h"
#include "S_Start.h"
#include "S_Game.h"
#include "S_Result.h"
#include "S_Fail.h"
#include "S_Clear.h"

SceneMgr::SceneMgr() : mNextScene(eScene_None) {
	mScene = (BaseScene*) new S_Start(this);
	argFlag = false;
}

void SceneMgr::Initialize(){
	mScene->Initialize();
}

void SceneMgr::Finalize(){ mScene->Finalize(); }

void SceneMgr::Update(){
	//ƒV[ƒ“ˆ—
	if(mNextScene != eScene_None){
		mScene->Finalize();

		delete mScene;
		if (!argFlag) {
			switch (mNextScene) {
			case eScene_S_Start:
				mScene = (BaseScene*) new S_Start(this);
				break;
			case eScene_S_Game:
				mScene = (BaseScene*) new S_Game(this);
				break;
			case eScene_S_Fail:
				mScene = (BaseScene*) new S_Fail(this);
				break;
			case eScene_S_Clear:
				mScene = (BaseScene*) new S_Clear(this);
			}
		}
		else {
			argFlag = false;
			switch (mNextScene) {
			case eScene_S_Start:
				mScene = (BaseScene*) new S_Start(this,data);
				break;
			case eScene_S_Game:
				mScene = (BaseScene*) new S_Game(this,data);
				break;
			case eScene_S_Fail:
				mScene = (BaseScene*) new S_Fail(this,data);
				break;
			case eScene_S_Clear:
				mScene = (BaseScene*) new S_Clear(this,data);
			}
			delete data;
		}
		mNextScene = eScene_None;
		mScene->Initialize();
	}
	mScene->Update();
}

void SceneMgr::Draw(){ mScene->Draw(); }

void SceneMgr::ChangeScene(eScene NextScene){
	mNextScene = NextScene;
}
void SceneMgr::ChangeSceneArg(eScene NextScene, Data *_data) {
	mNextScene = NextScene;
	argFlag = true;
	//data = _data;
	data = new Data(*_data);
}
