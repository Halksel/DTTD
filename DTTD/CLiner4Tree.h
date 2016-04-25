#pragma once
// 線形4分木衝突判定サンプルプログラム
//  製作者 : IKD
//   ○×つくろ～どっとコム（http://www.h6.dion.ne.jp/~game296o/）
/*#include "Object.h"

using namespace std;

template <class T>
class CCell;

template< class T>
class Object_For_Tree {
public:
	shared_ptr<CCell<T>> m_pCell;                // 登録空間
	shared_ptr<T> m_pObject;                     // 判定対象オブジェクト
	weak_ptr <Object_For_Tree<T> > m_spPre;  // 前のOBJECT_FOR_TREE構造体
	weak_ptr<Object_For_Tree<T> > m_spNext; // 次のOBJECT_FOR_TREE構造体

	int m_id;
	Object_For_Tree() {};
	Object_For_Tree(int id) {
		m_id = id;
		m_pCell.reset();
		m_pObject.reset();
		m_spPre.reset();
		m_spNext.reset();
	}
	bool Remove();
	shared_ptr<Object_For_Tree<T>> GetMine() {
		return shared_ptr<Object_For_Tree<T>>(new Object_For_Tree<T>(*this));
	}
};

// 自らリストから外れる
//ok
template<class T>
bool Object_For_Tree<T>::Remove() {

	// すでに逸脱している時は処理終了
	if (!m_pCell)
		return false;

	// 自分を登録している空間に自身を通知
	if (!m_pCell->OnRemove(shared_ptr<Object_For_Tree<T>>( this))) {
		return false;
	}

	// 逸脱処理
	// 前後のオブジェクトを結びつける
	if (m_spPre.lock())
	{
		m_spPre.lock().get()->m_spNext.lock().reset(new Object_For_Tree<T>( *m_spNext.lock()->GetMine()));
	}
	if (m_spNext.lock())
	{
		m_spNext.lock().get()->m_spPre.lock().reset(new Object_For_Tree<T>( *m_spPre.lock()->GetMine()));
	}

	m_spPre.reset();
	m_spNext.reset();

	m_pCell.reset();
	return true;
}

template<class T>
class CCell {
protected:
	weak_ptr<Object_For_Tree<T>> m_spLatest;
public:
	// コンストラクタ
	CCell()
	{
		m_spLatest = shared_ptr<Object_For_Tree<T>>(nullptr);
	}
	// デストラクタ
	virtual ~CCell()
	{
		if (m_spLatest.lock() != NULL) {
			//ResetLink( m_spLatest );
		}
	}
	void ResetLink(shared_ptr<Object_For_Tree<T>> &);
	bool Push(shared_ptr<Object_For_Tree<T>> &);
	bool OnRemove(shared_ptr<Object_For_Tree<T>> &);
	weak_ptr<Object_For_Tree<T>> GetFirstObj();
};

// 削除されるオブジェクトをチェック
//ok
template<class T>
bool CCell<T>::OnRemove(shared_ptr<Object_For_Tree<T>> &pRemoveObj) {
	auto p = shared_ptr<Object_For_Tree<T>>(m_spLatest.lock());
	if (p == pRemoveObj) {
		// 次のオブジェクトに挿げ替え
		if (m_spLatest.lock() != nullptr) {
			m_spLatest = shared_ptr<Object_For_Tree<T>>( new Object_For_Tree<T>(*m_spLatest.lock()->m_spNext.lock().get()));
		}
	}
	return true;
}

/////////////////////////////////////
// 衝突リスト
//////////////////////////////////
const int COLLISIONLIST_REALLOCSIZE = 50;
template < class T >
class CollisionList {
private:
	T**		root_;		// リストルートポインタ
	size_t	pos_;
	size_t	mallocSize_;
public:
	CollisionList() : root_( 0 ), pos_( 0 ), mallocSize_( 0 ) {
		root_ = (T**)malloc( 0 );
	}
	~CollisionList() {
		reflesh();
	}

	size_t getSize() {
		return pos_;
	}

	T** getRootPtr() {
		return root_;
	}

	void resetPos() {
		pos_ = 0;
	}

	void wright( T* obj1, T*obj2 ) {
		if ( pos_ >= mallocSize_ ) {
			root_ = (T**)realloc( root_, sizeof (T*) * ( mallocSize_ + COLLISIONLIST_REALLOCSIZE ) );
			mallocSize_ += COLLISIONLIST_REALLOCSIZE;
		}
		root_[ pos_++ ] = obj1;
		root_[ pos_++ ] = obj2;
	}

	void reflesh() {
		if ( root_ ) {
			free( root_ );
		}
	}

};
template<class T>
class CLiner4TreeManager {
	static const int CLINER4TREEMANAGER_MAXLEVEL = 9;
	int m_iPow[CLINER4TREEMANAGER_MAXLEVEL+1];
	int m_dwCellNum;
	unsigned int m_uiLevel,m_uiDim;			// 最下位レベル
	double m_dLeft, m_dTop, m_dW, m_dH, m_dUnit_W, m_dUnit_H;
	shared_ptr<CCell<T>> ppCellAry;
	CollisionList< T > m_ColList;	// 衝突リスト
protected:
	bool GetCollisionList(unsigned long Elem, list<shared_ptr<T>> &);
public:
	CLiner4TreeManager() {
		m_uiLevel = 0;
		m_dW = 0.0f;
		m_dH = 0.0f;
		m_dLeft = 0.0f;
		m_dTop = 0.0f;
		m_dUnit_W = 0.0f;
		m_dUnit_H = 0.0f;
		m_dwCellNum = 0;
		ppCellAry.reset();
		m_uiDim = 0;

		// 各レベルでの空間数を算出
		int i;
		m_iPow[0] = 1;
		for(i=1;i<CLINER4TREEMANAGER_MAXLEVEL+1;i++)
			m_iPow[i] = m_iPow[i-1]*4;
	}
	// デストラクタ
	virtual ~CLiner4TreeManager()
	{
		ppCellAry.reset();
	}
	bool Init(unsigned int, double, double, double, double);
	bool Regist(double, double, double, double,std::shared_ptr<Object_For_Tree<T>> &);
	bool CreateNewCell(unsigned long);
	unsigned long GetAllCollisionList(CollisionList<T>** );
	unsigned long GetPointElem(double,double);
	unsigned long BitSeparate32(unsigned long );
	unsigned long GetMortonNumber(double left, double top, double right, double bottom);
	unsigned long Get2DMortonNumber(unsigned short x, unsigned short y);
};

// リンクを全てリセットする
template<class T>
void CCell<T>::ResetLink(shared_ptr< Object_For_Tree<T> > &spOFT){
	
	if (spOFT->m_spNext != NULL) {
		ResetLink(spOFT->m_spNext);
	}
	spOFT.reset();
}

//ok
template<class T>
bool CCell<T>::Push(shared_ptr<Object_For_Tree<T>>& upOFT) {
	if (!upOFT) return false; // 無効オブジェクトは登録しない
	if (upOFT->m_pCell.get() == this) return false; // 2重登録チェック
	if (!m_spLatest.lock())
	{
		m_spLatest = shared_ptr<Object_For_Tree<T>>( new Object_For_Tree<T>(*upOFT)); // 空間に新規登録
	}
	else
	{
		// 最新OFTオブジェクトを更新
		upOFT->m_spNext.lock().reset(new Object_For_Tree<T>(*m_spLatest.lock()));
		m_spLatest.lock()->m_spPre.lock().reset(new Object_For_Tree<T>(*upOFT));
		m_spLatest = shared_ptr<Object_For_Tree<T>>( new Object_For_Tree<T>(*upOFT));
	}
	upOFT->m_pCell = shared_ptr<CCell<T>>(this); // 空間を登録
	return true;
}

template<class T>
weak_ptr<Object_For_Tree<T>> CCell<T>::GetFirstObj() {
	return m_spLatest;
}

// 線形4分木配列を構築する
//NG
template<class T>
bool CLiner4TreeManager<T>::Init(unsigned int Level, double left, double top, double right, double bottom)
{
	// 設定最高レベル以上の空間は作れない
	
	if (Level >= CLINER4TREEMANAGER_MAXLEVEL )
		return false;

	// 各レベルでの空間数を算出
	m_iPow[0] = 1;
	for (int i = 1; i < CLINER4TREEMANAGER_MAXLEVEL + 1; i++)
		m_iPow[i] = m_iPow[i - 1] * 4;

	// Levelレベル（0基点）の配列作成
	m_dwCellNum = (m_iPow[Level + 1] - 1) / 3;

	//ppCellAry = new CCell<T>*[m_dwCellNum];
	ppCellAry = shared_ptr<CCell<T>>(new CCell<T>[m_dwCellNum],std::default_delete<CCell<T>[]>());
	//ZeroMemory( ppCellAry, sizeof(CCell<T>*)*m_dwCellNum );

	// 有効領域を登録
	m_dLeft = left;
	m_dTop = top;
	m_dW = right - left;
	m_dH = bottom - top;
	m_dUnit_W = m_dW / (1 << Level);
	m_dUnit_H = m_dH / (1 << Level);

	m_uiLevel = Level;
	auto p_OFT = shared_ptr<Object_For_Tree<T>>( new Object_For_Tree<T>());
	Regist(0, 0, 0, 0,p_OFT);
	p_OFT.reset();
	return true;
}
// 座標→線形4分木要素番号変換関数
template<class T>
unsigned long CLiner4TreeManager<T>::GetPointElem(double pos_x, double pos_y)
{
	return Get2DMortonNumber((unsigned short)((pos_x - m_dLeft) / m_dUnit_W), (unsigned short)((pos_y - m_dTop) / m_dUnit_H));
}
template<class T>
unsigned long CLiner4TreeManager<T>::BitSeparate32(unsigned long n) {
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

template<class T>
unsigned long CLiner4TreeManager<T>::Get2DMortonNumber(unsigned short x, unsigned short y) {
	return (BitSeparate32(x) | (BitSeparate32(y) << 1));
}

template<class T>
unsigned long CLiner4TreeManager<T>::GetMortonNumber(double left, double top, double right, double bottom)
{
	// 最小レベルにおける各軸位置を算出
	unsigned long LT = GetPointElem(left, top);
	unsigned long RB = GetPointElem(right, bottom);

	// 空間番号の排他的論理和から
	// 所属レベルを算出
	unsigned long Def = RB ^ LT;
	unsigned int HiLevel = 0;
	unsigned int i;
	for (i = 0; i < m_uiLevel; i++)
	{
		unsigned long Check = (Def >> (i * 2)) & 0x3;
		if (Check != 0)
			HiLevel = i + 1;
	}
	unsigned long SpaceNum = RB >> (HiLevel * 2);
	unsigned long AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 3;
	SpaceNum += AddNum;

	if (SpaceNum > m_dwCellNum)
		return 0xffffffff;

	return SpaceNum;
}
// 空間を生成
template<class T>
bool CLiner4TreeManager<T>::CreateNewCell(unsigned long Elem)
{
	// 引数の要素番号
	while (!ppCellAry.get()[Elem].GetFirstObj().expired())
	{
		// 指定の要素番号に空間を新規作成
		ppCellAry.get()[Elem] = CCell<T>() ;

		// 親空間にジャンプ
		Elem = (Elem - 1) >> 2;
		if (Elem >= m_dwCellNum) break;
	}
	return true;
}

// 衝突判定リストを作成する
template<class T>
unsigned long CLiner4TreeManager<T>::GetAllCollisionList(CollisionList<T>** colList)
{
	// リスト（配列）は必ず初期化します
	m_ColList.resetPos();

	// ルート空間の存在をチェック
	if (ppCellAry.get()[0].GetFirstObj().expired())
		return 0;	// 空間が存在していない

	// ルート空間を処理
	list<shared_ptr<T>> ColStac;
	GetCollisionList(0, ColStac);

	*colList = &m_ColList;

	return (unsigned long)m_ColList.getSize();
}

// 空間内で衝突リストを作成する
template<class T>
bool CLiner4TreeManager<T>::GetCollisionList(unsigned long Elem, list<shared_ptr<T>> &ColStac)
{
	// ① 空間内のオブジェクト同士の衝突リスト作成
	shared_ptr<Object_For_Tree<T> > spOFT1 = shared_ptr<Object_For_Tree<T>>(ppCellAry.get()[Elem].GetFirstObj().lock());
	while (spOFT1 != NULL)
	{
		shared_ptr<Object_For_Tree<T> > spOFT2 = spOFT1->m_spNext.lock();
		while (spOFT2) {
			// 衝突リスト作成
			m_ColList.wright(spOFT1->m_pObject.get(), spOFT2->m_pObject.get());
			spOFT2 = spOFT2->m_spNext.lock();
		}
		// ② 衝突スタックとの衝突リスト作成
		for (auto it = ColStac.begin(); it != ColStac.end(); it++) {
			m_ColList.wright(spOFT1->m_pObject.get(), it->get());
		}
		spOFT1 = spOFT1->m_spNext.lock();
	}

	bool ChildFlag = false;
	// ③ 子空間に移動
	unsigned long ObjNum = 0;
	unsigned long NextElem;
	for (unsigned long i = 0; i < 4; i++) {
		NextElem = Elem * 4 + 1 + i;
		if (NextElem < m_dwCellNum && ppCellAry.get()[Elem * 4 + 1 + i].GetFirstObj().expired()) {
			if (!ChildFlag) {
				// ④ 登録オブジェクトをスタックに追加
				spOFT1 = shared_ptr<Object_For_Tree<T>>(ppCellAry.get()[Elem].GetFirstObj().lock());
				while (spOFT1 != NULL) {
					ColStac.push_back(spOFT1->m_pObject);
					ObjNum++;
					spOFT1 = spOFT1->m_spNext.lock();
				}
			}
			ChildFlag = true;
			GetCollisionList(Elem * 4 + 1 + i, ColStac); // 子空間へ
		}
	}

	// ⑤ スタックからオブジェクトを外す
	if (ChildFlag) {
		for (unsigned int i = 0; i < ObjNum; i++)
			ColStac.pop_back();
	}

	return true;
};
// オブジェクトを登録する
template<class T>
bool CLiner4TreeManager<T>::Regist(double left, double top, double right, double bottom,std::shared_ptr<Object_For_Tree<T>> &upOFT) {
	// オブジェクトの境界範囲から登録モートン番号を算出
	unsigned long Elem = GetMortonNumber(left, top, right, bottom);
	if (Elem < m_dwCellNum) {
		// 空間が無い場合は新規作成
		if (!ppCellAry.get()[Elem].GetFirstObj().expired())
			CreateNewCell(Elem);
		bool f =ppCellAry.get()[Elem].Push(make_shared<Object_For_Tree<T>>(*upOFT));

		return f;
	}
	return false; // 登録失敗
}
*/
