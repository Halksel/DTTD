#pragma once

const static int   BASEFPS	= 60;           //FPS
const static int   SAMPLE_N = 60;          //���ς����T���v����
const static double FRAME_T	= (1000.f/BASEFPS); //1�t���[���̎���

//SingltonClass
class GlobalTimer
{
public:
	int      mStartTime;     //����J�n����
	int      mCount;         //�J�E���^
	int      mWaitTime;      //�ҋ@����
	int      mSkipFrameNum;  //�X�L�b�v���ׂ��t���[����
	static GlobalTimer* Instance();
	double* GetTimePointer();
	void FPS_Update();
	void FPS_Draw();
	void FPS_Wait();
	int FPS_GetSkipFrameNum();
	double GetDT();
	void SetDT(double);
	void SetTime(double);
	void OnCount();
protected:
	GlobalTimer();
private:
	int counter, FPSTime[2], FPSTime_i, FPSCount;
	double time,FPS,minFPS,maxFPS,DT;
	bool count;
	const double DeltaTime = 0.1;
	static GlobalTimer* _Instance;
};
