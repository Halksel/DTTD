#include "S_Start.h"

S_Start::S_Start(ISceneChanger* changer) : BaseScene(changer) {}
S_Start::S_Start(ISceneChanger* changer, Data *_data) : BaseScene(changer),data(_data) {}

void S_Start::Initialize(){
	menuH = LoadGraph("Picture/Screen/start_gamen.png");
	selectH = LoadGraph("Picture/Screen/Map_Select.png");
	startflag = false;
	configflag = false;

	string str;
	stringstream ss;
	ifstream ifs("Data/Map/TotalMap.txt");
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> mapnum;
	ss.str(empty_string);
	ss.clear();
	ifs.close();
	UIs.resize(mapnum);
	for (int i = 0; i < mapnum; ++i) {
		string s = "Picture/UI/Map" + itos(i + 1) + ".png";
		UIs[i] = UI(Pos(100+i*120,100+(i/2)*120),s.c_str(),true,true,false);
	}
	config = UI(Pos(280, 400), "Picture/UI/Config.png", true, true, false);
}

void S_Start::Finalize(){}

void S_Start::Update(){
	config.Update(&config);
	if (config.InMouseClick()) {
		configflag = true;
	}
	else if(getMouseLeftPress(PRESS_ONCE) && !startflag) {
		startflag = true;
	}
	if (configflag) {
		KC.Update();
		if (KC.Exit()) {
			configflag = false;
		}
	}
	if (startflag) {
		for (int i = 0; i < mapnum; ++i) {
			if (UIs[i].InMouseClick()) {
				Data d = { i,0,0,0,0,0 };
				mSceneChanger->ChangeSceneArg(eScene_S_Game, &d);
			}
		}
	}
}

void S_Start::Draw(){
	BaseScene::Draw();
	if(configflag == startflag){
		DrawGraph(0, 0, menuH, FALSE);
	}
	if(startflag){
		DrawGraph(0, 0, selectH, FALSE);
		for (auto n : UIs) {
			n.Draw(&n);
		}
	}
	if (configflag) {
		KC.Draw();
	}
	else if(!startflag){
		config.Draw(&config);
	}
}
