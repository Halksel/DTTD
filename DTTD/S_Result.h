#pragma once

#include "BaseScene.h"


class S_Result : public BaseScene {
public:
	S_Result(ISceneChanger* changer);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};
