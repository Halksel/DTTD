#include"CLiner4Tree.h"

using namespace std;
/*
template< class T>
class Object_For_Tree {
public:
	shared_ptr<CCell<T>> m_pCell;                // �o�^���
	shared_ptr<T> m_pObject;                     // ����ΏۃI�u�W�F�N�g
	weak_ptr <Object_For_Tree<T> > m_spPre;  // �O��OBJECT_FOR_TREE�\����
	weak_ptr<Object_For_Tree<T> > m_spNext; // ����OBJECT_FOR_TREE�\����

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
	shared_ptr<Object_For_Tree<T>> GetMine();
};

template<class T>
class CCell {
protected:
	weak_ptr<Object_For_Tree<T>> m_spLatest;
public:
	// �R���X�g���N�^
	CCell()
	{
		m_spLatest = shared_ptr<Object_For_Tree<T>>(nullptr);
	}
	// �f�X�g���N�^
	virtual ~CCell()
	{
		if (m_spLatest.lock() != NULL) {
			//ResetLink( m_spLatest );
		}
	}
	void ResetLink(shared_ptr<Object_For_Tree<T>>);
	bool Push(shared_ptr<Object_For_Tree<T>> );
	bool OnRemove(shared_ptr<Object_For_Tree<T>> );
	weak_ptr<Object_For_Tree<T>> GetFirstObj();
};

template<class T>
class CLiner4TreeManager {
	static const int CLINER4TREEMANAGER_MAXLEVEL = 9;
	int m_iPow[CLINER4TREEMANAGER_MAXLEVEL+1];
	int m_dwCellNum;
	unsigned int m_uiLevel,m_uiDim;			// �ŉ��ʃ��x��
	double m_dLeft, m_dTop, m_dW, m_dH, m_dUnit_W, m_dUnit_H;
	shared_ptr<CCell<T>> ppCellAry;
	CollisionList< T > m_ColList;	// �Փ˃��X�g
protected:
	bool GetCollisionList(unsigned long Elem, list<shared_ptr<T>> &);
public:
	CLiner4TreeManager() {};
	// �f�X�g���N�^
	virtual ~CLiner4TreeManager()
	{
		ppCellAry.reset();
	}
	bool Init(unsigned int, double, double, double, double);
	bool Regist(double, double, double, double,std::shared_ptr<Object_For_Tree<T>> );
	bool CreateNewCell(unsigned long);
	unsigned long GetAllCollisionList(CollisionList<T>** );
	unsigned long GetPointElem(double,double);
	unsigned long BitSeparate32(unsigned long );
	unsigned long GetMortonNumber(double left, double top, double right, double bottom);
	unsigned long Get2DMortonNumber(unsigned short x, unsigned short y);
};
*/