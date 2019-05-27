#pragma once
#include "include/Map.h"
#include "include/Point.h"
using namespace std;
class AStar{
public:
	AStar(void);
	~AStar(void);
private:
	Map* m_map;
	PointList openlist;	//待探索列表
	PointList closelist;	//已探索列表
	Point* currentPoint;
public:
	std::list<Point> result;		
	std::list<Point> GetResultList();//返回结果序列
	void Init(Map* map);
	void Calculate(bool isIgnoreCornor);
private:
	Point* isInList(PointList& list,Point *point);	//判断list中是否含有某点
	Point* FindMinF(PointList& list);
	std::vector<Point *> getSurroundPoints(Point *point,bool isIgnoreCorner);
	bool isReachable(Point* p1, Point* p2, bool isIgnoreCornor);

	int CalF(Point* p);
	int CalG(Point* p1, Point* p2);
	int CalH(Point* p);
};

