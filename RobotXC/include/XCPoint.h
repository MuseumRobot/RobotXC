#pragma once
#include <list>
using namespace std;
class XCPoint{
public:
	int x,y;		//µ±Ç°×ø±ê
	int F,G,H;		
	XCPoint* parent;	

public:
	XCPoint():x(0),y(0),F(0),G(0),H(0),parent(NULL){}
	XCPoint(int _x,int _y):x(_x),y(_y),F(0),G(0),H(0),parent(NULL){}

};
typedef list<XCPoint*> PointList;