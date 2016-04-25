#pragma once

#include "BaseScene.h"

class S_Fail : public BaseScene {
	int failHandle;
	UI retry;
	Data *data;
public:
	S_Fail(ISceneChanger* changer);
	S_Fail(ISceneChanger* changer,Data *data);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};
