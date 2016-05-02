#pragma once

#include "BaseScene.h"
#include "Tower.h"
#include "Enemy.h"
#include "KeyConfig.h"

using namespace std;

const int Size = 40;
static const double DeltaTime = 0.01;
extern Particle particle;

class MouseVisual{
	int handle;
public:
	MouseVisual(){};
	MouseVisual(char *);
	void Draw();
};

class UI :public Object{
private:
	int interval;
	bool visible, transflag,switchflag;//見えているかどうか、DrawGraphの第四引数の値、クリックされることにより見えなくなる処理を行うかどうか
	bool pushflag;
public:
	UI(){};
	~UI() {};
	UI(int, int, string, bool, bool,bool);
	void Draw(UI *);
	void Update(UI *);
	bool GetFlag();
	bool InMouseClick();
	void SetVisible(bool);
};

class Field {
	int *life, *money,*stage, *wave;
	int moneyHandle,  baggroundHandle, lifeHandle,waveHandle;
	int menuHeight, moneyX, baggroundX, lifeX,waveX,Width;
	UI pause, retry, trash, speed;
public:
	Field();
	Field(int*,int*,int*,int*);
	void Update() ;
	void Draw() ;
	bool Pause();
	bool Retry();
	bool Trash();
	bool Speed();
};

class S_Game : public BaseScene {
	static int wave,wavenum,money,life,stage;//staticData
	static deque<Tower> onField;
	Data data;
	static int wavecp, moneycp, lifecp;//copyData
	deque<Tower> onFieldcp, starttower;

	int enemyMaxSPD,gainmoney,buildmoney;
	int selectmap,enemynum,towernum,towerOnField,selectwt,enemyspd,SHandle;
	const unsigned int partionLevel = 4;
	bool selecttower, faster,finish;
	string enemyDataPath,towerDataPath;
	shared_ptr<Tower> tower;
	vector <int> waveenemynum;
	vector <Object*> objects;
	vector < Enemy* > enemylist,enemies;
	vector < shared_ptr<Tower> > towerlist, waveTowers;
	vector < vector<int> > kinds, route;//エネミーの通れる道;
	vector < vector<double> > timings;
	vector <vector<int>> usinglist;//stage毎に使えるタワーの種類
	Field f;
	MapViewer mv;
	UI start;

	unsigned long colNum;
	int objectNum;

public:
	S_Game(ISceneChanger* changer);
	S_Game(ISceneChanger* changer,Data*);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void LoadEnemiesData();
	void EnemyWaveInit(Pos);
	void EnemyUpdate();
	bool ExistEnemies();
	void LoadTowersData();
	void TowerWaveInit();
	void TowerUpdate();
	void TowerAttack();
	void SetTower(int , int);
	bool FindTowerInField(int,int);
};