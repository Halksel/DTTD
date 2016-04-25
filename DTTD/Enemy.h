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
	double time,curetime, starttime,sliptime;//���Ԍo�߁A�����܂ł̎��ԁA�����Ԋu�A�J�n����
	bool slowflag;//�݉������邩�ۂ��A�݉������̂��߂̕ϐ�
	Status() {};
	Status(char *);
};

class Enemy : public Object{
private:
	static int birthnum;
	static bool birthflag;
	static vector < vector<int> > route,routeDir;//�G�l�~�[�̒ʂ�铹;
	static vector<Status> statusList;

	Status status;
	STATE state;
	int kind, maxhp, sx, sy, vx, vy,*now,bfrprog;//��ށA�ő�̗́A��Ԉُ�A���݂̍��W��Y�����ɕϊ��������́A���x�����A���݃E�F�[�u�̃|�C���^�A���O�̐i�s�x
	int deathSoundH,atkedflag;
	int R, G, B,colY = 40;
	double birthtime;
	vector<int> hp, atk, money;
	vector<double> speed;
	vector<ATTRIBUTE> attribute;
	bool existflag,priorityflag,directflag,particleflag,effectflag,aflag,oncebirth;//�����t���O�A�D�悳���ׂ����A���������܂������ǂ���,Particle�̃t���O,�U���t���O,��x���܂ꂽ��
	Pos startSuf;//�������W�̓Y����
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