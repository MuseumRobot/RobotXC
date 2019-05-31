#pragma once
#include "include/XCMap.h"
#include "include/XCPoint.h"
using namespace std;
class AStar{
public:
	AStar(void);
	~AStar(void);
private:
	PointList openlist;	//��̽���б�
	PointList closelist;	//��̽���б�
	XCPoint* currentPoint;
	int m_Score;		//�յ��Fֵ
public:
	XCMap* m_map;
	std::list<XCPoint> result;		
	std::list<XCPoint> GetResultList();//���ؽ������
	int GetScore();
	void Init(XCMap* map);
	void Uninit();					//�ͷ�openlist��closelist�е��ڴ�
	void Calculate(bool isIgnoreCornor,bool isWithDynamicObstacle);
	std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);
private:
	XCPoint* isInList(PointList& list,XCPoint *point);	//�ж�list���Ƿ���ĳ��
	XCPoint* FindMinF(PointList& list);
	std::vector<XCPoint *> getSurroundPoints(XCPoint *point,bool isIgnoreCorner,bool isWithDynamicObstacle);
	bool isReachable(XCPoint* p1, XCPoint* p2, bool isIgnoreCornor,bool isWithDynamicObstacle);

	int CalF(XCPoint* p);
	int CalG(XCPoint* p1, XCPoint* p2);
	int CalH(XCPoint* p);
	
};

