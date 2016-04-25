#include "GlobalTimer.h"

GlobalTimer::GlobalTimer() {
	time = 0.0;
	FPS = 0.0;
	minFPS = 60.0;
	maxFPS = 0.0;
	counter = 0;
	FPSTime[0] = 0;
	FPSTime[1] = 0;
	FPSTime_i = 0;
	FPSCount = 0;
	mStartTime = 0;
	mCount = 0;
	mWaitTime = 0;
	mSkipFrameNum = 0;
	count = false;
}

GlobalTimer* GlobalTimer::Instance()
{
   if(_Instance == 0)
      _Instance = new GlobalTimer;

   return _Instance;
}

double* GlobalTimer::GetTimePointer() {
	return &time;
}

void GlobalTimer::FPS_Update(){
    int t = GetNowCount();
    if( mStartTime != 0 ){//�ŏ���1��ڈȊO�ʂ�
        int gameTime = FRAME_T*mCount;
        int realTime = t-mStartTime;
        int sub = gameTime - realTime;
        mWaitTime = sub > 0 ? sub : 0;
        mSkipFrameNum = sub < 0 ? (int)( -sub / FRAME_T ) : 0;
    }
    if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
        mStartTime = GetNowCount();
    }
    if( mCount == SAMPLE_N ){ //SAMPLE_N�t���[���ڂȂ畽�ς��v�Z����
        FPS = 1000.f/((t-mStartTime)/(double)SAMPLE_N);//�v0���`�F�b�N
        mCount = 0;
        mStartTime = t;
		maxFPS = max(maxFPS, FPS);
		minFPS = min(minFPS, FPS);
    }
    mCount++;
	DT = (BASEFPS-mWaitTime)/ BASEFPS;
	if (count)
		time += 0.01 * DT;
	return;
}

void GlobalTimer::FPS_Draw() {
	if (FPS != 0) {
		DrawFormatString(500, 600, Red, "%lf", DT);
		if (DEBUGMODE) {
			DrawFormatString(WinWidth - GetDrawStringWidth("DebugMode: MaxFPS %.1f :MinFPS %.1f", 36), WinHeight - 20, Black, "DebugMode:");
		}
		DrawFormatString(WinWidth - GetDrawFormatStringWidth("FPS %.1f", FPS), WinHeight - 40, (BASEFPS - FPS > BASEFPS/10)? Red : Black, "FPS %.1f", FPS); //fps��\��
		DrawFormatString(WinWidth - GetDrawFormatStringWidth("MaxFPS %.1f :MinFPS %.1f", maxFPS,minFPS), WinHeight - 20, Black, "MaxFPS %.1f :MinFPS %.1f", maxFPS,minFPS); //fps��\��
	}
}

//�ҋ@
void GlobalTimer::FPS_Wait(){
    Sleep(mWaitTime);
}
 
//�X�L�b�v���ׂ��t���[������Ԃ�
int GlobalTimer::FPS_GetSkipFrameNum(){
    return mSkipFrameNum;
}

double GlobalTimer::GetDT() {
	return DT ;
}

void GlobalTimer::SetDT(double _f) {
	DT = _f;
}

void GlobalTimer::SetTime(double _f) {
	count = false;
	time = _f;
}

void GlobalTimer::OnCount() {
	count = true;
}

