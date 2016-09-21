#include "S_Game.h"

using namespace std;

KeyConfig KC;

extern GlobalTimer* GT;
MouseVisual::MouseVisual(char * _handle){
	handle = LoadGraph(_handle);
	SetMouseDispFlag(false);
}

void MouseVisual::Draw(){
	DrawRotaGraph(getMouseX(), getMouseY(),0.1,0, handle, TRUE);
}

//座標、画像パス、可視化フラグ、透過フラグ、切り替えフラグの初期化
UI::UI(Pos _p, string _path, bool _visible, bool _transflag,bool _switchflag) :visible(_visible), transflag(_transflag),switchflag(_switchflag){
	p = _p;
	path = _path;
	handle = LoadGraph(_path.c_str());
	GetGraphSize(handle, &width, &height);
	flag = false;
	interval = 0;
	pushflag = false;
}
//見えている間だけ、クリック判定を受け取る
void UI::Update(UI * ui){
	if (ui->pushflag) {
		++ui->interval;
	}
	if (ui->interval > 30) {
		ui->pushflag = false;
		ui->interval = 0;
	}
	if (ui->visible){
		if (ui->InMouseClick()){
			ui->flag = !ui->flag;
			if (ui->switchflag){//切り替えフラグがあるときのみ、見えなくする
				ui->visible = false;
			}
			else {
				ui->pushflag = true;
				ui->interval = 0;
			}
		}
	}
}
//見えていないなら描画されない
void UI::Draw(UI* ui){
	if (ui->visible){
		if ( ui->pushflag) {
			SetDrawBright(150, 150, 150);
			DrawGraph(ui->p.x, ui->p.y, ui->handle, ui->transflag);
			SetDrawBright(255, 255, 255);
		}
		else if (ui->InMousePointer()) {
			SetDrawBright(200, 200, 200);
			DrawGraph(ui->p.x, ui->p.y, ui->handle, ui->transflag);
			SetDrawBright(255, 255, 255);
		}
		else{
			DrawGraph(ui->p.x, ui->p.y, ui->handle, ui->transflag);
		}
	}
}
//クリックされたかどうかを受け取る
bool UI::GetFlag(){
	return flag;
}
//見えているなら通常判定、見えていないなら常に偽
bool UI::InMouseClick(){
	if (visible){
		return p.x <= getMouseX() && (getMouseX() <= p.x + width) && p.y <= getMouseY() && (getMouseY() <= p.y + height) && getMouseLeftPress(PRESS_ONCE);
	}
	else{
		return false;
	}
}
void UI::SetVisible(bool f){
	visible = f;
}


Field::Field(){
	pause = UI(Pos(380,menuHeight),"Picture/UI/pause.png",true,true,false);
	retry = UI(Pos(295, 300), "Picture/UI/Button_Retry.png",false,false,true);
	trash = UI(Pos(640,410),"Picture/UI/trash.png",true,false,false);
	speed = UI(Pos(420, menuHeight), "Picture/UI/speedup.png", true, true, false);
}

Field::Field(int *_life,int *_money,int *_stage,int *_wave){
	life = _life;
	money = _money;
	wave = _wave;
	stage = _stage;
	baggroundHandle = LoadGraph("Picture/Screen/makimono.png");
	moneyHandle = LoadGraph("Picture/UI/sumi.png");
	lifeHandle = LoadGraph("Picture/UI/heart_black.png");
	waveHandle = LoadGraph("Picture/UI/wave.png");
	menuHeight = 20;
	lifeX = 80;
	moneyX = 160;
	waveX = 260;
	Width = 40;
	pause = UI(Pos(380,menuHeight),"Picture/UI/pause.png",true,true,false);
	retry = UI(Pos(295, 300), "Picture/UI/Button_Retry.png",false,false,true);
	trash = UI(Pos(640,410),"Picture/UI/trash.png",true,false,false);
	speed = UI(Pos(420, menuHeight), "Picture/UI/speedup.png", true, true, false);
}

void Field::Draw() {
	DrawGraph(0, 0, baggroundHandle, TRUE);
	DrawGraph(lifeX, menuHeight, lifeHandle, TRUE);
	DrawFormatString(lifeX+Width, menuHeight, Black, "%d", *life);
	DrawGraph(moneyX, menuHeight, moneyHandle, TRUE);
	DrawFormatString(moneyX+Width, menuHeight, Black, "%d", *money);
	DrawGraph(waveX, menuHeight, waveHandle,TRUE);
	DrawFormatString(waveX+Width, menuHeight, Black, "%d - %d", *stage+1,*wave+1);
	pause.Draw(&pause);
	retry.Draw(&retry);
	trash.Draw(&trash);
	speed.Draw(&speed);
}

void Field::Update(){
	pause.Update(&pause);
	retry.Update(&retry);
	trash.Update(&trash);
	speed.Update(&speed);
	retry.SetVisible(pause.GetFlag());
}

bool Field::Pause(){
	return pause.GetFlag();
}

bool Field::Retry(){
	return retry.InMouseClick();
}

bool Field::Trash() {
	return trash.InMouseClick();
}

bool Field::Speed() {
	return speed.InMouseClick();
}

int S_Game::wave = -1;
int S_Game::money = 100;
int S_Game::life = 10;
int S_Game::wavecp = -1;
int S_Game::moneycp = 100;
int S_Game::lifecp = 10;
int S_Game::stage = 0;
int S_Game::wavenum = 100;
deque<Tower> S_Game::onField = deque<Tower>(0);

S_Game::S_Game(ISceneChanger* changer) : BaseScene(changer) {
	finish = false;
	wave = wavecp;
	money = moneycp;
	life = lifecp;
	++wave ;
	if (wave > wavenum) {
		finish = true;
	}
	else {
		GT->SetTime(0.0);
		gainmoney = 0;
		buildmoney = 0;
		enemyspd = 0;
		enemyMaxSPD = 1;
		towerOnField = 0;
		f = Field(&life, &money, &stage, &wave);
		LoadEnemiesData();
		mv = MapViewer(stage);
		Enemy().Initilize();
		EnemyWaveInit(mv.FindStartPos());
		Enemy().SetRoute(mv.GetRoute(), mv.GetRouteDir());
		start = UI( Pos(mv.FindStartPos().x + BASISX, mv.FindStartPos().y + BASISY), "Picture/UI/Button_start.png", true, true, true);
		LoadTowersData();
		TowerWaveInit();
	}
}

S_Game::S_Game(ISceneChanger* changer, Data *d) :BaseScene(changer), data(*d) {
	finish = false;
	stage = data.first;
	wave = wavecp;
	money = moneycp;
	life = lifecp;
	++wave ;
	if (wave > wavenum) {
		finish = true;
	}
	else {
		GT->SetTime(0.0);
		gainmoney = 0;
		buildmoney = 0;
		enemyspd = 0;
		enemyMaxSPD = 1;
		towerOnField = 0;
		f = Field(&life, &money, &stage, &wave);
		LoadEnemiesData();
		mv = MapViewer(stage);
		Enemy().Initilize();
		EnemyWaveInit(mv.FindStartPos());
		Enemy().SetRoute(mv.GetRoute(), mv.GetRouteDir());
		start = UI(Pos(mv.FindStartPos().x + BASISX, mv.FindStartPos().y + BASISY), "Picture/UI/Button_start.png", true, true, true);
		LoadTowersData();
		TowerWaveInit();
	}
}

void S_Game::Initialize(){
	if (!finish) {
		faster = 1;
		f = Field(&life, &money, &stage, &wave);
		selecttower = false;
		selectmap = 0;
		objects.clear();
		for (auto it = enemies.begin(); it != enemies.end(); ++it) {
			objects.push_back(*it);
		}
	}
}

void S_Game::Finalize(){
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		delete *it;
	}
	for (auto it = enemylist.begin(); it != enemylist.end(); ++it) {
		delete *it;
	}
	if (finish){
		finish = false;
		wave = -1;
		money = 100;
		life = 10;
		stage = 0;
		wavenum = 100;
		wavecp = wave;
		moneycp = money;
		lifecp = life;
	}
	particle.Clear();
}

void S_Game::Update(){
	KC.Update();
	if (Input.GetKeyPress(KEY_INPUT_D,MyInput::PRESSED_ONCE)){
		DEBUGMODE = !DEBUGMODE;
	}
	f.Update();
	start.Update(&start);

	if (!f.Pause() && !finish){

		if (DEBUGMODE){
			if (getKeyPress(KEY_INPUT_G, PRESS_ONCE)){
				money = 10000;
			}
			if (!faster) {
				DrawFormatString(0, 570, Black, "sppedup");
			}
			if (Input.GetKeyPress(KEY_INPUT_E, MyInput::State::PRESSED_ONCE)) {
				wavecp++;
				starttower = onField;
				for (auto e : enemies) {
					money += e->Sucide();
				}
				moneycp = money;
				lifecp = life;
				if (wavecp > wavenum) {
					Data d{ -1,stage,-1,-1,-1,-1,"SSS" };
					(mSceneChanger)->ChangeSceneArg(eScene_S_Clear, &d);
				}
				else {
					Data d = { wave,wave,life,wavecp,moneycp,lifecp,"S" };
					(mSceneChanger)->ChangeSceneArg(eScene_S_Clear, &d);
				}
			}
		}
		if (Input.GetKeyPress(KEY_INPUT_S,MyInput::State::PRESSED_ONCE) ){
			faster = !faster;
		}
		GT->SetDT(faster ? 1.0 : 2.0);
		mv.Update();
		TowerUpdate();
		if (start.InMouseClick()) {
			GT->SetTime(0.0);
		}
		if (start.GetFlag()) {
			GT->OnCount();
			EnemyUpdate();
		}
		if (selecttower && (getMouseRightPress(PRESS_ONCE) || f.Trash())) {
			selecttower = false;
			tower.reset(new Tower());
		}
		if (getMouseLeftPress(PRESS_ONCE) && selecttower) {
			if (mv.GetSelectKind() == PUT) {
				SetTower(mv.GetSelectPos().x, mv.GetSelectPos().y);
			}
		}
		TowerAttack();
		if (life <= 0) {
			lifecp = 10;
			moneycp = 150;
			wavecp = -1;
			onField.clear();
			mSceneChanger->ChangeScene(eScene_S_Fail);
		}
		if (!ExistEnemies()) {
			Data d = { wave,money,life,wavecp,moneycp,lifecp };
			fstream fs1("Data/PlayData/datanum.txt");
			string s;
			stringstream ss;
			int n;
			getline(fs1.seekg(0, ios_base::cur), s);
			ss.str(s);
			ss >> n;
			ofstream ofs("Data/PlayData/Data" + itos(n + 1) + ".txt", ios::app);
			ofs << "Wave" << wave << ", Life" << life << ", money" << money << "\n";
			ofs << "Towers\n";
			for (auto n : onField) {
				ofs << "Kind:" << n.GetKind() << " Grade:" << n.GetGrade() << " Pos:" << n.GetPos().x << "," << n.GetPos().y << " ";
			}
			ofs << endl;
			fs1 << n + 1 << endl;
			fs1.close();
			ofs.close();
			wavecp++;
			starttower = onField;
			moneycp = money;
			lifecp = life;
			(mSceneChanger)->ChangeSceneArg(eScene_S_Clear, &d);
		}
		if (Input.GetKeyPress(KEY_INPUT_A, MyInput::State::PRESSED_ONCE)){
			Data d = { stage,0,0,0,0,0 };
			wavecp = wave-1;
			onField = starttower;
			faster = false;
			mSceneChanger->ChangeSceneArg(eScene_S_Game,&d);
		}
	}
	else if(f.Retry()){
		Data d = { stage,0,0,0,0,0 };
		wavecp = wave - 1;
		onField = starttower;
		faster = false;
		mSceneChanger->ChangeSceneArg(eScene_S_Game, &d);
	}
	else if (finish) {
		Data d{ -1,stage,-1,-1,-1,-1,"SSS" };
		(mSceneChanger)->ChangeSceneArg(eScene_S_Clear,&d);
	}
	particle.UpdateParticles();
}

void S_Game::Draw(){
	BaseScene::Draw();
	if (!finish) {
		//DrawFormatString(getMouseX(), getMouseY(), Yellow, "%4d,%4d", getMouseX(), getMouseY());
		if (!Input.GetKeySwitched(KEY_INPUT_M)) {
			mv.Draw();
		}
		f.Draw();
		start.Draw(&start);
		if (wave >= 0) {
			if (DEBUGMODE) {
				DrawFormatString(0, 580, Black, "gainmoney %d", gainmoney);
				DrawFormatString(0, 600, Black, "buildmoney %d", buildmoney);
				DrawFormatString(0, 480, Black, "%lf,%d,%d", *GT->GetTimePointer(), wavenum, waveenemynum[wave]);
			}
			for (auto it = waveTowers.begin(); it != waveTowers.end(); ++it) {
				(*it)->SetDarkflag(money < (*it)->GetCost());
				(*it)->Draw(it->get());
				(*it)->DrawInfo(it->get());
			}
			for (auto it = onField.begin(); it != onField.end(); ++it) {
				it->Draw(&(*it));
				it->DrawAttack(&(*it));
				it->DrawUpgrade(&(*it));
				if (DEBUGMODE) {
					DrawFormatString((it - onField.begin()) * 60, 550, Black, "%.3f", it->GetInterval());
					it->DrawRange(&(*it));
				}
			}
			for (auto it = enemies.begin(); it != enemies.end(); ++it) {
				(*it)->Draw((*it));
				if (DEBUGMODE) {
					DrawFormatString((it - enemies.begin()) * 45, 500, Black, "%d", (*it)->attribute[*(*it)->now]);
					DrawFormatString((it - enemies.begin()) * 45, 540, Black, "%d", (*it)->id);
				}
			}
			if (selecttower) {
				tower->Draw(tower.get());
				tower->DrawRange(tower.get());
			}
		}
	}
	particle.DrawParticles();
}

void S_Game::LoadEnemiesData(){
	enemyDataPath = "Data/Enemy/EnemyData.csv";
	string str;
	stringstream ss;
	ifstream ifs;
	ifs.open(enemyDataPath);

	if (!ifs.fail()){
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> enemynum;
		ss.str(empty_string);
		ss.clear();
		getline(ifs.seekg(0, ios_base::cur), str);
		ss.str(str);
		ss >> wavenum;
		ss.str(empty_string);
		ss.clear();
		timings.resize(wavenum);
		kinds.resize(wavenum);
		waveenemynum.resize(wavenum);
		for (int i = 0; i < wavenum; ++i){
			getline(ifs.seekg(0, ios_base::cur), str, ',');
			ss.str(empty_string);
			ss.clear();
			getline(ifs.seekg(0, ios_base::cur), str);
			ss.str(str);
			ss >> waveenemynum[i];
			ss.str(empty_string);
			ss.clear();
			timings[i].resize(waveenemynum[i]);
			kinds[i].resize(waveenemynum[i]);
			for (int j = 0; j < waveenemynum[i]; ++j){
				getline(ifs.seekg(0, ios_base::cur), str, ',');
				ss.str(str);
				ss >> timings[i][j];
				ss.str(empty_string);
				ss.clear();
				getline(ifs.seekg(0, ios_base::cur), str);
				ss.str(str);
				ss >> kinds[i][j];
				ss.str(empty_string);
				ss.clear();
			}
		}
		enemylist.resize(enemynum);
		for (int i = 0; i < enemynum; ++i){
			enemylist[i] = new Enemy(Enemy(wavenum,&wave,"Data/Enemy/Enemy" + itos(i + 1) + ".csv", i + 1, Pos(0, 0)));
		}
	}
	ifs.close();
}

void S_Game::EnemyWaveInit(Pos pos){
	enemies.clear();
	enemies.resize(waveenemynum[wave]);
	for (int i = 0; i < waveenemynum[wave]; ++i){
		for (int j = 0; j < enemylist.size(); ++j){
			if (kinds[wave][i] == enemylist[j]->GetKind()){//リストからコピー
				enemies[i] = new Enemy(*enemylist[j]);
				enemies[i]->SetBirthTime(timings[wave][i]);
				enemies[i]->PosSet(pos);
				enemies[i]->SetID(i);
				break;
			}
		}
	}
}

void S_Game::EnemyUpdate(){
	for (auto it = enemies.begin(); it != enemies.end(); ) {
		if ((*it)->IsBirth()){//時間になっており、一度も生まれていないなら
			(*it)->Spawn(*it);
		}
		if ((*it)->Exist() || (*it)->GetHP() > 0) {
			(*it)->Update(*it);
			life -= (*it)->Move(*it);
			if ((*it)->InMouseClick()) {
				for (auto it2 = enemies.begin(); it2 != enemies.end(); ++it2) {
					(*it2)->SetProgress(-1);
					(*it2)->SetPriorityFlag(false);
				}
				(*it)->SetPriorityFlag(true);
				(*it)->SetProgress(1000);
			}
			++it;
		}
		else {
			it = enemies.erase(it);
		}
	}
	sort(enemies.begin(), enemies.end());
}
//全滅ならFalse
bool S_Game::ExistEnemies(){
	if (!Enemy().GetBirthFlag() || Enemy().GetBirthnum() < waveenemynum[wave]){//一匹もうまれていないなら
		return true;
	}
	for (auto n : enemies){
		if (n->Exist()){
			return true;
		}
	}
	return false;
}

void S_Game::LoadTowersData(){
	towerDataPath = "Data/Tower/TowerData.csv";

	string str;
	stringstream ss;
	ifstream ifs;
	ifs.open(towerDataPath);
	if (!ifs.fail()){
		getline(ifs.seekg(0, ios_base::cur), str);
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> wavenum;
		ss.str(empty_string);
		ss.clear();
		getline(ifs.seekg(0, ios_base::cur), str);
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		ss.str(str);
		ss >> towernum;
		ss.str(empty_string);
		ss.clear();
		getline(ifs.seekg(0, ios_base::cur), str);
		usinglist.resize(towernum);
		for (int i = 0; i < towernum; ++i){
			usinglist[i].resize(WAVETOWERMAX);
			for (int j = 0; j < WAVETOWERMAX; ++j){
				getline(ifs.seekg(0, ios_base::cur), str, ',');
				ss.str(str);
				ss >> usinglist[i][j];
				ss.str(empty_string);
				ss.clear();
			}
		}
		towerlist.resize(towernum);
		for (int i = 0; i < towernum; ++i){
			towerlist[i].reset(new Tower("Data/Tower/Tower" + itos(i + 1) + ".csv", i + 1, Pos(0, 0),&money));
		}
	}
	TowerWaveInit();
}

void S_Game::TowerWaveInit(){
	if (wave == 0) {
		onField.clear();
		onFieldcp.clear();
		starttower.clear();
	}
	else {
		starttower = onField;
		for (auto && t : onField) {
			t.SetAtkTime(0.0);
		}
	}
	waveTowers.clear();
	waveTowers.resize(WAVETOWERMAX);
	for (int i = 0; i < WAVETOWERMAX; ++i){
		for (int j = 0; j < towerlist.size(); ++j){
			if (usinglist[stage][i] == towerlist[j]->GetKind()){//リストからコピー
				waveTowers[i].reset(new Tower( Tower(*towerlist[j].get())));
				waveTowers[i]->PosSet(Pos(640, i * 60 + 60));
			}
		}
	}
}

void S_Game::TowerUpdate(){
	for (auto it = onField.begin(); it != onField.end(); ++it){
		it->Update(&(*it));
	}
	for (int i = 0; i < waveTowers.size(); ++i){
		if ((waveTowers[i]->InMouseClick() || Input.GetKeyPress(Input.NumberKey[(i+1)%10],MyInput::PRESSED_ONCE)) && waveTowers[i]->GetCost() <= money){
			tower.reset( new Tower(Tower(*waveTowers[i])));
			selecttower = true;
			tower->PlaySE(1);
			break;
		}
	}
	if (selecttower){
		tower->PosSetRev(Pos(getMouseX(), getMouseY()));
	}
}

void S_Game::TowerAttack() {
	for (auto it = onField.begin(); it != onField.end(); ++it){
		it->UpdateOnField(&(*it));
		if ((it)->Interval()){//攻撃出来る
			for (auto it2 = enemies.begin(); it2 != enemies.end(); ++it2){
				if ( (*it2)->Exist() && ( (*it2)->GetAttribute() == NORMAL || (*it2)->GetAttribute() == it->GetAttribute())) {//対象は生きている
					if (RectCollision((*it2)->GetRect(), (it)->GetCircle())) {//攻撃範囲内に存在するなら
						it->EPSet((*it2)->GetPosition());
						(*it2)->SetStatus(it->GetState());
						if (!(*it2)->HPMinus((*it2),(it)->GetAtk())){//攻撃
							money += (*it2)->GetMoney();
							gainmoney += (*it2)->GetMoney();
							it->EPSet((*it2)->GetPosition());
						}
						if(it->GetNumber() == 1){
							break;
						}
					}
				}
			}
		}
	}
}

void S_Game::SetTower(int x,int y){
	if (FindTowerInField(x, y) && money - tower->GetCost() >= 0){//まだタワーが置かれておらず、コストが足りているなら
		tower->PosSet(Pos(x, y));
		onField.push_back(*tower);
		money -= tower->GetCost();
		buildmoney += tower->GetCost();
		tower->PlaySE(0);
		onFieldcp = onField;
		++towerOnField;
		selecttower = false;
	}
}

bool S_Game::FindTowerInField(int x,int y){
	for (auto n : onField){
		if (n.ConfirmPos(Pos(x, y))){
			return false;
		}
	}
	return true;
}
