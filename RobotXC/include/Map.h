#pragma once
#include <vector>
using namespace std;
class Map{
public:
	int M,N;	//地图由M行N列单元格构成
	int x_start,y_start,x_end,y_end;			//定义起点和终点的坐标
	std::vector<std::vector<int>> m_maze;		//二维数组存储障碍信息
	std::vector<std::vector<int>> m_weight;		//二维数组存储地格权值
	Map(int _M,int _N):M(_M),N(_N){
		x_start=x_end=y_start=y_end=-1;
		//初始化时即设定二位向量规模提高程序运行效率
		m_maze.resize(M);
		m_weight.resize(M);
		for(int i=0;i<M;i++){
			m_maze[i].resize(N);
			m_weight[i].resize(N);
		}
		for(int i=0;i<M;i++){
			for(int j=0;j<N;j++){
				m_maze[i][j]=0;
				m_weight[i][j]=0;
			}
		}

	}
	~Map(){}
};