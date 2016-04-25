#pragma once

#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneMgr : public ISceneChanger, Task {

private:
    BaseScene* mScene;
    eScene mNextScene;
	bool argFlag;
	Data *data;
public:
	SceneMgr();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	void ChangeScene(eScene NextScene) override;
	void ChangeSceneArg(eScene NextScene, Data *data) override;
};