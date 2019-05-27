#include "stdafx.h"
#include "AStar.h"
AStar::AStar(void){
	currentPoint = NULL;
}
AStar::~AStar(void){}

std::list<Point> AStar::GetResultList(){
	return result;
}
void AStar::Init(Map* map){
	m_map = map;
}
void AStar::Calculate(bool isIgnoreCornor){
	//将起点加入open表
	Point start(m_map->x_start,m_map->y_start);
	openlist.push_back(&start);
	Point end(m_map->x_end,m_map->y_end);
	do{
		if(openlist.size() == 0){
			break;
		}else{
			currentPoint = FindMinF(openlist);	//找到open表中F最小的点置为当前点
			closelist.push_back(currentPoint);	//把当前点加入close表
			openlist.remove(currentPoint);		//把当前节点从open表中删除
			std::vector<Point *> surroundPoints = getSurroundPoints(currentPoint,isIgnoreCornor);	//找到当前节点周围的可到达的格子
			for(std::vector<Point *>::iterator iter = surroundPoints.begin(); iter != surroundPoints.end(); iter++){
				if(!isInList(openlist,(*iter))){
					//如果这个节点不在open中则加入open并更新其GHF
					openlist.push_back(*iter);
					Point* p1 = isInList(openlist,*iter);
					p1->G = CalG(currentPoint, p1);
					p1->H = CalH(p1);
					p1->F = CalF(p1);
					p1->parent = currentPoint;
				}else{
					//如果这个节点在open中则计算G的值，如果比原来的大则不作为，否则设置其父节点为当前节点
					int tempG = CalG(currentPoint, *iter);
					Point* p1 = isInList(openlist,*iter);
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
	Point* p = NULL;
	for(PointList::iterator iter = openlist.begin(); iter != openlist.end(); iter++){
		if((*iter)->x == m_map->x_end && (*iter)->y == m_map->y_end){
			p = (*iter);
		}
	}
	while(p != NULL){
		result.push_front(*p);
		p = p->parent;
	}
}
Point* AStar::isInList(PointList &plist,Point *point){
	for(std::list<Point*>::iterator iter = plist.begin(); iter != plist.end(); iter++) 
		if((*iter)->x==point->x&&(*iter)->y==point->y) 
			return *iter; 
	return NULL;  
}
Point* AStar::FindMinF(PointList& list){
	if(!openlist.empty()){
		Point* p = *(openlist.begin());
		for(PointList::iterator iter = openlist.begin(); iter != openlist.end(); iter++){
			if((*iter)->F < p->F){
				p = (*iter);
			}
		}
		return p;
	}
	return NULL;
}
std::vector<Point *> AStar::getSurroundPoints(Point *point,bool isIgnoreCorner){
	std::vector<Point *> surroundPoints; 
	for(int x = point->x-1;x <= point->x+1;x++) 
		for(int y = point->y-1;y <= point->y+1;y++) 
			if(isReachable(point,new Point(x,y),isIgnoreCorner)) 
				surroundPoints.push_back(new Point(x,y)); 
	return surroundPoints;  
}
bool AStar::isReachable(Point* p1, Point* p2, bool isIgnoreCornor){
	bool flag;
	if(p2->x < 0 || p2->y < 0 || p2->x > m_map->M-1 || p2->y > m_map->N-1 || m_map->m_maze[p2->x][p2->y] == 1 ||
		isInList(closelist,p2) || p1->x == p2->x && p1->y == p2->y){
		flag = false;
	}else{
		if(abs(p1->x - p2->x) + abs(p1->y - p2->y) == 1){
			flag = true;
		}else{
			if(m_map->m_maze[p2->x][p1->y] == 0 && m_map->m_maze[p1->x][p2->y] == 0){
				flag = true;
			}else{
				flag = isIgnoreCornor;
			}
		}
	}
	return flag;
}
int AStar::CalF(Point* p){
	return p->G + p->H;
}
int AStar::CalG(Point* p1, Point * p2){
	int cost1 = 10;
	int cost2 = 14;
	int extraG = (abs(p2->x - p1->x)+abs(p2->y - p1->y)) == 1 ? cost1 : cost2; 
	int parentG = p1->parent == NULL ? 0 : p1->G; //如果是初始节点，则其父节点是空
	int sum = parentG + extraG + m_map->m_weight[p2->x][p2->y];
	return sum;
}
int AStar::CalH(Point* p){
	float dist = sqrt(pow((float)(p->x - m_map->x_end),2)+pow((float)(p->y - m_map->y_end),2));
	return dist * 10;
}