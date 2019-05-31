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
	int fatal_obs_threshold_;	//致命阈值(mm)(如果该距离内探测到障碍物则停止运行静待障碍离开)
	int map_scale_;				//地图显示尺寸(用于设置绘制地图时一个单元格的边长，单位为像素)
	int architect_scale_;		//地图比例尺(一格边长代表着真实世界多少cm)
	float error_angle_;			//角度误差(°)
	float error_distance_;		//距离误差(mm)
	float offset_correction_;	//偏移量修正(mm)(星标中心点与电机中心点的相对位置)
	float speed_line_basic_simulate_;		//模拟运动基础线速度(cm/cycle)
	float speed_line_basic_;				//真实运动基础线速度(单位不详)
	float speed_angle_basic_simulate_;		//模拟运动基础角速度(°/cycle)
	float speed_angle_basic_;				//真实运动基础角速度(单位不详)
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
	int fatal_obs_threshold();	//致命阈值(mm)(如果该距离内探测到障碍物则停止运行静待障碍离开)
	int map_scale();			//获取地图显示尺寸(用于设置绘制地图时一个单元格的边长，单位为像素)
	int architect_scale();		//获取地图比例尺(一格边长代表着真实世界多少cm)
	float error_angle();		//获取角度误差(°)
	float error_distance();		//获取距离误差(mm)
	float offset_correction();	//获取偏移量修正(mm)(星标中心点与电机中心点的相对位置)
	float speed_line_basic();	//获取基础线速度
	float speed_angle_basic();	//获取基础角速度
	float speed_line_basic_simulate();	//获取基础模拟线速度
	float speed_angle_basic_simulate();	//获取基础模拟角速度
public slots:
	void map_scale_add();
	void map_scale_diminish();
};

