#pragma once
#include <vector>
using namespace std;
class Map{
public:
	int M,N;	//��ͼ��M��N�е�Ԫ�񹹳�
	int x_start,y_start,x_end,y_end;			//���������յ������
	std::vector<std::vector<int>> m_maze;		//��ά����洢�ϰ���Ϣ
	std::vector<std::vector<int>> m_weight;		//��ά����洢�ظ�Ȩֵ
	Map(int _M,int _N):M(_M),N(_N){
		x_start=x_end=y_start=y_end=-1;
		//��ʼ��ʱ���趨��λ������ģ��߳�������Ч��
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