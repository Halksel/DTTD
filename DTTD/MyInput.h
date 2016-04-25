#pragma once

#include "Object.h"
#include "KeyConfig.h"

class MyInput{
	static const int KeyNum = 256;				//入力キーの最大数
	static const int PadMax = 4;				//接続できるゲームパッドの最大数
	static const int PadNum = 32;				//パッドのボタンの最大数
	static const int MouseButNum = 8;			//マウスのボタンの最大数
	static const int Pads[PadMax];				
	static const int PadMasks[PadNum];
	static const int MouseMasks[MouseButNum];
	static const int KEY_INPUT_CONTROL[2];
	static const int KEY_INPUT_SHIFT[2];
	static const int KEY_INPUT_ALT[2];
	static const int InputInterval = 60;			//二回押し、ダブルクリックなどの間隔

	KeyConfig KC;								//キーコンフィグ

	int keyPressedCnt[KeyNum];
	bool keyFlag[KeyNum];
	int keyReleasedCnt[KeyNum];					//			　離されカウンター
	int padNum;									//接続しているゲームパッドの数
	int padPressedCnt[PadMax][PadNum];			//パッドの押されカウンター
	int padReleasedCnt[PadMax][PadNum];			//		　離されカウンター
	int mousePressedCnt[MouseButNum];			//マウスの押されカウンター
	int mouseReleasedCnt[MouseButNum];			//		　離されカウンター
	int mouseReleasedPrevCnt[MouseButNum];		//直前に話されてからのカウンター
	int mousePressCnt[MouseButNum];					//何回押されたかのカウンター

	int mouseRot;								//マウスホールの回転量
	int mouseRotSum;							//マウスホイールの回転量の合計

	Pos mousePoint;								//マウスの座標

	bool UpdateKey();
	bool UpdatePad();							//パッドの入力状態を取得
	bool UpdateMouse();							//マウスの入力状態を取得
public:
	MyInput();
	static const int NumberKey[10];				//数字キーにループでアクセスする用
	void Initialize();
	enum State{
		PRESSED = 0,
		PRESSED_NOT,
		PRESSED_ONCE,
		STATE_Size,
	};
	//ゲームパッドの入力を列挙
	enum PadInput {
		PAD_Down = 0, PAD_Left, PAD_Right, PAD_Up,
		PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9, PAD_10,
		PAD_11, PAD_12, PAD_13, PAD_14, PAD_15, PAD_16, PAD_17, PAD_18, PAD_19, PAD_20,
		PAD_21, PAD_22, PAD_23, PAD_24, PAD_25, PAD_26, PAD_27, PAD_28,
		PAD_Size,
	};

	//マウスの入力を列挙
	enum MouseInput {
		MOUSE_1 = 0, MOUSE_2, MOUSE_3, MOUSE_4, MOUSE_5, MOUSE_6, MOUSE_7, MOUSE_8,
		MOUSE_Size,
	};
	bool Update();
	int GetKey(const int);
	int GetKeyDef(int);
	bool GetKeyPress(const int, State);
	bool GetKeyPressDef(int, State);
	bool GetKeyInterval(const int, int ,State);
	bool GetKeyIntervalDef(int, int, State);
	bool GetKeySwitched(const int);
	bool GetKeySwitchedDef(int);

	int GetPadPress(const int KeyCode,int _padnum, State par);
	bool GetPadInterval(const int KeyCode,int interval,int _padnum,State par);
	//キーボードとパッドの両方から入力を取得する
	int		GetInputPress(const int KeyCode, State par, int _padnum = 0);
	bool	GetInputInterval(const int KeyCode, int interval, int _padnum = 0, State par = PRESSED);
	//マウスからの入力
	int		GetMousePress(MouseInput MouseCode, State par);
	bool	GetMouseInterval(MouseInput MouseCode, int interval, State par);
	bool	GetMousePressDouble(MouseInput MouseCode);
	Pos		GetMousePos();
	int		GetMouseWheelRot();
	int		GetMouseWheelRotSum();
	int		GetMousePressLeft(State par = PRESSED);
	bool	GetMousePressLeftOnce();
	int		GetMousePressRight(State par = PRESSED);
	bool	GetMousePressRightOnce();
};

extern MyInput Input;