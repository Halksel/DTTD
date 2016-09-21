#pragma once

#include "Object.h"
#include "MapEditor.h"
#include "Input.h"

const int WAVETOWERMAX = 4;

enum STATE{
	HEALTH = 0,
	SLOW,
	STATESIZE,
};

enum ATTRIBUTE{
	NORMAL =0,
	FLY,
	ATTRIBUTESIZE,
};


//進行度により管理されたエネミーデータを受け取り、進行度が最も高いエネミーから攻撃する

class Tower : public Object{
	int kind, grade, nowgrade, number, count,name;//種類、グレードの最大値、今のグレード、？、？
	int infoHandle, atkHandle, upgradeInfoHandle,upgradeHandle;
	int writeSoundH, holdSoundH;
	static int *money;
	double atktime,Effectx,Effecty,angle,darktime;
	bool flag,atkflag,upflag,flag2,flag3,flag4,darkflag;
	vector<int> atk, range, cost;//アップグレード後のデータまで配列で管理
	vector<STATE> state;
	vector<ATTRIBUTE> attribute;
	vector<double> interval;
	vector<string> paths, infopaths, atkpaths,upgradeInfoPaths;
	Pos EPos;
	Circle circle;
public:
	Tower();
	~Tower() {};
	Tower(string, int, Pos ,int *);
	static void Update(Tower *);
	static void UpdateOnField(Tower *);
	static void Upgrade(Tower *);
	static void Draw(Tower *);
	static void DrawInfo(Tower *);
	static void DrawAttack(Tower *);
	static void DrawRange(Tower *);
	static void DrawUpgrade(Tower *);
	void PlaySE(int);
	void PosSet(Pos);
	void PosSetRev(Pos);
	void EPSet(Pos);
	void SetDarkflag(bool);
	void SetAtkTime(double);
	bool ConfirmPos(Pos);
	bool Interval();
	int GetNumber();
	int GetKind();
	int GetAtk();
	int GetCost();
	int GetGrade();
	ATTRIBUTE GetAttribute();
	STATE GetState();
	double GetInterval();
	Pos GetPos();
	Circle GetCircle();
};