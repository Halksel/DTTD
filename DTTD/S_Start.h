#pragma once

#include "BaseScene.h"

class S_Start : public BaseScene {
	int menuH,selectH;
	int mapnum;
	bool startflag,configflag;
	KeyConfig KC;
	UI config;
	Data *data;
	vector<UI> UIs;
public:
	S_Start(ISceneChanger* changer);
	S_Start(ISceneChanger* changer, Data * _data);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};
