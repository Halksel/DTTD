#pragma once

#include "DxLib.h"
#include "Object.h"

using namespace std;

const int SizeX = 40;
const int SizeY = 40;

class Particle{
	int x, y, alpha, time, vtime, live_f;
	static int destx, desty, particleHandle;
	double v, theta,angle, size, dx, dy;
	bool finish,end;
	vector<Particle*> particles;
public:
	Particle(){};
	Particle(int, int);
	static void Draw(Particle *);
	static void Update(Particle *);
	void UpdateParticles();
	void DrawParticles();
	void SetDestination(int, int);
	void Factory(Particle*);
	void Clear();
};

class Status {
public:
	int damage,slow;
	static int SlowMAX;
	double time,curetime, starttime,sliptime;//時間経過、完治までの時間、発生間隔、開始時間
	bool slowflag;//鈍化させるか否か、鈍化処理のための変数
	Status() {};
	Status(char *);
};

class Enemy : public Object{
private:
	static int birthnum;
	static bool birthflag;
	static vector < vector<int> > route,routeDir;//エネミーの通れる道;
	static vector<Status> statusList;

	Status status;
	STATE state;
	int kind, maxhp, sx, sy, vx, vy,*now,bfrprog;//種類、最大体力、状態異常、現在の座標を添え字に変換したもの、速度成分、現在ウェーブのポインタ、直前の進行度
	int deathSoundH,atkedflag;
	int R, G, B,colY = 40;
	double birthtime;
	vector<int> hp, atk, money;
	vector<double> speed;
	vector<ATTRIBUTE> attribute;
	bool existflag,priorityflag,directflag,particleflag,effectflag,aflag,oncebirth;//生存フラグ、優先されるべきか、方向が求まったかどうか,Particleのフラグ,攻撃フラグ,一度生まれたか
	Pos startSuf;//初期座標の添え字
	Rect rect;
public:
	int progress,id;
	Enemy();
	~Enemy() {};
	Enemy(int,int *,string,int,int,int);
	bool operator<(const Enemy&) const;
	static void Initilize();
	static void Update(Enemy *);
	static void Draw(Enemy *);
	static void Spawn(Enemy *);
	static int Move(Enemy *);
	static void SetRoute(vector<vector<int>> *, vector<vector<int>> *);
	static int GetBirthnum();
	static bool GetBirthFlag();
	static void Health(Enemy *);
	int Sucide();
	void PosSet(int ,int);
	bool Exist();
	int GetKind();
	int GetAtk();
	int GetMoney();
	int GetHP();
	int GetStatus();
	ATTRIBUTE GetAttribute();
	bool GetPriorityFlag();
	bool IsBirth();
	void DrawFourPoint();
	void SetPriorityFlag(bool);
	void SetProgress(int);
	void SetID(int);
	void SetStatus(STATE);
	void SetBirthTime(double);
	static bool HPMinus(Enemy *,int);
	Pos GetPosition();
	Rect GetRect();
};