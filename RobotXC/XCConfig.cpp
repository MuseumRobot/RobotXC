#include "StdAfx.h"
#include "XCConfig.h"
XCConfig::XCConfig(void){
	init();		//初始化配置项
	LoadFromFile("./Configure/Config.ini");		//	LoadFromFile("Configure/Config.ini");这种写法也可以
}
XCConfig::~XCConfig(void){}
void XCConfig::init(){
	comm_motor_ = 3;			//默认电机串口3
	comm_star_ = 4;				//默认星标串口4
	comm_laser_ = 5;			//默认激光串口5
	gui_mode_ = 0;				//默认开发者模式
	instruction_cycle_ = 200;	//默认指令周期ms
	data_refresh_cycle_ = 50;	//默认数据刷新周期ms
	obstacle_threshold_ = 400;	//默认通行阈值mm
	far_obs_threshold_ = 800;	//默认畅通阈值mm
	error_angle_ = 10;			//默认误差角°
	error_distance_ = 150;		//默认误差距离mm
	offset_correction_ = 325;	//默认修正偏移量mm
	map_scale_ = 10;			//默认绘图尺度为每格边长10像素
}
bool XCConfig::LoadFromFile(const string &config_file){
	ifstream fin;
	fin.open(config_file.c_str());		//打开文件
	if (!fin) return false;
	//按行遍历
	string line;
	while(getline(fin,line)){
		line = trim(line);
		if (line.empty() || line[0] == '#' ){	//为空或首字符为"#",则忽略
			continue;
		}
		vector<string> key_value = split(line,"=");		//根据"="分裂字符串
		if (stricmp(key_value[0].c_str(),"comm_motor") == 0){
			comm_motor_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"comm_star") == 0){
			comm_star_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"comm_laser") == 0){
			comm_laser_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"gui_mode") == 0){
			gui_mode_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"instruction_cycle") == 0){
			instruction_cycle_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"data_refresh_cycle") == 0){
			data_refresh_cycle_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"obstacle_threshold") == 0){
			obstacle_threshold_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"far_obs_threshold") == 0){
			far_obs_threshold_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"error_angle") == 0){
			error_angle_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"error_distance") == 0){
			error_distance_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"offset_correction") == 0){
			offset_correction_ = stoi(key_value[1]);
		}else if (stricmp(key_value[0].c_str(),"map_scale") == 0){
			map_scale_ = stoi(key_value[1]);
		}
	}
	fin.close();		//关闭文件
	return true;
}
string XCConfig::trim(const string& str){	//字符串trim函数
	string t = str;
	t.erase(0, t.find_first_not_of(" \t\n\r"));
	t.erase(t.find_last_not_of(" \t\n\r") + 1);
	return t;
}  
vector<string> XCConfig::split(string str,string pattern){		//字符串分割函数
	string::size_type pos;
	vector<std::string> result;
	str+=pattern;		//扩展字符串以方便操作
	int size=str.size();
	for(int i=0; i<size; i++){
		pos=str.find(pattern,i);
		if(pos<size){
			std::string s=str.substr(i,pos-i);
			s=trim(s);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}
int XCConfig::comm_motor(){
	return comm_motor_;
}
int XCConfig::comm_star(){
	return comm_star_;
}
int XCConfig::comm_laser(){
	return comm_laser_;
}
int XCConfig::gui_mode(){
	return gui_mode_;
}
int XCConfig::instruction_cycle(){
	return instruction_cycle_;
}
int XCConfig::data_refresh_cycle(){
	return data_refresh_cycle_;
}
int XCConfig::obstacle_threshold(){
	return obstacle_threshold_;
}
int XCConfig::far_obs_threshold(){
	return far_obs_threshold_;
}
int XCConfig::error_angle(){
	return error_angle_;
}
int XCConfig::error_distance(){
	return error_distance_;
}
int XCConfig::offset_correction(){
	return offset_correction_;
}
int XCConfig::map_scale(){
	return map_scale_;
}