#include "stdafx.h"
#include "AStar.h"
AStar::AStar(void){
	currentPoint = NULL;
}
AStar::~AStar(void){
	if(currentPoint!=NULL){
		delete currentPoint;
	}
}

std::list<XCPoint> AStar::GetResultList(){
	return result;
}
void AStar::Init(XCMap* map){
	m_map = map;
	m_map->m_dilate_maze = DilateMatrix(0,m_map->m_maze);	//默认不膨胀
	m_map->m_dilate_dynamicObstacleLife = DilateMatrix(0,m_map->m_dynamicObstacleLife);
}
void AStar::Uninit(){
	for(PointList::iterator iter = openlist.begin();iter != openlist.end();iter++){
		delete *iter;
		*iter = NULL;
	}
	openlist.clear();
	for(PointList::iterator iter = closelist.begin();iter != closelist.end();iter++){
		if(*iter!=NULL) 	delete *iter;
	}
	closelist.clear();
}
void AStar::Calculate(bool isIgnoreCornor){
	//将起点加入open表
	XCPoint* start = new XCPoint(m_map->x_start,m_map->y_start);
	openlist.push_back(start);
	XCPoint end(m_map->x_end,m_map->y_end);
	do{
		if(openlist.size() == 0){
			break;
		}else{
			currentPoint = FindMinF(openlist);	//找到open表中F最小的点置为当前点
			closelist.push_back(currentPoint);	//把当前点加入close表
			openlist.remove(currentPoint);		//把当前节点从open表中删除
			std::vector<XCPoint *> surroundPoints = getSurroundPoints(currentPoint,isIgnoreCornor);	//找到当前节点周围的可到达的格子
			for(std::vector<XCPoint *>::iterator iter = surroundPoints.begin(); iter != surroundPoints.end(); iter++){
				if(!isInList(openlist,(*iter))){
					//如果这个节点不在open中则加入open并更新其GHF
					openlist.push_back(*iter);
					XCPoint* p1 = isInList(openlist,*iter);
					p1->G = CalG(currentPoint, p1);
					p1->H = CalH(p1);
					p1->F = CalF(p1);
					p1->parent = currentPoint;
				}else{
					//如果这个节点在open中则计算G的值，如果比原来的大则不作为，否则设置其父节点为当前节点
					int tempG = CalG(currentPoint, *iter);
					XCPoint* p1 = isInList(openlist,*iter);
					delete *iter;	//将getsurround中临时new的Point删除
					if(tempG < p1->G){
						p1->parent = currentPoint;
						p1->G = tempG;
						p1->F = CalF(p1);
					}
				}
			}
		}
	}while(!isInList(openlist,&end) && !openlist.empty());
	//从终点回溯到起点即可得到结果表
	XCPoint* p = NULL;
	for(PointList::iterator iter = openlist.begin(); iter != openlist.end(); iter++){
		if((*iter)->x == m_map->x_end && (*iter)->y == m_map->y_end){
			p = (*iter);
		}
	}
	while(p != NULL){
		result.push_front(*p);
		p = p->parent;
	}
	Uninit();	//释放openlist与closelist中的内存
}
XCPoint* AStar::isInList(PointList &plist,XCPoint *point){
	for(std::list<XCPoint*>::iterator iter = plist.begin(); iter != plist.end(); iter++) 
		if((*iter)->x==point->x&&(*iter)->y==point->y) 
			return *iter; 
	return NULL;  
}
XCPoint* AStar::FindMinF(PointList& list){
	if(!openlist.empty()){
		XCPoint* p = *(openlist.begin());
		for(PointList::iterator iter = openlist.begin(); iter != openlist.end(); iter++){
			if((*iter)->F < p->F){
				p = (*iter);
			}
		}
		return p;
	}
	return NULL;
}
std::vector<XCPoint *> AStar::getSurroundPoints(XCPoint *point,bool isIgnoreCorner){
	std::vector<XCPoint *> surroundPoints; 
	for(int x = point->x-1;x <= point->x+1;x++) 
		for(int y = point->y-1;y <= point->y+1;y++) 
			if(isReachable(point,new XCPoint(x,y),isIgnoreCorner)) 
				surroundPoints.push_back(new XCPoint(x,y)); 
	return surroundPoints;  
}
bool AStar::isReachable(XCPoint* p1, XCPoint* p2, bool isIgnoreCornor){
	bool flag;
	if(p2->x < 0 || p2->y < 0 || p2->x > m_map->M-1 || p2->y > m_map->N-1 || m_map->m_dilate_maze[p2->x][p2->y] == 1 ||
		isInList(closelist,p2) || p1->x == p2->x && p1->y == p2->y || m_map->m_dilate_dynamicObstacleLife[p2->x][p2->y] > 0){
		flag = false;
	}else{
		if(abs(p1->x - p2->x) + abs(p1->y - p2->y) == 1){
			flag = true;
		}else{
			if(m_map->m_dilate_maze[p2->x][p1->y] == 0 && m_map->m_dilate_maze[p1->x][p2->y] == 0 && m_map->m_dilate_dynamicObstacleLife[p2->x][p1->y] == 0 && m_map->m_dilate_dynamicObstacleLife[p1->x][p2->y] == 0){
				flag = true;
			}else{
				flag = isIgnoreCornor;
			}
		}
	}
	delete p2;
	return flag;
}
int AStar::CalF(XCPoint* p){
	return p->G + p->H;
}
int AStar::CalG(XCPoint* p1, XCPoint * p2){
	int cost1 = 10;
	int cost2 = 14;
	int extraG = (abs(p2->x - p1->x)+abs(p2->y - p1->y)) == 1 ? cost1 : cost2; 
	int parentG = p1->parent == NULL ? 0 : p1->G; //如果是初始节点，则其父节点是空
	int sum = parentG + extraG + m_map->m_weight[p2->x][p2->y];
	return sum;
}
int AStar::CalH(XCPoint* p){
	float dist = sqrt(pow((float)(p->x - m_map->x_end),2)+pow((float)(p->y - m_map->y_end),2));
	return dist * 10;
}
std::vector<std::vector<int>> AStar::DilateMatrix(int n,std::vector<std::vector<int>> originMatrix){
	std::vector<std::vector<int>> dilate_maze;
	dilate_maze.resize(m_map->M);
	for(int i=0;i<m_map->M;i++){
		dilate_maze[i].resize(m_map->N);
	}
	if(n == 0){
		return originMatrix;
	}else if(n > 0){
		std::vector<std::vector<int>> pre_dilate_maze =  DilateMatrix(n-1,originMatrix);
		for(int i=0;i<m_map->M;i++){
			for(int j=0;j<m_map->N;j++){
				if(pre_dilate_maze[i][j]>0){
					for(int x=i-1;x<i+2;x++){
						for(int y=j-1;y<j+2;y++){
							if(x>-1 && y>-1 && x<m_map->M && y<m_map->N){
								if(dilate_maze[x][y] == 0){
									dilate_maze[x][y] = pre_dilate_maze[i][j];
								}
							}
						}
					}
				}
			}
		}
	}
	return dilate_maze;
}