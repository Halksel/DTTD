#pragma once

#include "Object.h"
#include "KeyConfig.h"

class MyInput{
	static const int KeyNum = 256;				//���̓L�[�̍ő吔
	static const int PadMax = 4;				//�ڑ��ł���Q�[���p�b�h�̍ő吔
	static const int PadNum = 32;				//�p�b�h�̃{�^���̍ő吔
	static const int MouseButNum = 8;			//�}�E�X�̃{�^���̍ő吔
	static const int Pads[PadMax];				
	static const int PadMasks[PadNum];
	static const int MouseMasks[MouseButNum];
	static const int KEY_INPUT_CONTROL[2];
	static const int KEY_INPUT_SHIFT[2];
	static const int KEY_INPUT_ALT[2];
	static const int InputInterval = 60;			//��񉟂��A�_�u���N���b�N�Ȃǂ̊Ԋu

	KeyConfig KC;								//�L�[�R���t�B�O

	int keyPressedCnt[KeyNum];
	bool keyFlag[KeyNum];
	int keyReleasedCnt[KeyNum];					//			�@������J�E���^�[
	int padNum;									//�ڑ����Ă���Q�[���p�b�h�̐�
	int padPressedCnt[PadMax][PadNum];			//�p�b�h�̉�����J�E���^�[
	int padReleasedCnt[PadMax][PadNum];			//		�@������J�E���^�[
	int mousePressedCnt[MouseButNum];			//�}�E�X�̉�����J�E���^�[
	int mouseReleasedCnt[MouseButNum];			//		�@������J�E���^�[
	int mouseReleasedPrevCnt[MouseButNum];		//���O�ɘb����Ă���̃J�E���^�[
	int mousePressCnt[MouseButNum];					//���񉟂��ꂽ���̃J�E���^�[

	int mouseRot;								//�}�E�X�z�[���̉�]��
	int mouseRotSum;							//�}�E�X�z�C�[���̉�]�ʂ̍��v

	Pos mousePoint;								//�}�E�X�̍��W

	bool UpdateKey();
	bool UpdatePad();							//�p�b�h�̓��͏�Ԃ��擾
	bool UpdateMouse();							//�}�E�X�̓��͏�Ԃ��擾
public:
	MyInput();
	static const int NumberKey[10];				//�����L�[�Ƀ��[�v�ŃA�N�Z�X����p
	void Initialize();
	enum State{
		PRESSED = 0,
		PRESSED_NOT,
		PRESSED_ONCE,
		STATE_Size,
	};
	//�Q�[���p�b�h�̓��͂��
	enum PadInput {
		PAD_Down = 0, PAD_Left, PAD_Right, PAD_Up,
		PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9, PAD_10,
		PAD_11, PAD_12, PAD_13, PAD_14, PAD_15, PAD_16, PAD_17, PAD_18, PAD_19, PAD_20,
		PAD_21, PAD_22, PAD_23, PAD_24, PAD_25, PAD_26, PAD_27, PAD_28,
		PAD_Size,
	};

	//�}�E�X�̓��͂��
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
	//�L�[�{�[�h�ƃp�b�h�̗���������͂��擾����
	int		GetInputPress(const int KeyCode, State par, int _padnum = 0);
	bool	GetInputInterval(const int KeyCode, int interval, int _padnum = 0, State par = PRESSED);
	//�}�E�X����̓���
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