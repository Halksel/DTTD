#include"MapEditor.h"

using namespace std;

MapFragment::MapFragment(){};

MapFragment::MapFragment(int _x, int _y, MAPSTATE _attribute, string _filePath) :x(_x), y(_y), attribute(_attribute), filePath(_filePath){
	drawH = LoadGraph(filePath.c_str());
	GetGraphSize(drawH, &width, &height);
};

void MapFragment::Update(){

}

void MapFragment::Draw(){
}

void MapFragment::DrawKind(){
	DrawFormatString(x,y, StringColor, " %d", attribute);
}

bool MapFragment::LoadFail(){
	return (-1 == drawH);
}

bool MapFragment::InMouseClick(){
	return x <= getMouseX() && (getMouseX() <= x + width*expWRate) && y <= getMouseY() && (getMouseY() <= y + height * expHRate) && getMouseLeftPress(PRESS_ONCE);
}

MAPSTATE MapFragment::GetAttribute(){
	return attribute;
}

Pos MapFragment::GetPos(){
	return Pos(x, y);
}

MapViewer::MapViewer(){};
MapViewer::MapViewer(int n){
	filePath = PATH + itos(n) + ".csv";
	SetData(n);
	GetGraphSize(handle, &mapW, &mapH);
};

void MapViewer::Update(){
	atbflag = false;
	for (auto it = fragments.begin(); it != fragments.end();++it){
		it->Update();
		if (it->InMouseClick()){
			select = it - fragments.begin();
			attribute = it->GetAttribute();
			selectPos = it->GetPos();
			atbflag = true;
		}
	}
	if (!atbflag){
		attribute = -1;
		select = -1;
	}
}

void MapViewer::Draw(){
	DrawExtendGraph(BASISX, BASISY + 40, BASISX + mapW*expWRate, BASISY + 40 + mapH*expHRate, handle, TRUE);
	if (getKeySwitched(KEY_INPUT_V)) {
		for (auto n : fragments) {
				n.DrawKind();
		}
	}
	if (getKeySwitched(KEY_INPUT_I)){
		DrawInfo();
	}
	if (getKeySwitched(KEY_INPUT_R)){
		DrawRoute();
	}
}

void MapViewer::DrawInfo(){
	DrawFormatString(0, 700, StringColor, "MapSize(W*H) = %d * %d, Imagenum = %d", mwidth, mheight,imagenum);
	DrawFormatString(0,720,StringColor,"FragmentSize(x*y) = %d * %d", width,height);
}

void MapViewer::DrawRoute(){
	for (int i = 0; i < mheight; ++i){
		for (int j = 0; j < mwidth; ++j){
			if (routeDir[j][i] != -1){
				DrawFormatString(j * width*expWRate+BASISX, (i * height )*expHRate+BASISY+40, Black, "%d", routeDir[j][i]);
			}
		}
	}
}

void MapViewer::SetData(int number){
	mwidth = 0;
	mheight = 0;
	handle = LoadGraph(("Picture/Map/Map" + itos(number) + ".png").c_str());
	string str;
	stringstream ss;
	ifstream ifs;
	ifs.open(filePath);
	fail = ifs.fail();
	getline(ifs.seekg(0, ios_base::cur), str, ',');
	Split(ds, str, ":");
	ss.str(ds[0]);
	ss >> width;
	ss.str(empty_string);
	ss.clear();
	ss.str(ds[1]);
	ss >> height;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str, ',');

	Split(ds, str, ":");
	ss.str(ds[0]);
	ss >>mwidth;
	ss.str(empty_string);
	ss.clear();
	ss.str(ds[1]);

	ss >> mheight;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str, ',');
	ss.str(str);
	ss >> imagenum;
	ss.str(empty_string);
	ss.clear();
	getline(ifs.seekg(0, ios_base::cur), str);
	filepaths.resize(imagenum);
	for (int i = 0; i < imagenum; ++i){//ファイル名の読み込み
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		str = "Picture/MapTip/" + str;
		ss.str(str);
		ss >> filepaths[i];
		ss.str(empty_string);
		ss.clear();
	}
	fragments.resize(mwidth*mheight);
	int k = 0;
	for (int i = 0, x = 0, y = 0; i < mheight*mwidth; ++i){
		int j = i % mwidth;
		if (i != 0 && j == 0) {
			++k;
			getline(ifs.seekg(0, ios_base::cur), str);
		}
		getline(ifs.seekg(0, ios_base::cur), str, ',');
		Split(ds, str, ":");
		ss.str(ds[0]);
		ss >> x;
		ss.str(empty_string);
		ss.clear();
		ss.str(ds[1]);
		ss >> y;
		if (y > 0) {
			fragments[pos(j, k)] = MapFragment(j*width*expWRate + BASISX, k*height*expHRate + BASISY+Y, (MAPSTATE)x, filepaths[y - 1]);
			if (x == START) {
				startPos = Pos(j*width*expWRate + X, k*height*expHRate + Y);
				startSuf = Pos(j, k);
			}
			else if (x == GOAL) {
				endPos = Pos(j*width*expWRate + X, k*height*expHRate + Y);
				endSuf = Pos(j, k);
			}
		}
		ss.str(empty_string);
		ss.clear();
	}
	route.resize(mwidth);
	for (auto it = route.begin(); it != route.end(); ++it) {
		it->resize(mheight);
		for (auto it2 = it->begin(); it2 != it->end();++it2){
			*it2 = 1000;
		}
	}
	this->SetStoERoute();
}

bool MapViewer::Fail(){
	return fail;
}

int MapViewer::GetSelect(){
	return select;
}

int MapViewer::GetSelectAttribute(){
	return attribute;
}

Pos MapViewer::GetSelectPos(){
	return selectPos;
}

void MapViewer::SetStoERoute(){
	queue<Pos> qpos;
	int x, y;
	Pos tmp = startSuf;
	qpos.push(tmp);
	route[tmp.x][tmp.y] = 0;
	while (!(tmp == endSuf) && !qpos.empty()){
		tmp = qpos.front();
		qpos.pop();
		x = tmp.x, y = tmp.y;
		for (int i = 0; i < 4; ++i){
			if (saferange(x + dx[i], y + dy[i], mwidth, mheight)){
				if (fragments[pos(x + dx[i], y + dy[i])].GetAttribute() == ROAD && route[x + dx[i]][y + dy[i]] > route[x][y]){
					route[x + dx[i]][y + dy[i]] = route[x][y]+1;
					qpos.push(Pos(x + dx[i], y + dy[i]));
				}
				if (fragments[pos(x + dx[i], y + dy[i])].GetAttribute() == GOAL && route[x + dx[i]][y + dy[i]] > route[x][y]){
					route[x + dx[i]][y + dy[i]] = route[x][y]+1;
				}
			}
		}	
	}
	tmp = startSuf;
	routeDir = vector<vector<int>>(mwidth,vector<int>(mheight,-1));
	while (!(tmp == endSuf)) {
		x = tmp.x, y = tmp.y;
		for (int i = 0; i < 4; ++i) {
			if (saferange(x + dx[i], y + dy[i], mwidth, mheight)) {
				int a = route[x + dx[i]][y + dy[i]], b = route[x][y];
				if (route[x + dx[i]][y + dy[i]] == route[x][y]+1 && route[x+dx[i]][y+dy[i]] != -1) {
					routeDir[x][y] = i;
					tmp = Pos(x+dx[i],y+dy[i]);
					break;
				}
			}
		}
	}
	routeDir[endSuf.x][endSuf.y] = 4;
}

vector<vector<int>>* MapViewer::GetRoute(){
	for (auto it = route.begin(); it != route.end(); ++it){
		it->resize(mheight);
		for (auto it2 = it->begin(); it2 != it->end(); ++it2){
			if (*it2 == 1000){
				*it2 = -1;
			}
		}
	}
	return &route;
}


vector<vector<int>>* MapViewer::GetRouteDir() {
	return &routeDir;
}

Pos MapViewer::FindStartPos(){
	return startPos;
}

Pos MapViewer::FindEndPos(){
	return endPos;
}

void Split(vector<string> &ds,const string& str, const string& delimiter) {
	// delimiter(2 文字以上も可) を空白に置換
	string item(str);
	for (unsigned pos = item.find(delimiter); pos != string::npos; pos = item.find(delimiter, pos)) {
		item.replace(pos, delimiter.size(), " ");
	}
	// 分解
	stringstream buf(item);

	// 読み取り
	ds.clear();
	while (buf >> item) {
		ds.push_back(item);
	}
}

string itos(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

double GetMax(deque<double> dt){
	double max = 0.0;
	for (auto n : dt){
		if (n > max){
			max = n;
		}
	}
	return max;
}

int GetMaxDDT(vector<vector<int>> ddt){
	int max = 0.0;
	for (auto n : ddt){
		for (auto n2 : n){
			if (n2 > max){
				max = n2;
			}
		}
	}
	return max;
}