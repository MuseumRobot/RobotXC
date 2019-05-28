#include "stdafx.h"
#include "RobotXC.h"
RobotXC::RobotXC(QWidget *parent, Qt::WFlags flags):QMainWindow(parent, flags){
	ui.setupUi(this);
	m_voice = new XCVoice;
	m_config = new XCConfig;
	m_overview = new XCOverview;
	
	m_overview->m_config = m_config;	//将配置项提供给绘图组件
	if(LoadMap()){
		m_overview->m_map = m_map;		//如果成功读取地图，则将地图赋给绘图组件
	}
	ui.overview->setWidget(m_overview);	//将绘图组件放在主界面的ui.overview控件(QScrollArea)中
	if(m_overview->m_map!=NULL){
		m_overview->setFixedWidth(m_overview->m_map->N * m_config->map_scale());
		m_overview->setFixedHeight(m_overview->m_map->M * m_config->map_scale());
	}
	connect(ui.btnRecord,SIGNAL(clicked()),this,SLOT(OnBtnRecord()));
	connect(ui.actionZoomIn,SIGNAL(triggered()),m_config,SLOT(map_scale_add()));
	connect(ui.actionZoomout,SIGNAL(triggered()),m_config,SLOT(map_scale_diminish()));
	timer_instruction = startTimer(m_config->instruction_cycle());		//启动指令周期计时器
	AStar a;
	a.Init(m_map);
	
}
RobotXC::~RobotXC(){
	delete m_voice;
	delete m_config;
	if(m_map!=NULL){
		delete m_map;
		m_map = NULL;
	}
	delete m_overview;
}
void RobotXC::timerEvent(QTimerEvent *event){
	if(event->timerId() == timer_instruction){

	}
}
void RobotXC::OnBtnRecord(){
	//m_voice->StartRecord();
	if(m_overview->m_map!=NULL){
		delete m_overview->m_map;
		m_overview->m_map = NULL;
	}
}
bool RobotXC::LoadMap(){
	QString filepath = "./Configure/museum.map";
	if(filepath.length()>0){
		string gbk_name=GBK::FromUnicode(filepath);
		//打开文件，读取内容
		FILE* fp=fopen(gbk_name.c_str(),"rb");
		//文件的大小
		if(fp != NULL){
			int M,N;
			fread(&M,sizeof(int),1,fp);
			fread(&N,sizeof(int),1,fp);
			m_map = new Map(M,N);
			for(int i=0;i<M;i++){
				for(int j=0;j<N;j++){
					fread(&(m_map->m_maze[i][j]),sizeof(int),1,fp);
					fread(&(m_map->m_weight[i][j]),sizeof(int),1,fp);
				}
			}
			fclose(fp);	//关闭文件
		}else{
			return false;
		}
	}else{
		return false;
	}
	return true;
}