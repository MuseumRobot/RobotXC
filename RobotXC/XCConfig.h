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
	int map_scale_;				//地图显示尺寸(用于设置绘制地图时一个单元格的边长，单位为像素)
	int architect_scale_;		//地图比例尺(一格边长代表着真实世界多少cm)
	void init();
	string trim(const string& str);
	vector<string> split(string str,string pattern);
	bool LoadFromFile(const string &config_file);
public:
	int comm_motor();			//获取电机串口号
	int comm_star();			//获取星标串口号
	int comm_laser();			//获取激光串口号
	int gui_mode();				//获取图形化交互界面显示模式	
	int instruction_cycle();	//获取指令周期(ms)
	int data_refresh_cycle();	//获取数据刷新周期(ms)
	int obstacle_threshold();	//获取通行阈值(mm)(如果该距离内没有探测到物体则认为前方可行)
	int far_obs_threshold();	//获取通畅阈值(mm)(如果该距离内没有探测到物体则认为前方通畅，用于高速行驶)
	int error_angle();			//获取角度误差(°)
	int error_distance();		//获取距离误差(mm)
	int offset_correction();	//获取偏移量修正(mm)(星标中心点与电机中心点的相对位置)
	int map_scale();			//获取地图显示尺寸(用于设置绘制地图时一个单元格的边长，单位为像素)
	int architect_scale();		//获取地图比例尺(一格边长代表着真实世界多少cm)
public slots:
	void map_scale_add();
	void map_scale_diminish();
};

