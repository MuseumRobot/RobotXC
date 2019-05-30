#pragma once
#include "include/Map.h"
#include "include/Point.h"
using namespace std;
class AStar{
public:
	AStar(void);
	~AStar(void);
private:
	PointList openlist;	//待探索列表
	PointList closelist;	//已探索列表
	Point* currentPoint;
public:
	Map* m_map;
	std::list<Point> result;		
	std::list<Point> GetResultList();//返回结果序列
	void Init(Map* map);
	void Uninit();					//释放openlist与closelist中的内存
	void Calculate(bool isIgnoreCornor);
	std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);
private:
	Point* isInList(PointList& list,Point *point);	//判断list中是否含有某点
	Point* FindMinF(PointList& list);
	std::vector<Point *> getSurroundPoints(Point *point,bool isIgnoreCorner);
	bool isReachable(Point* p1, Point* p2, bool isIgnoreCornor);

	int CalF(Point* p);
	int CalG(Point* p1, Point* p2);
	int CalH(Point* p);
	
};

