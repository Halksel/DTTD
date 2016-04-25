#pragma once

#include "DxLib.h"
#include "Input.h"

#define pos(i,j) ((i) + mwidth*(j))
#define saferange(x,y,w,h) (0 <= x && x < w && 0 <= y && y < h)

using namespace std;

//定数式置き場所
static const string empty_string;
const unsigned int red = GetColor(0, 0, 255);
const unsigned int green = GetColor(0, 200, 0);
const unsigned int blue = GetColor(255, 0, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int black = GetColor(0, 0, 0);
const unsigned int StringColor = black;
const int X = 0, Y = 40;//マップの原点
const string PATH = "Data/Map/SaveData";
const int dx[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };//右、左、下、上
const int dy[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
const double expWRate = 0.5, expHRate = 0.5;

//グローバル変数	

//以下関数
void Split(vector<string> &, const string&, const string&);
string itos(int);
double GetMax(deque<double>);
int GetMaxDDT(vector<vector<int>>);

//以下クラス
class MapFragment{
private:
	string filePath;
	int drawH,x,y,width,height,attribute;
public:
	MapFragment();
	MapFragment(int,int,int,string);
	void Update();
	void Draw();
	void DrawKind();
	bool LoadFail();
	bool InMouseClick();
	int GetAttribute();
	Pos GetPos();
};

class MapViewer{
private:
	string filePath;
	int handle,width, height, mwidth, mheight, imagenum, select, attribute,mapW,mapH;
	bool fail,atbflag;
	Pos startPos, endPos,startSuf,endSuf,selectPos;
	vector<int> drawHs;
	vector<vector<int>> route,routeDir;
	vector< string > ds,filepaths;
	vector<MapFragment> fragments;
public:
	MapViewer();
	MapViewer(int);
	void Update();
	void Draw();
	void DrawInfo();
	void DrawRoute();
	void SetData(int);
	bool Fail();
	int GetSelect();
	int GetSelectAttribute();
	Pos GetSelectPos();
	void SetStoERoute();
	vector<vector<int>>* GetRoute();
	vector<vector<int>>* GetRouteDir();
	Pos FindStartPos();
	Pos FindEndPos();
};
