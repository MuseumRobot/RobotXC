#pragma once
#include "include/XCMap.h"
#include "include/XCPoint.h"
using namespace std;
class AStar{
public:
	AStar(void);
	~AStar(void);
private:
	PointList openlist;	//待探索列表
	PointList closelist;	//已探索列表
	XCPoint* currentPoint;
public:
	XCMap* m_map;
	std::list<XCPoint> result;		
	std::list<XCPoint> GetResultList();//返回结果序列
	void Init(XCMap* map);
	void Uninit();					//释放openlist与closelist中的内存
	void Calculate(bool isIgnoreCornor);
	std::vector<std::vector<int>> DilateMatrix(int n,std::vector<std::vector<int>> originMatrix);
private:
	XCPoint* isInList(PointList& list,XCPoint *point);	//判断list中是否含有某点
	XCPoint* FindMinF(PointList& list);
	std::vector<XCPoint *> getSurroundPoints(XCPoint *point,bool isIgnoreCorner);
	bool isReachable(XCPoint* p1, XCPoint* p2, bool isIgnoreCornor);

	int CalF(XCPoint* p);
	int CalG(XCPoint* p1, XCPoint* p2);
	int CalH(XCPoint* p);
	
};

