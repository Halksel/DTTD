#include "Enemy.h"

int Particle::destx = 0;
int Particle::desty = 0;
int Particle::particleHandle = -1;
extern GlobalTimer* GT;
Particle particle;
Particle::Particle(int _x, int _y){
	x = _x;
	y = _y;
	particleHandle = LoadGraph("Picture/Enemy/ink.png");
	time = 0;
	live_f = true;
	v = GetRand(10);
	theta = GetRand(360) * PI / 180;
	dx = v * cos(theta);
	dy = v * sin(theta);
	size = 1.0*GetRand(100) / 100 * expWRate;
	alpha = GetRand(255);
	vtime = 60 + GetRand(60);
	finish = false;
	end = false;
	SetDestination(180, 20);
}

void Particle::Draw(Particle *p){
	if (!p->end){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, p->alpha);
		DrawRotaGraph(p->x, p->y, p->size, 0, p->particleHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Particle::Update(Particle *p){
	++p->time;
	if (p->time > p->vtime){
		p->v = 3;
		p->finish = true;
	}
	if (p->time > 2 * p->vtime){
		p->end = true;
	}
	if (!p->end){
		if ((p->vtime % p->time == 0 && !p->finish)){
			p->x += p->dx;
			p->y += p->dy;
		}
		else if (p->finish){
			if (!saferange(abs((p->destx)-p->x),abs((p->desty)-p->y),30,30)){
				p->angle = atan2((p->desty) - p->y, (p->destx) - p->x) ;
				p->x += p->v * cos(p->angle);
				p->y += p->v * sin(p->angle);
			}
			else{
				p->end = true;
			}
		}
	}
}

void Particle::UpdateParticles(){
	for (auto it = particles.begin(); it != particles.end(); ) {
		if ((*it)->end) {
			it = particles.erase(it);
		}
		else {
			(*it)->Update(*it);
			++it;
		}
	}
}

void Particle::DrawParticles(){
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		(*it)->Draw(*it);
	}
	if (particles.size() > 0) {
		DrawFormatString(700, 700, Black, "%d", particles.size());
	}
}

void Particle::SetDestination(int _x, int _y){
	destx = _x;
	desty = _y;
}

void Particle::Factory(Particle *p) {
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		if ((*it)->end) {
			*it = p;
			break;
		}
	}
	particles.push_back(p);
}

void Particle::Clear(){
	particles.clear();
}

int Status::SlowMAX = 2;

Status::Status(char * filepath) {
	time = 0.0;
	starttime = 0.0;
	slow = false;
	string str;
	stringstream ss;
	ifstream ifs(filepath);
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> curetime;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> sliptime;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> damage;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str);
	ss.str(str);
	ss >> slowflag;
	ss.str(empty_string);
	ss.clear();
}

int Enemy::birthnum = 0;
bool Enemy::birthflag = false;
vector < vector<int> > Enemy::route;
vector < vector<int> > Enemy::routeDir;
vector<Status> Enemy::statusList = {Status("Data/Enemy/Status/Health.csv"),Status("Data/Enemy/Status/Slow.csv")};

Enemy::Enemy(){};

Enemy::Enemy(int wavenum,int *_now,string filepath,int _kind,int _x, int _y){
	x = _x;
	y = _y;
	sx = 0; sy = 0;
	vx = 0; vy = 0;
	state = HEALTH;
	progress = 0;
	birthnum = 0;
	bfrprog = 0;
	time = GT->GetTimePointer();
	deathSoundH = LoadSoundMem("Music/SE/SE_death.ogg");
	R = G = B = 255;
	kind = _kind;
	existflag = false;
	priorityflag = false;
	birthflag = false;
	effectflag = false;
	aflag = false;
	oncebirth = false;
	atkedflag = 0;
	now = _now;
	status = statusList[HEALTH];

	string str;
	ifstream ifs;
	ifs.open(filepath);
	ifs >> path;
	getline(ifs, str);
	getline(ifs, str);
	handle = LoadGraph(path.c_str());
	fail = (handle == -1);
	GetGraphSize(handle, &width, &height);
	hp.resize(wavenum);
	attribute.resize(wavenum);
	speed.resize(wavenum);
	atk.resize(wavenum);
	money.resize(wavenum);
	int tmp = 0;
	for (int i = 0; i < wavenum; ++i) {
		ifs >> hp[i] >> tmp >> speed[i] >> atk[i] >> money[i];
		attribute[i] = (ATTRIBUTE)tmp;
	}
	maxhp = hp[*now];
}

bool Enemy::operator<(const Enemy &e) const{//要注意！演算子を逆に定義しています
	return progress>e.progress;
}

void Enemy::Initilize(){
	birthnum = 0;
	birthflag = false;
	route.clear();
}

//時間を管理、クリックされたら優先度を高くするようにフラグを立てる
void Enemy::Update(Enemy *e){
	if (e->existflag){
		Health(e);
	}
};

void Enemy::Draw(Enemy *e){
	if (e->aflag) {
		++e->atkedflag;
	}
	if (e->atkedflag > 15) {
		e->atkedflag = 0;
		e->aflag = false;
	}
	if (e->existflag){
		DrawFormatString(e->x+20,e->y,Red,"%d",e->id);
		if (!e->fail){
			if (e->state == HEALTH) {
				e->R = e->G = e->B = 255;
			}
			else if (e->state == SLOW) {
				e->R = e->G =200;
				e->B = 255;
			}
			if (e->InMousePointer()){
				SetDrawBright(e->R,e->G,e->B);
				DrawExtendGraph(e->x, e->y,e->x+e->width*expWRate,e->y+e->height*expHRate, e->handle, TRUE);
				SetDrawBright(255,255,255);
			}
			else if (e->aflag) {
				SetDrawBright(200, 0, 0);
				DrawExtendGraph(e->x, e->y,e->x+e->width*expWRate,e->y+e->height*expHRate, e->handle, TRUE);
				SetDrawBright(255,255,255);
			}
			else{
				SetDrawBright(e->R*0.85,e->G*0.85,e->B*0.85);
				DrawExtendGraph(e->x, e->y,e->x+e->width*expWRate,e->y+e->height*expHRate, e->handle, TRUE);
				SetDrawBright(255,255,255);
			}
			if (e->progress == 1000) {
				DrawBox(e->x,e->y-10,e->x+e->width*expWRate,e->y,Green,TRUE);
				DrawBox(e->x+e->width,e->y-10,e->x+(1.0 * e->hp[*e->now]/e->maxhp)*e->width,e->y,Red,TRUE);
			}
		}
		else{
			DrawFormatString(e->x, e->y, Black, "fail,%s", e->path.c_str());
		}
	}
};

//リスポ位置にエネミー生成
void Enemy::Spawn(Enemy *e){
	e->existflag = true;
	birthflag = true;
	for (int i = 0; i < route.size(); ++i){
		for (int j = 0; j < route[i].size(); ++j){
			if (route[i][j] == 0){
				e->startSuf = Pos(j, i);
				e->sx = i;
				e->sy = j;
				e->directflag = false;
				++birthnum;
			}
		}
	}
}
//エネミーが動く、返り値はゴールについたかどうか
int Enemy::Move(Enemy *e){
	if (e->existflag) {
		if (e->state == HEALTH || e->status.slow % 2 == 1 ) {
			if (e->x == (e->sx * (e->width*expWRate) + BASISX) && e->y == (e->sy * e->height*expHRate + e->colY + BASISY) && !e->directflag) {//丁度マスの上なら、方向探索
				e->vx = dx[routeDir[e->sx][e->sy]];
				e->vy = dy[routeDir[e->sx][e->sy]];
				e->directflag = true;//方向が定まった
				if (!e->priorityflag) {//優先度が高くない場合のみ
					if (e->attribute[*e->now] != 0) {
						e->progress = e->route[e->sx][e->sy] + 1000;
					}
					else {
						e->progress = e->route[e->sx][e->sy];
					}
				}
				if (e->attribute[*e->now] != 0) {
					e->bfrprog = e->route[e->sx][e->sy] + 1000;
				}
				else {
					e->bfrprog = e->route[e->sx][e->sy];
				}
			}
			else {//移動中
				if (e->vx > 0 && e->x <= (e->sx + e->vx) * e->width*expWRate + BASISX + e->width*expWRate) {
					e->x += e->speed[*e->now] * e->vx * GT->GetDT();
					if (e->x > (e->sx + e->vx) * e->width*expWRate + BASISX + e->width*expWRate)
					{
						e->x = (e->sx + e->vx) * e->width*expWRate + BASISX + e->width*expWRate;
					}
				}
				else if (e->vx < 0 && e->x >= (e->sx + e->vx) * e->width*expWRate + BASISX) {
					e->x += e->speed[*e->now] * e->vx * GT->GetDT();
					if (e->x < (e->sx + e->vx) * e->height*expHRate + BASISX) {
						e->x = (e->sx + e->vx) * e->height*expHRate + BASISX;
					}
				}
				if (e->vy > 0 && e->y <= ((e->sy + e->vy) * e->height*expHRate + e->colY + BASISY + e->height*expHRate)) {
					e->y += e->speed[*e->now] * e->vy* GT->GetDT();
					if (e->y > ((e->sy + e->vy) * e->height*expHRate + e->height*expHRate + BASISY + e->height)) {
						e->y = ((e->sy + e->vy) * e->height*expHRate + e->height*expHRate + BASISY + e->height);
					}
				}
				else if (e->vy < 0 && e->y >= ((e->sy + e->vy) * e->height*expHRate + e->colY + BASISY)) {
					e->y += e->speed[*e->now] * e->vy* GT->GetDT();
					if (e->y < ((e->sy + e->vy) * e->height*expHRate + e->colY + BASISY)) {
						e->y = ((e->sy + e->vy) * e->height*expHRate + e->colY + BASISY);
					}
				}//移動完了していたなら
				if (e->x == ((e->sx + e->vx) * e->height*expHRate + BASISX) && e->y == ((e->sy + e->vy) * e->height*expHRate + 40  + BASISY)) {
					e->sx += e->vx;
					e->sy += e->vy;
					e->directflag = false;
				}
			}
			if (route[e->sx][e->sy] == GetMaxDDT(route)) {//ゴールについたかどうか
				e->existflag = false;
				return e->atk[*e->now];
			}
		}
	}
	return 0;
}
//Getter,Setter
void Enemy::SetRoute(vector < vector<int> > *ddi,vector<vector<int>> *ddi2){
	route = *ddi;
	routeDir = *ddi2;
}

int Enemy::GetBirthnum(){
	return birthnum;
}

bool Enemy::GetBirthFlag(){
	return birthflag;
}

void Enemy::Health(Enemy *e) {
	if (e->state != HEALTH) {
		if (*e->time - e->status.time > e->status.sliptime) {
			e->hp[*e->now] -= e->status.damage;
			e->status.time = *e->time;
		}
		if (*e->time - e->status.starttime > e->status.curetime) {
			e->state = HEALTH;
		}
		else {
			if (e->status.slowflag) {
				++e->status.slow;
			}
		}
	}
}

int Enemy::Sucide() {
	existflag = false;
	hp[*now] = 0;
	return money[*now];
}

void Enemy::PosSet(int _x, int _y){
	x = _x + BASISX;
	y = _y + BASISY;
}

bool Enemy::Exist(){
	return existflag;
}

int Enemy::GetKind(){
	return kind;
}

int Enemy::GetAtk(){
	return atk[*now];
}

int Enemy::GetMoney(){
	return money[*now];
}

int Enemy::GetHP() {
	return hp[*now];
}

int Enemy::GetStatus() {
	return state;
}

ATTRIBUTE Enemy::GetAttribute(){
	return attribute[*now];
}

Pos Enemy::GetPosition(){
	return Pos(x, y);
}
//自身の四隅を返す
Rect Enemy::GetRect(){
	rect = Rect(Pos(x, y), Pos(x + width*expWRate, y), Pos(x, y + height*expHRate), Pos(x + width*expWRate, y + height*expHRate));
	return std::move(rect);
}

bool Enemy::GetPriorityFlag(){
	return priorityflag;
}

bool Enemy::IsBirth() {
	if (!oncebirth && *time > birthtime) {
		oncebirth = true;
		return true;
	}
	return false;
}

void Enemy::DrawFourPoint() {
	DrawCircle(x,y,2,Black,TRUE,TRUE);
	DrawCircle(x+width*expWRate,y,2,Black,TRUE,TRUE);
	DrawCircle(x,y+height*expHRate,2,Black,TRUE,TRUE);
	DrawCircle(x+width*expWRate,y+height*expHRate,2,Black,TRUE,TRUE);
}

void Enemy::SetPriorityFlag(bool f){
	if (progress < 1000) {
		priorityflag = f;
	}
}

void Enemy::SetProgress(int _p){
	if (_p == -1) {
		progress = bfrprog;
	}
	else {
		bfrprog = progress;
		progress = _p;
	}
}

void Enemy::SetID(int _id){
	id = _id;
}

void Enemy::SetStatus(STATE _s) {
	if (state == HEALTH) {
		state = _s;
		status = statusList[_s];
		status.starttime = *time;
		if (_s == SLOW) {
			status.slow = 0;
		}
	}
}

void Enemy::SetBirthTime(double _t) {
	birthtime = _t;
}

//攻撃を受ける関数、返り値は生存しているかどうか
bool Enemy::HPMinus(Enemy *e,int _atk){
	try {
		e->hp.at(*e->now) -= _atk;
		e->atkedflag = 1;
		e->aflag = true;
		e->atkedflag = 0;
		if (e->hp.at(*e->now) <= 0) {
			e->existflag = false;
			if (!e->effectflag) {
				e->effectflag = true;
			}
			PlaySoundMem(e->deathSoundH, DX_PLAYTYPE_BACK);
		}
	}
	catch (std::out_of_range &ex) {
		cerr << "Out of range:" << ex.what() << endl;
	}
	if (e->hp[*e->now] <= 0) {
		for (int i = 0; i < max(30, e->money[*e->now]); ++i) {
			auto p = new Particle(e->x + e->width / 2, e->y + e->height / 2);
			particle.Factory(p);
		}
	}

	return e->existflag;
}
