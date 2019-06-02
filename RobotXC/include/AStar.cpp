#include "stdafx.h"
#include "AStar.h"
AStar::AStar(void){
	currentPoint = NULL;
}
AStar::~AStar(void){}

std::list<XCPoint> AStar::GetResultList(){
	return result;
}
void AStar::Init(XCMap* map){
	m_map = map;
	m_map->m_dilate_maze = DilateMatrix(0,m_map->m_maze);	//Ĭ�ϲ�����
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

void AStar::Calculate(bool isIgnoreCornor,bool isWithDynamicObstacle){
	result.clear();	//��ս����
	//��������open��
	XCPoint* start = new XCPoint(m_map->x_start,m_map->y_start);
	openlist.push_back(start);
	XCPoint end(m_map->x_end,m_map->y_end);
	if(m_map->m_dilate_maze[start->x][start->y] == 1 || m_map->m_dilate_dynamicObstacleLife[start->x][start->y] > 0
		|| m_map->m_dilate_maze[end.x][end.y] == 1 || m_map->m_dilate_dynamicObstacleLife[end.x][end.y]>0){
			return;	//��������յ�����ɴ��ֱ�ӷ��ؿս����ʡȥ������Ч����
	}
	do{
		if(openlist.size() == 0){
			break;
		}else{
			currentPoint = FindMinF(openlist);	//�ҵ�open����F��С�ĵ���Ϊ��ǰ��
			closelist.push_back(currentPoint);	//�ѵ�ǰ�����close��
			openlist.remove(currentPoint);		//�ѵ�ǰ�ڵ��open����ɾ��
			std::vector<XCPoint *> surroundPoints = getSurroundPoints(currentPoint,isIgnoreCornor,isWithDynamicObstacle);	//�ҵ���ǰ�ڵ���Χ�Ŀɵ���ĸ���
			for(std::vector<XCPoint *>::iterator iter = surroundPoints.begin(); iter != surroundPoints.end(); iter++){
				if(!isInList(openlist,(*iter))){
					//�������ڵ㲻��open�������open��������GHF
					openlist.push_back(*iter);
					XCPoint* p1 = isInList(openlist,*iter);
					p1->G = CalG(currentPoint, p1);
					p1->H = CalH(p1);
					p1->F = CalF(p1);
					p1->parent = currentPoint;
				}else{
					//�������ڵ���open�������G��ֵ�������ԭ���Ĵ�����Ϊ�����������丸�ڵ�Ϊ��ǰ�ڵ�
					int tempG = CalG(currentPoint, *iter);
					XCPoint* p1 = isInList(openlist,*iter);
					delete *iter;	//��getsurround����ʱnew��Pointɾ��
					if(tempG < p1->G){
						p1->parent = currentPoint;
						p1->G = tempG;
						p1->F = CalF(p1);
					}
				}
			}
		}
	}while(!isInList(openlist,&end) && !openlist.empty());
	//���յ���ݵ���㼴�ɵõ������
	XCPoint* p = NULL;
	for(PointList::iterator iter = openlist.begin(); iter != openlist.end(); iter++){
		if((*iter)->x == m_map->x_end && (*iter)->y == m_map->y_end){
			p = (*iter);
		}
	}
	if(p != NULL){
		m_Score = p->F;
	}else{
		m_Score = 10000;
	}
	while(p != NULL){
		result.push_front(*p);
		p = p->parent;
	}
	Uninit();	//�ͷ�openlist��closelist�е��ڴ�
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
std::vector<XCPoint *> AStar::getSurroundPoints(XCPoint *point,bool isIgnoreCorner,bool isWithDynamicObstacle){
	std::vector<XCPoint *> surroundPoints; 
	for(int x = point->x-1;x <= point->x+1;x++) 
		for(int y = point->y-1;y <= point->y+1;y++) 
			if(isReachable(point,new XCPoint(x,y),isIgnoreCorner,isWithDynamicObstacle))
				surroundPoints.push_back(new XCPoint(x,y)); 
	return surroundPoints;  
}
bool AStar::isReachable(XCPoint* p1, XCPoint* p2, bool isIgnoreCornor,bool isWithDynamicObstacle){
	bool flag;
	if(p2->x < 0 || p2->y < 0 || p2->x > m_map->M-1 || p2->y > m_map->N-1 || m_map->m_dilate_maze[p2->x][p2->y] == 1 ||
		isInList(closelist,p2) || p1->x == p2->x && p1->y == p2->y){
		flag = false;
	}else{
		if(isWithDynamicObstacle){
			if(m_map->m_dilate_dynamicObstacleLife[p2->x][p2->y] > 0){
				return false;
			}
		}
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
	int parentG = p1->parent == NULL ? 0 : p1->G; //����ǳ�ʼ�ڵ㣬���丸�ڵ��ǿ�
	int sum = parentG + extraG + m_map->m_weight[p2->x][p2->y];
	return sum;
}
int AStar::CalH(XCPoint* p){
	float dist = sqrt(pow((float)(p->x - m_map->x_end),2)+pow((float)(p->y - m_map->y_end),2));
	return dist * 10;
}
int AStar::GetScore(){
	return m_Score;
}