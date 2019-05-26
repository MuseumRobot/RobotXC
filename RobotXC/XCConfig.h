#pragma once
#include <fstream>
using namespace std;
class XCConfig{
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
	void init();
	string trim(const string& str);
	vector<string> split(string str,string pattern);
	bool LoadFromFile(const string &config_file);
public:
	int comm_motor();
	int comm_star();
	int comm_laser();
	int gui_mode();
	int instruction_cycle();
	int data_refresh_cycle();
	int obstacle_threshold();
	int far_obs_threshold();
	int error_angle();
	int error_distance();
	int offset_correction();
};

