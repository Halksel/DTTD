#include"KeyConfig.h"

int KeyConfig::map[256] = {};
vector<string> KeyConfig::keyinfo;
vector<string> KeyConfig::keyname;
vector<string> KeyConfig::keydef;

KeyConfig::KeyConfig() {
	flag = false;
	for (int i = 0; i < 256; ++i) {
		map[i] = i;
	}
	s = "";
	s2 = "";
	string str;
	stringstream ss;
	ifstream ifs;
	ifs.open("Data/Key/KeyConfig.csv");
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> settingkey;
	ss.str(empty_string);
	ss.clear();
	keyinfo.resize(settingkey);
	keyname.resize(settingkey);
	keydef.resize(settingkey);
	for (int i = 0; i < settingkey; ++i) {
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> keyinfo[i];
		ss.str(empty_string);
		ss.clear();
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> keyname[i];
		ss.str(empty_string);
		ss.clear();
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> keydef[i];
		if (keyname[i] != keydef[i]) {
			SwapKeyConfig(Convert2int(keydef[i]),Convert2int(keyname[i]));
		}
		ss.str(empty_string);
		ss.clear();
	}
	ifs.close();
	config = UI(240, 400, "Picture/UI/Config.png", true, true, false);
	exit = UI(360, 400, "Picture/UI/Exit.png", true, true, false);
}

void KeyConfig::Update() {
	exit.Update(&exit);
	config.Update(&config);
	if (config.InMouseClick()) {
		flag = true;
	}

	if (Input.GetKeyPress(KEY_INPUT_ESCAPE,MyInput::PRESSED) ) {
		flag = false;
	}
	if (flag) {
		if (first == -1) {
			for (int i = 0; i < 256; ++i) {
				if (Input.GetKeyPressDef(i, MyInput::PRESSED_ONCE)) {
					auto it = keyname.end();
					if ((it = find(keyname.begin(), keyname.end(), Convert2String(i))) != keyname.end()) {
						first = i;
						keysuf = it - keyname.begin();
						break;
					}
				}
			}
		}
		else {
			for (int i = 0; i < 256; ++i) {
				if (Input.GetKeyPressDef(i, MyInput::PRESSED_ONCE)) {
					if (find(keyname.begin(), keyname.end(), Convert2String(i)) == keyname.end()) {
						second = i;
						keyname[keysuf] = Convert2String(second);
						SwapKeyConfig(first, second);
						ConfigSave();
						flag = false;
						break;
					}
				}
			}
		}

	}
	else {
		first = -1;
		second = -1;
	}
}

void KeyConfig::Draw() {
	config.Draw(&config);
	exit.Draw(&exit);
	if (flag) {
		DrawFormatString(200, 440, Black, "ESCキー以外を設定することができます、ESCキーで設定中断");
		if (first == -1) {
			DrawFormatString(200, 460, Black, "変更したいキーの一つ目を入力してください...");
		}
		else {
			DrawFormatString(200, 460, Black, "変更したいキーの二つ目を入力してください...");
		}
	}
	DrawString(200, 70, "役割　　　｜現在の割り振り ｜デフォルト", Black);
	for (int i = 0; i < settingkey; ++i) {
		DrawFormatString(200, 100 + i * 25, Black, "%10s: %15s: %10s", keyinfo[i].c_str(), keyname[i].c_str(), keydef[i].c_str());
	}
}


void KeyConfig::SwapKeyConfig(const int k1, const int k2) {
	swap(map[k1], map[k2]);
}

void KeyConfig::ConfigInitialize() {
	for (int i = 0; i < 256; ++i) {
		map[i] = i;
	}
}

void KeyConfig::ConfigSave() {
	ofstream ofs("Data/Key/KeyConfig.csv");
	ofs << settingkey << endl;
	for (int i = 0; i < settingkey; ++i) {
		ofs << keyinfo[i] + "," + keyname[i] + "," + keydef[i] +"," << endl;
	}
}

int KeyConfig::GetKeyNow(const int KeyCode) {
	return map[KeyCode];
}


bool KeyConfig::Exit() {
	return exit.InMouseClick();
}

string KeyConfig::Convert2String(int KeyCode) {
	switch (KeyCode)
	{
	case KEY_INPUT_BACK: return "BACK";
	case KEY_INPUT_TAB: return "TAB";
	case KEY_INPUT_RETURN: return "RETURN";
	case KEY_INPUT_LSHIFT: return "LSHIFT";
	case KEY_INPUT_RSHIFT: return "RSHIFT";
	case KEY_INPUT_LCONTROL: return "LCONTROL";
	case KEY_INPUT_RCONTROL: return "RCONTROL";
	case KEY_INPUT_ESCAPE: return "ESCAPE";
	case KEY_INPUT_SPACE: return "SPACE";
	case KEY_INPUT_PGUP: return "PGUP";
	case KEY_INPUT_PGDN: return "PGDN";
	case KEY_INPUT_END: return "END";
	case KEY_INPUT_HOME: return "HOME";
	case KEY_INPUT_LEFT: return "LEFT";
	case KEY_INPUT_UP: return "UP";
	case KEY_INPUT_RIGHT: return "RIGHT";
	case KEY_INPUT_DOWN: return "DOWN";
	case KEY_INPUT_INSERT: return "INSERT";
	case KEY_INPUT_DELETE: return "DELETE";
	case KEY_INPUT_MINUS: return "MINUS";
	case KEY_INPUT_YEN: return "YEN";
	case KEY_INPUT_PREVTRACK: return "PREVTRACK";
	case KEY_INPUT_PERIOD: return "PERIOD";
	case KEY_INPUT_SLASH: return "SLASH";
	case KEY_INPUT_LALT: return "LALT";
	case KEY_INPUT_RALT: return "RALT";
	case KEY_INPUT_SCROLL: return "SCROLL";
	case KEY_INPUT_SEMICOLON: return "SEMICOLON";
	case KEY_INPUT_COLON: return "COLON";
	case KEY_INPUT_LBRACKET: return "LBRACKET";
	case KEY_INPUT_RBRACKET: return "RBRACKET";
	case KEY_INPUT_AT: return "AT";
	case KEY_INPUT_BACKSLASH: return "BACKSLASH";
	case KEY_INPUT_COMMA: return "COMMA";
	case KEY_INPUT_KANJI: return "KANJI";
	case KEY_INPUT_CONVERT: return "CONVERT";
	case KEY_INPUT_NOCONVERT: return "NOCONVERT";
	case KEY_INPUT_KANA: return "KANA";
	case KEY_INPUT_APPS: return "APPS";
	case KEY_INPUT_CAPSLOCK: return "CAPSLOCK";
	case KEY_INPUT_SYSRQ: return "SYSRQ";
	case KEY_INPUT_PAUSE: return "PAUSE";
	case KEY_INPUT_LWIN: return "LWIN";
	case KEY_INPUT_RWIN: return "RWIN";
	case KEY_INPUT_NUMLOCK: return "NUMLOCK";
	case KEY_INPUT_NUMPAD0: return "NUMPAD0";
	case KEY_INPUT_NUMPAD1: return "NUMPAD1";
	case KEY_INPUT_NUMPAD2: return "NUMPAD2";
	case KEY_INPUT_NUMPAD3: return "NUMPAD3";
	case KEY_INPUT_NUMPAD4: return "NUMPAD4";
	case KEY_INPUT_NUMPAD5: return "NUMPAD5";
	case KEY_INPUT_NUMPAD6: return "NUMPAD6";
	case KEY_INPUT_NUMPAD7: return "NUMPAD7";
	case KEY_INPUT_NUMPAD8: return "NUMPAD8";
	case KEY_INPUT_NUMPAD9: return "NUMPAD9";
	case KEY_INPUT_MULTIPLY: return "MULTIPLY";
	case KEY_INPUT_ADD: return "ADD";
	case KEY_INPUT_SUBTRACT: return "SUBTRACT";
	case KEY_INPUT_DECIMAL: return "DECIMAL";
	case KEY_INPUT_DIVIDE: return "DIVIDE";
	case KEY_INPUT_NUMPADENTER: return "NUMPADENTER";
	case KEY_INPUT_F1: return "F1";
	case KEY_INPUT_F2: return "F2";
	case KEY_INPUT_F3: return "F3";
	case KEY_INPUT_F4: return "F4";
	case KEY_INPUT_F5: return "F5";
	case KEY_INPUT_F6: return "F6";
	case KEY_INPUT_F7: return "F7";
	case KEY_INPUT_F8: return "F8";
	case KEY_INPUT_F9: return "F9";
	case KEY_INPUT_F10: return "F10";
	case KEY_INPUT_F11: return "F11";
	case KEY_INPUT_F12: return "F12";
	case KEY_INPUT_A: return "A";
	case KEY_INPUT_B: return "B";
	case KEY_INPUT_C: return "C";
	case KEY_INPUT_D: return "D";
	case KEY_INPUT_E: return "E";
	case KEY_INPUT_F: return "F";
	case KEY_INPUT_G: return "G";
	case KEY_INPUT_H: return "H";
	case KEY_INPUT_I: return "I";
	case KEY_INPUT_J: return "J";
	case KEY_INPUT_K: return "K";
	case KEY_INPUT_L: return "L";
	case KEY_INPUT_M: return "M";
	case KEY_INPUT_N: return "N";
	case KEY_INPUT_O: return "O";
	case KEY_INPUT_P: return "P";
	case KEY_INPUT_Q: return "Q";
	case KEY_INPUT_R: return "R";
	case KEY_INPUT_S: return "S";
	case KEY_INPUT_T: return "T";
	case KEY_INPUT_U: return "U";
	case KEY_INPUT_V: return "V";
	case KEY_INPUT_W: return "W";
	case KEY_INPUT_X: return "X";
	case KEY_INPUT_Y: return "Y";
	case KEY_INPUT_Z: return "Z";
	case KEY_INPUT_0: return "0";
	case KEY_INPUT_1: return "1";
	case KEY_INPUT_2: return "2";
	case KEY_INPUT_3: return "3";
	case KEY_INPUT_4: return "4";
	case KEY_INPUT_5: return "5";
	case KEY_INPUT_6: return "6";
	case KEY_INPUT_7: return "7";
	case KEY_INPUT_8: return "8";
	case KEY_INPUT_9: return "9";
	default:
		return "Error";
	}
}
int KeyConfig::Convert2int(string KeyName) {
	if (KeyName == "BACK")return KEY_INPUT_BACK;
	if (KeyName == "TAB")return KEY_INPUT_TAB;
	if (KeyName == "RETURN")return KEY_INPUT_RETURN;
	if (KeyName == "LSHIFT")return KEY_INPUT_LSHIFT;
	if (KeyName == "RSHIFT")return KEY_INPUT_RSHIFT;
	if (KeyName == "LCONTROL")return KEY_INPUT_LCONTROL;
	if (KeyName == "RCONTROL")return KEY_INPUT_RCONTROL;
	if (KeyName == "ESCAPE")return KEY_INPUT_ESCAPE;
	if (KeyName == "SPACE")return KEY_INPUT_SPACE;
	if (KeyName == "PGUP")return KEY_INPUT_PGUP;
	if (KeyName == "PGDN")return KEY_INPUT_PGDN;
	if (KeyName == "END")return KEY_INPUT_END;
	if (KeyName == "HOME")return KEY_INPUT_HOME;
	if (KeyName == "LEFT")return KEY_INPUT_LEFT;
	if (KeyName == "UP")return KEY_INPUT_UP;
	if (KeyName == "RIGHT")return KEY_INPUT_RIGHT;
	if (KeyName == "DOWN")return KEY_INPUT_DOWN;
	if (KeyName == "INSERT")return KEY_INPUT_INSERT;
	if (KeyName == "DELETE")return KEY_INPUT_DELETE;
	if (KeyName == "MINUS")return KEY_INPUT_MINUS;
	if (KeyName == "YEN")return KEY_INPUT_YEN;
	if (KeyName == "PREVTRACK")return KEY_INPUT_PREVTRACK;
	if (KeyName == "PERIOD")return KEY_INPUT_PERIOD;
	if (KeyName == "SLASH")return KEY_INPUT_SLASH;
	if (KeyName == "LALT")return KEY_INPUT_LALT;
	if (KeyName == "RALT")return KEY_INPUT_RALT;
	if (KeyName == "SCROLL")return KEY_INPUT_SCROLL;
	if (KeyName == "SEMICOLON")return KEY_INPUT_SEMICOLON;
	if (KeyName == "COLON")return KEY_INPUT_COLON;
	if (KeyName == "LBRACKET")return KEY_INPUT_LBRACKET;
	if (KeyName == "RBRACKET")return KEY_INPUT_RBRACKET;
	if (KeyName == "AT")return KEY_INPUT_AT;
	if (KeyName == "BACKSLASH")return KEY_INPUT_BACKSLASH;
	if (KeyName == "COMMA")return KEY_INPUT_COMMA;
	if (KeyName == "KANJI")return KEY_INPUT_KANJI;
	if (KeyName == "CONVERT")return KEY_INPUT_CONVERT;
	if (KeyName == "NOCONVERT")return KEY_INPUT_NOCONVERT;
	if (KeyName == "KANA")return KEY_INPUT_KANA;
	if (KeyName == "APPS")return KEY_INPUT_APPS;
	if (KeyName == "CAPSLOCK")return KEY_INPUT_CAPSLOCK;
	if (KeyName == "SYSRQ")return KEY_INPUT_SYSRQ;
	if (KeyName == "PAUSE")return KEY_INPUT_PAUSE;
	if (KeyName == "LWIN")return KEY_INPUT_LWIN;
	if (KeyName == "RWIN")return KEY_INPUT_RWIN;
	if (KeyName == "NUMLOCK")return KEY_INPUT_NUMLOCK;
	if (KeyName == "NUMPAD0")return KEY_INPUT_NUMPAD0;
	if (KeyName == "NUMPAD1")return KEY_INPUT_NUMPAD1;
	if (KeyName == "NUMPAD2")return KEY_INPUT_NUMPAD2;
	if (KeyName == "NUMPAD3")return KEY_INPUT_NUMPAD3;
	if (KeyName == "NUMPAD4")return KEY_INPUT_NUMPAD4;
	if (KeyName == "NUMPAD5")return KEY_INPUT_NUMPAD5;
	if (KeyName == "NUMPAD6")return KEY_INPUT_NUMPAD6;
	if (KeyName == "NUMPAD7")return KEY_INPUT_NUMPAD7;
	if (KeyName == "NUMPAD8")return KEY_INPUT_NUMPAD8;
	if (KeyName == "NUMPAD9")return KEY_INPUT_NUMPAD9;
	if (KeyName == "MULTIPLY")return KEY_INPUT_MULTIPLY;
	if (KeyName == "ADD")return KEY_INPUT_ADD;
	if (KeyName == "SUBTRACT")return KEY_INPUT_SUBTRACT;
	if (KeyName == "DECIMAL")return KEY_INPUT_DECIMAL;
	if (KeyName == "DIVIDE")return KEY_INPUT_DIVIDE;
	if (KeyName == "NUMPADENTER")return KEY_INPUT_NUMPADENTER;
	if (KeyName == "F1")return KEY_INPUT_F1;
	if (KeyName == "F2")return KEY_INPUT_F2;
	if (KeyName == "F3")return KEY_INPUT_F3;
	if (KeyName == "F4")return KEY_INPUT_F4;
	if (KeyName == "F5")return KEY_INPUT_F5;
	if (KeyName == "F6")return KEY_INPUT_F6;
	if (KeyName == "F7")return KEY_INPUT_F7;
	if (KeyName == "F8")return KEY_INPUT_F8;
	if (KeyName == "F9")return KEY_INPUT_F9;
	if (KeyName == "F10")return KEY_INPUT_F10;
	if (KeyName == "F11")return KEY_INPUT_F11;
	if (KeyName == "F12")return KEY_INPUT_F12;
	if (KeyName == "A")return KEY_INPUT_A;
	if (KeyName == "B")return KEY_INPUT_B;
	if (KeyName == "C")return KEY_INPUT_C;
	if (KeyName == "D")return KEY_INPUT_D;
	if (KeyName == "E")return KEY_INPUT_E;
	if (KeyName == "F")return KEY_INPUT_F;
	if (KeyName == "G")return KEY_INPUT_G;
	if (KeyName == "H")return KEY_INPUT_H;
	if (KeyName == "I")return KEY_INPUT_I;
	if (KeyName == "J")return KEY_INPUT_J;
	if (KeyName == "K")return KEY_INPUT_K;
	if (KeyName == "L")return KEY_INPUT_L;
	if (KeyName == "M")return KEY_INPUT_M;
	if (KeyName == "N")return KEY_INPUT_N;
	if (KeyName == "O")return KEY_INPUT_O;
	if (KeyName == "P")return KEY_INPUT_P;
	if (KeyName == "Q")return KEY_INPUT_Q;
	if (KeyName == "R")return KEY_INPUT_R;
	if (KeyName == "S")return KEY_INPUT_S;
	if (KeyName == "T")return KEY_INPUT_T;
	if (KeyName == "U")return KEY_INPUT_U;
	if (KeyName == "V")return KEY_INPUT_V;
	if (KeyName == "W")return KEY_INPUT_W;
	if (KeyName == "X")return KEY_INPUT_X;
	if (KeyName == "Y")return KEY_INPUT_Y;
	if (KeyName == "Z")return KEY_INPUT_Z;
	if (KeyName == "0")return KEY_INPUT_0;
	if (KeyName == "1")return KEY_INPUT_1;
	if (KeyName == "2")return KEY_INPUT_2;
	if (KeyName == "3")return KEY_INPUT_3;
	if (KeyName == "4")return KEY_INPUT_4;
	if (KeyName == "5")return KEY_INPUT_5;
	if (KeyName == "6")return KEY_INPUT_6;
	if (KeyName == "7")return KEY_INPUT_7;
	if (KeyName == "8")return KEY_INPUT_8;
	if (KeyName == "9")return KEY_INPUT_9;
	return -1;
}