#pragma once

#include "BaseScene.h"

class S_Clear : public BaseScene {
	int clearHandle;
	int fontH,font2H;
	bool stageclear;
	Data d;
public:
	S_Clear(ISceneChanger* changer);
	S_Clear(ISceneChanger* changer, Data *d);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};
