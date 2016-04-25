#pragma once

using namespace std;

typedef enum {
	//ƒV[ƒ“í—Ş
	eScene_S_Start,
	eScene_S_Game,
	eScene_S_Fail,
	eScene_S_Clear,

	eScene_None,
} eScene;
struct Data {
	int first, second, third;
	int four, five, six;
	string s1;
};

class ISceneChanger {
public:
	virtual ~ISceneChanger() = 0;
	virtual void ChangeScene(eScene NextScene) = 0;
	virtual void ChangeSceneArg(eScene, Data *) = 0;
};
