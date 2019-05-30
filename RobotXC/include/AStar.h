#pragma once
#include "include/Map.h"
#include "include/Point.h"
using namespace std;
class AStar{
public:
	AStar(void);
	~AStar(void);
private:
	PointList openlist;	//��̽���б�
	PointList closelist;	//��̽���б�
	Point* currentPoint;
public:
	Map* m_map;
	std::list<Point> result;		
	std::list<Point> GetResultList();//���ؽ������
	void Init(Map* map);
	void Uninit();					//�ͷ�openlist��closelist�е��ڴ�
	void Calculate(bool isIgnoreCornor);
	std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);
private:
	Point* isInList(PointList& list,Point *point);	//�ж�list���Ƿ���ĳ��
	Point* FindMinF(PointList& list);
	std::vector<Point *> getSurroundPoints(Point *point,bool isIgnoreCorner);
	bool isReachable(Point* p1, Point* p2, bool isIgnoreCornor);

	int CalF(Point* p);
	int CalG(Point* p1, Point* p2);
	int CalH(Point* p);
	
};

