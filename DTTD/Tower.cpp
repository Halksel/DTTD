#include "Tower.h"

int* Tower::money;
extern GlobalTimer* GT;
Tower::Tower(){};

Tower::Tower(string filepath, int _kind, int _x, int _y,int *_money){
	x = _x;
	y = _y;
	nowgrade = -1;
	kind = _kind;
	money = _money;
	writeSoundH = LoadSoundMem("Music/SE/SE_write.ogg");
	holdSoundH = LoadSoundMem("Music/SE/SE_hold.ogg");
	upgradeHandle = LoadGraph("Picture/Tower/tower_sword2.png");

	flag = false;
	flag2 = false;
	flag3 = false;
	flag4 = false;
	darkflag = false;
	Effectx = x;
	Effecty = y;
	time = GT->GetTimePointer();
	
	count = 0;
	atktime = 0;	

	string str;
	ifstream ifs;
	ifs.open(filepath);
	getline(ifs, str);
	str.clear();
	ifs >> name >> grade >> number;
	getline(ifs, str);
	getline(ifs, str);
	getline(ifs, str);

	paths.resize(grade);
	infopaths.resize(grade);
	atkpaths.resize(grade);
	upgradeInfoPaths.resize(grade);
	atk.resize(grade);
	state.resize(grade);
	attribute.resize(grade);
	interval.resize(grade);
	range.resize(grade);
	cost.resize(grade);
	int tmp = 0,tmp2=0;
	for (int i = 0; i < grade; ++i) {
		ifs >> paths[i] >> infopaths[i] >> upgradeInfoPaths[i] >> atkpaths[i] >> atk[i] >> tmp>> tmp2 >> interval[i] >> range[i] >> cost[i];
		state[i] = STATE(tmp);
		attribute[i] = ATTRIBUTE(tmp2);
	}
	Upgrade(this);
}

void Tower::Update(Tower *t){
	if (t->InMouseClick()){
		t->flag = !t->flag;
		t->flag2 = true;
	}
	if (t->atkflag){
		t->angle = atan2(t->EPos.y - t->y, t->EPos.x - t->x);
		if (t->name <= 2){//ここだよ
			if (t->Effectx == t->EPos.x && t->Effecty == t->EPos.y){
				t->Effectx == t->EPos.x;
				t->Effecty == t->EPos.y;
			}
			else{
				t->Effectx += 5 * cos(t->angle);
				t->Effecty += 5 * sin(t->angle);
			}
		}
		
	}
	else {
		t->Effectx = t->x;
		t->Effecty = t->y;
	}
	//現在時間から攻撃時間を引き、それが今のグレードのインターバルより短いなら攻撃できない
	if (*t->time - t->atktime > t->interval[t->nowgrade]){
		t->atkflag = FALSE;
	}
}

void Tower::UpdateOnField(Tower *t){
	if (t->InMouseDoubleClick()){
		t->flag3 = true;
		t->Upgrade(t);
	}
}

void Tower::Upgrade(Tower *t){
	if (t->nowgrade == -1){//初期化時
		++t->nowgrade;
		t->handle = LoadGraph(t->paths[t->nowgrade].c_str());
		GetGraphSize(t->handle, &t->width, &t->height);
		t->infoHandle = LoadGraph(t->infopaths[t->nowgrade].c_str());
		t->atkHandle = LoadGraph(t->atkpaths[t->nowgrade].c_str());
		t->upgradeInfoHandle = LoadGraph(t->upgradeInfoPaths[t->nowgrade].c_str());
	}//コスト要求
	else if (t->nowgrade < t->grade-1) {
		if (*money >= t->cost[t->nowgrade + 1] ){
			++t->nowgrade;
			t->handle = LoadGraph(t->paths[t->nowgrade].c_str());
			GetGraphSize(t->handle, &t->width, &t->height);
			t->infoHandle = LoadGraph(t->infopaths[t->nowgrade].c_str());
			t->atkHandle = LoadGraph(t->atkpaths[t->nowgrade].c_str());
			t->upgradeInfoHandle = LoadGraph(t->upgradeInfoPaths[t->nowgrade].c_str());
			*money -= t->cost[t->nowgrade];
		}
	}
}

void Tower::Draw(Tower *t){
	if (t->InMousePointer() || t->darkflag){//ポインターに刺されたとき暗化
		DrawBox(t->x, t->y, t->x + t->width*expWRate, t->y + t->height*expHRate, Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
		DrawExtendGraph(t->x, t->y,t->x+t->width*expWRate,t->y+t->height*expHRate, t->handle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else{
		DrawExtendGraph(t->x, t->y,t->x+t->width*expWRate,t->y+t->height*expHRate, t->handle, TRUE);
	}
	if (t->handle == -1){
		DrawString(t->x, t->y, ("Fail" + t->paths[t->nowgrade]).c_str(), Black);
	}
}
//タワーの情報をだしてくれる
void Tower::DrawInfo(Tower *t){
	if (t->InMousePointer()){
		DrawRotaGraph(t->x - 100, t->y, 0.3, 0, t->infoHandle, TRUE);
		DrawFormatString(t->x - 90, t->y, Black, "%d,%d,%d", t->cost[0], t->cost.at(1), t->atk[t->nowgrade]);
	}
}

void Tower::DrawAttack(Tower *t){
	//殴れるなら
	/*if (t->atkflag){
		if (t->name == 1){
			DrawGraph(t->Effectx, t->Effecty, t->atkHandle, TRUE);
		}
		else if(t->name == 2) {
			DrawGraph(t->Effectx, t->Effecty, t->atkHandle, TRUE);
		}
		else if (t->name == 3){
			DrawGraph(t->EPos.x, t->EPos.y, t->atkHandle, TRUE);
		}
		else if (t->name == 5){
			DrawGraph(t->x, t->y, t->atkHandle, TRUE);
		}
	}*/
}

void Tower::DrawRange(Tower *t){
	DrawCircle(t->x + t->width*expWRate / 2, t->y + t->height*expHRate / 2, t->range[t->nowgrade], Red, FALSE, 2);
}

void Tower::DrawUpgrade(Tower *t){

	if (t->InMousePointer()){
		DrawRotaGraph(t->x + 25, t->y - 50, 0.3, 0, t->upgradeInfoHandle, TRUE);
	}
}

void Tower::PlaySE(int n) {
	if (n == 0) {
		PlaySoundMem(writeSoundH, DX_PLAYTYPE_BACK);
	}
	else if (n == 1) {
		PlaySoundMem(holdSoundH, DX_PLAYTYPE_BACK);
	}
}

void Tower::PosSet(int _x, int _y){
	x = _x;
	y = _y;
}
//タワーの見かけ上の真ん中にセット
void Tower::PosSetRev(int _x, int _y){
	x = _x - width*expWRate / 2 ;
	y = _y - height*expHRate / 2 ;
}

void Tower::EPSet(Pos P){
	EPos = P;
}

void Tower::SetDarkflag(bool f) {
	darkflag = f;
}

void Tower::SetAtkTime(double t) {
	atktime = t;
}

//そこにタワーがあるかどうか
bool Tower::ConfirmPos(int _x, int _y){
	return (_x == x && _y == y);
}
//攻撃できるかどうか
bool Tower::Interval(){
	if (atktime == 0){
		return true;
	}
	return (*time - atktime > interval[nowgrade]);
}

int Tower::GetNumber() {
	return number;
}

int Tower::GetKind(){
	return kind ;
}

int Tower::GetAtk(){
	Effectx = x;
	Effecty = y;
	atktime = *time+DeltaTime;
	atkflag = true;
	return atk[nowgrade];
}

int Tower::GetCost(){
	return cost[nowgrade];
}

int Tower::GetGrade() {
	return nowgrade;
}

ATTRIBUTE Tower::GetAttribute(){
	return attribute[nowgrade];
}

STATE Tower::GetState() {
	return state[nowgrade];
}

double Tower::GetInterval() {
	return interval[nowgrade]-(*time-atktime);
}

Pos Tower::GetPos() {
	return Pos(x,y);
}

Circle Tower::GetCircle(){
	circle = Circle(x + width*expWRate / 2, y + height*expHRate / 2, range[nowgrade]);
	return move(circle);
}