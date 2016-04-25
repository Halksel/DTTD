#include "MyInput.h"

const int MyInput::NumberKey[] = {KEY_INPUT_0,KEY_INPUT_1,KEY_INPUT_2,KEY_INPUT_3,KEY_INPUT_4,KEY_INPUT_5,KEY_INPUT_6,KEY_INPUT_7,KEY_INPUT_8,KEY_INPUT_9};
const int MyInput::Pads[] = { DX_INPUT_PAD1, DX_INPUT_PAD2, DX_INPUT_PAD3, DX_INPUT_PAD4 };
const int MyInput::PadMasks[] = {	PAD_INPUT_DOWN, PAD_INPUT_LEFT, PAD_INPUT_RIGHT, PAD_INPUT_UP,
									PAD_INPUT_1, PAD_INPUT_2, PAD_INPUT_3, PAD_INPUT_4, PAD_INPUT_5, PAD_INPUT_6, PAD_INPUT_7, PAD_INPUT_8, PAD_INPUT_9, PAD_INPUT_10,
									PAD_INPUT_11, PAD_INPUT_12, PAD_INPUT_13, PAD_INPUT_14, PAD_INPUT_15, PAD_INPUT_16, PAD_INPUT_17, PAD_INPUT_18, PAD_INPUT_19, PAD_INPUT_20,
									PAD_INPUT_21, PAD_INPUT_22, PAD_INPUT_23, PAD_INPUT_24, PAD_INPUT_25, PAD_INPUT_26, PAD_INPUT_27, PAD_INPUT_28 };
const int MyInput::MouseMasks[] = { MOUSE_INPUT_1, MOUSE_INPUT_2, MOUSE_INPUT_3, MOUSE_INPUT_4, MOUSE_INPUT_5, MOUSE_INPUT_6, MOUSE_INPUT_7, MOUSE_INPUT_8 };

const int MyInput::KEY_INPUT_CONTROL[2] = { KEY_INPUT_LCONTROL, KEY_INPUT_RCONTROL };
const int MyInput::KEY_INPUT_SHIFT[2] = { KEY_INPUT_LSHIFT, KEY_INPUT_RSHIFT };
const int MyInput::KEY_INPUT_ALT[2] = { KEY_INPUT_LALT, KEY_INPUT_RALT };

MyInput::MyInput() {
	Initialize();
}

void MyInput::Initialize(){
	for (int i = 0; i < KeyNum; i++){
		keyPressedCnt[i] = 0;
		keyFlag[i] = false;
	}

	padNum = GetJoypadNum();					//接続しているパッドの数を取得
	for (int i = 0; i < PadMax; i++){
		for (int j = 0; j < PadNum; j++){
			padPressedCnt[i][j] = 0;
			padReleasedCnt[i][j] = 0;
		}
	}

	for (int i = 0; i < MouseButNum; i++){
		mousePressedCnt[i] = 0;
		mouseReleasedCnt[i] = 0;
		mousePressCnt[i] = 0;
	}
	mouseRot = 0;
	mouseRotSum = 0;
}

bool MyInput::Update(){
	//キーボード,　パッド,　マウスの入力状態を更新
	return UpdateKey() && UpdatePad() && UpdateMouse();
}
bool MyInput::UpdateKey() {
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {
			keyReleasedCnt[i] = 0;
			keyPressedCnt[i]++;
			if (keyPressedCnt[i] == 1) {
				keyFlag[i] = !keyFlag[i];
			}
		}
		else {
			keyPressedCnt[i] = 0;
			++keyReleasedCnt[i];
		}
	}
	return true;
}
bool MyInput::UpdatePad(){
	for (int i = 0; i < padNum; i++){
		for (int j = 0; j < PadNum; j++){
			if ((GetJoypadInputState(Pads[i]) & PadMasks[j]) == 0){	//ボタンが押されているなら
				if (padPressedCnt[i][j] != 0){
					padPressedCnt[i][j] = 0;
				}
				++padReleasedCnt[i][j];
			}	
			else{													//ボタンが押されていないなら
				if (padReleasedCnt[i][j] != 0){
					padReleasedCnt[i][j] = 0;
				}
				++padPressedCnt[i][j] = 0;
			}
		}
	}

	return true;
}

bool MyInput::UpdateMouse(){
	int tmpMouse = GetMouseInput();					//マウスの入力状態
	int tmpMouseX, tmpMouseY;						//マウスの座標
	GetMousePoint(&tmpMouseX, &tmpMouseY);			//マウスの座標を取得
	mousePoint = Pos(tmpMouseX, tmpMouseY);
	for (int i = 0; i < MouseButNum; i++){
		if (tmpMouse & MouseMasks[i]){				//ボタンが押されていれば
			mouseReleasedPrevCnt[i] += mouseReleasedCnt[i];
			mouseReleasedCnt[i] = 0;
			++mousePressedCnt[i];
			if (mousePressedCnt[i] == 1) {
				++mousePressCnt[i];
				if (mousePressCnt[i] > 2) {
					mousePressCnt[i] = 0;
				}
			}
			if (mouseReleasedPrevCnt[i] > InputInterval ) {
				mouseReleasedPrevCnt[i] = 0;
				mousePressCnt[i] = 0;
			}
		}
		else{										//ボタンが押されていなければ
			mousePressedCnt[i] = 0;
			++mouseReleasedCnt[i];
			mouseReleasedPrevCnt[i] = 0;
		}
	}
	mouseRot = GetMouseWheelRotVol();				//マウスホイールの回転量を取得
	mouseRotSum += mouseRot;
	return true;
}
int MyInput::GetKey(const int KeyCode) {
	return keyPressedCnt[KC.GetKeyNow(KeyCode)];
}
int MyInput::GetKeyDef(int KeyCode) {
	return keyPressedCnt[KeyCode];
}

bool MyInput::GetKeyPress(const int KeyCode, State par) {
	if (par == PRESSED) {
		return (keyPressedCnt[KC.GetKeyNow(KeyCode)]++ > 0);
	}
	else if (par == PRESSED_NOT) {
		return (keyReleasedCnt[KC.GetKeyNow(KeyCode)] > 0);
	}
	else if (par == PRESSED_ONCE) {
		return (keyPressedCnt[KC.GetKeyNow(KeyCode)]++ == 2);
	}
	return false;
}
bool MyInput::GetKeyPressDef(int KeyCode, State par) {
	if (par == PRESSED) {
		return (keyPressedCnt[KeyCode]++ > 0);
	}
	else if (par == PRESSED_NOT) {
		return (keyReleasedCnt[KeyCode]++ > 0);
	}
	else if (par == PRESSED_ONCE) {
		return (keyPressedCnt[KeyCode]++ == 2);
	}
	return false;
}

bool MyInput::GetKeyInterval(const int KeyCode,int interval, State per) {
	if (per == PRESSED) {
		return (keyPressedCnt[KC.GetKeyNow(KeyCode)] % interval == 1);
	}
	return keyReleasedCnt[KC.GetKeyNow(KeyCode)] % interval == 1;
}
bool MyInput::GetKeyIntervalDef(int KeyCode,int interval, State per) {
	if (per == PRESSED) {
		return (keyPressedCnt[KeyCode]++ % interval == 1);
	}
	return keyReleasedCnt[KeyCode]++ % interval == 1;
}

bool MyInput::GetKeySwitched(const int KeyCode) {
	return keyFlag[KC.GetKeyNow(KeyCode)];
}
bool MyInput::GetKeySwitchedDef(int KeyCode) {
	return keyFlag[KeyCode];
}

int MyInput::GetPadPress(const int KeyCode,int _padnum, State par) {
	if (_padnum >= padNum){
		return -1;
	}
	if (par == PRESSED){
		return padReleasedCnt[_padnum][KeyCode];
	}
	else if (par == PRESSED_ONCE){
		return padPressedCnt[_padnum][KeyCode] == 1;
	}
	return padPressedCnt[_padnum][KeyCode];
}
bool MyInput::GetPadInterval(const int KeyCode, int interval, int _padnum, State par){
	if (_padnum >= padNum){
		return false;
	}
	if (par == PRESSED_NOT){
		return padReleasedCnt[_padnum][KeyCode] % interval == 1;
	}
	return padPressedCnt[_padnum][KeyCode] % interval == 1;
}
//キーボードとパッドの両方から入力を取得する
int MyInput::GetInputPress(const int KeyCode, State par, int _padnum){
	if (par == PRESSED_NOT){
		return  min(GetKeyPress(KeyCode, par), GetPadPress(KeyCode, _padnum, par));
	}
	return max(GetKeyPress(KeyCode, par), GetPadPress(KeyCode, _padnum, par));
}
bool MyInput::GetInputInterval(const int KeyCode, int interval, int _padnum, State par){
	return GetKeyInterval(KeyCode, interval, par) || GetPadInterval(KeyCode, interval,_padnum, par);
}

//マウスからの入力を取得
int	MyInput::GetMousePress(MouseInput MouseCode, State par){
	if (par == PRESSED_NOT){
		return mouseReleasedCnt[MouseCode];
	}
	else if (par == PRESSED_ONCE){
		return mousePressedCnt[MouseCode]++ == 2;
	}
	return mousePressedCnt[MouseCode]++;
}
bool MyInput::GetMouseInterval(MouseInput MouseCode, int interval, State par){
	if (par == PRESSED_NOT){
		return mouseReleasedCnt[MouseCode] % interval == 1;
	}
	return mousePressedCnt[MouseCode] % interval == 1;
}
//マウスのダブルクリックを判定
bool MyInput::GetMousePressDouble(MouseInput MouseCode) {
	if (mouseReleasedPrevCnt[MouseCode] == 0 || mouseReleasedPrevCnt[MouseCode] >= InputInterval || mousePressCnt[MouseCode] != 1 ) return false;
	mouseReleasedPrevCnt[MouseCode] = 0;
	++mousePressCnt[MouseCode] ;
	return true;
}
//マウスの座標を取得
Pos	MyInput::GetMousePos(){
	return mousePoint;
}
//マウスホイールの回転量を取得
int	MyInput::GetMouseWheelRot(){
	return mouseRot;
}
int	MyInput::GetMouseWheelRotSum(){
	return mouseRotSum;
}
//マウスの左クリック
int	MyInput::GetMousePressLeft(State par){
	return GetMousePress(MOUSE_1, par);
}
//マウスの左クリック（ワンクリック）
bool MyInput::GetMousePressLeftOnce(){
	return GetMousePress(MOUSE_1, PRESSED_ONCE) == 1;
}
//マウスの右クリック
int	MyInput::GetMousePressRight(State par){
	return GetMousePress(MOUSE_2, par);
}
//マウスの右クリック（ワンクリック）
bool MyInput::GetMousePressRightOnce(){
	return GetMousePress(MOUSE_2, PRESSED_ONCE) == 1;
}
