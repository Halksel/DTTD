#pragma once

#include "S_Game.h"

class KeyConfig {
	int settingkey,first,second,keysuf;
	static int map[256];
	static vector<string> keyinfo,keyname,keydef;
	string s, s2;
	UI config,exit;
	bool flag;
public:
	KeyConfig();
	void Update();
	void Draw();
	void SwapKeyConfig(const int,const int);
	void ConfigInitialize();
	void ConfigSave();
	int GetKeyNow(const int);
	bool Exit();
	string Convert2String(int);
	int Convert2int(string);
};