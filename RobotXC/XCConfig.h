#pragma once
#include <fstream>
using namespace std;
class XCConfig{
public:
	XCConfig(void);
	~XCConfig(void);
private:
	int comm_motor_;			//电机串口号
	int comm_star_;				//星标串口号
	int comm_laser_;			//激光串口号
	int gui_mode_;				//图形化交互界面显示模式(0为开发者模式，1为用户模式)
	int instruction_cycle_;		//指令周期(ms)
	int data_refresh_cycle_;	//数据刷新周期(ms)
	int obstacle_threshold_;	//通行阈值(mm)(如果该距离内没有探测到物体则认为前方可行)
	int far_obs_threshold_;		//通畅阈值(mm)(如果该距离内没有探测到物体则认为前方通畅，用于高速行驶)
	int error_angle_;			//角度误差(°)
	int error_distance_;		//距离误差(mm)
	int offset_correction_;		//偏移量修正(mm)(星标中心点与电机中心点的相对位置)
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

