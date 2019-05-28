#pragma once
#include <fstream>
#include <QWidget>
using namespace std;
class XCConfig:public QWidget{
	Q_OBJECT
public:
	XCConfig(void);
	~XCConfig(void);
private:
	int comm_motor_;			//������ں�
	int comm_star_;				//�Ǳ괮�ں�
	int comm_laser_;			//���⴮�ں�
	int gui_mode_;				//ͼ�λ�����������ʾģʽ(0Ϊ������ģʽ��1Ϊ�û�ģʽ)
	int instruction_cycle_;		//ָ������(ms)
	int data_refresh_cycle_;	//����ˢ������(ms)
	int obstacle_threshold_;	//ͨ����ֵ(mm)(����þ�����û��̽�⵽��������Ϊǰ������)
	int far_obs_threshold_;		//ͨ����ֵ(mm)(����þ�����û��̽�⵽��������Ϊǰ��ͨ�������ڸ�����ʻ)
	int error_angle_;			//�Ƕ����(��)
	int error_distance_;		//�������(mm)
	int offset_correction_;		//ƫ��������(mm)(�Ǳ����ĵ��������ĵ�����λ��)
	int map_scale_;				//��ͼ��ʾ�ߴ�(�������û��Ƶ�ͼʱһ����Ԫ��ı߳�����λΪ����)
	int architect_scale_;		//��ͼ������(һ��߳���������ʵ�������cm)
	void init();
	string trim(const string& str);
	vector<string> split(string str,string pattern);
	bool LoadFromFile(const string &config_file);
public:
	int comm_motor();			//��ȡ������ں�
	int comm_star();			//��ȡ�Ǳ괮�ں�
	int comm_laser();			//��ȡ���⴮�ں�
	int gui_mode();				//��ȡͼ�λ�����������ʾģʽ	
	int instruction_cycle();	//��ȡָ������(ms)
	int data_refresh_cycle();	//��ȡ����ˢ������(ms)
	int obstacle_threshold();	//��ȡͨ����ֵ(mm)(����þ�����û��̽�⵽��������Ϊǰ������)
	int far_obs_threshold();	//��ȡͨ����ֵ(mm)(����þ�����û��̽�⵽��������Ϊǰ��ͨ�������ڸ�����ʻ)
	int error_angle();			//��ȡ�Ƕ����(��)
	int error_distance();		//��ȡ�������(mm)
	int offset_correction();	//��ȡƫ��������(mm)(�Ǳ����ĵ��������ĵ�����λ��)
	int map_scale();			//��ȡ��ͼ��ʾ�ߴ�(�������û��Ƶ�ͼʱһ����Ԫ��ı߳�����λΪ����)
	int architect_scale();		//��ȡ��ͼ������(һ��߳���������ʵ�������cm)
public slots:
	void map_scale_add();
	void map_scale_diminish();
};

