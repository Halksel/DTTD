#pragma once

const static int   BASEFPS	= 60;           //FPS
const static int   SAMPLE_N = 60;          //平均を取るサンプル数
const static double FRAME_T	= (1000.f/BASEFPS); //1フレームの時間

//SingltonClass
class GlobalTimer
{
public:
	int      mStartTime;     //測定開始時刻
	int      mCount;         //カウンタ
	int      mWaitTime;      //待機時間
	int      mSkipFrameNum;  //スキップすべきフレーム数
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
