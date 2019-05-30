#include "stdafx.h"
#include "RobotXC.h"
RobotXC::RobotXC(QWidget *parent, Qt::WFlags flags):QMainWindow(parent, flags){
	ui.setupUi(this);
	m_voice = new XCVoice;
	m_config = new XCConfig;
	m_overview = new XCOverview;
	m_control = new XCControl;
	m_overview->m_config = m_config;	//���������ṩ����ͼ���
	if(LoadMap()){
		m_overview->m_map = m_map;		//����ɹ���ȡ��ͼ���򽫵�ͼ������ͼ���
	}
	ui.overview->setWidget(m_overview);	//����ͼ��������������ui.overview�ؼ�(QScrollArea)��
	if(m_overview->m_map!=NULL){
		m_overview->setFixedWidth(m_overview->m_map->N * m_config->map_scale());
		m_overview->setFixedHeight(m_overview->m_map->M * m_config->map_scale());
	}

	connect(ui.btnRecord,SIGNAL(clicked()),this,SLOT(OnBtnRecord()));
	connect(ui.actionZoomIn,SIGNAL(triggered()),m_config,SLOT(map_scale_add()));
	connect(ui.actionZoomout,SIGNAL(triggered()),m_config,SLOT(map_scale_diminish()));
	connect(ui.actionDisplayDilate,SIGNAL(triggered()),this,SLOT(OnBtnDisplayDilate()));
	connect(m_control->ui.btnSimulate,SIGNAL(clicked()),this,SLOT(OnBtnChangeSimulateMode()));
	m_control->show();
	timer_instruction = startTimer(m_config->instruction_cycle());		//����ָ�����ڼ�ʱ��
	m_astar = new AStar;
	m_astar->Init(m_map);
	//ʵ��Ӧ��ʱ��ͼ��Ҫ����,����m_dilate_maze��,��ԭͼ����m_maze�����б���
	m_astar->m_map->m_dilate_maze = m_astar->DilateMatrix(m_config->obstacle_threshold()/10/m_config->architect_scale(),m_map->m_maze);	
	m_map->x_start = m_map->y_start = 5;
	m_map->x_end = m_map->y_end = 15;
	m_astar->Calculate(false);
	m_astar->Uninit();
	m_result = m_astar->GetResultList();
	m_overview->m_result = m_result;
	robotPos = QPointF(0.00,0.00);
	robotFaceAngle = 0.0;
	isSimulateMode = false;
}
RobotXC::~RobotXC(){
	delete m_voice;
	delete m_config;
	delete m_overview;
	delete m_astar;
	if(m_map!=NULL){
		delete m_map;
		m_map = NULL;
	}
}
void RobotXC::timerEvent(QTimerEvent *event){
	if(event->timerId() == timer_instruction){
		AssignInstruction();
	}
}
void RobotXC::AssignInstruction(){

}
void RobotXC::TrunForwardGoal(){
	QPointF d = goalPos - robotPos;
	float goalAngle = GetAngleFromVector(d);
	float deltaAngle = Modf360(goalAngle - robotFaceAngle);
	if(deltaAngle > 180){
		if(deltaAngle > 330) TurnLeft(0.5);
		else TurnLeft(1);
	}else{
		if(deltaAngle < 30) TurnRight(0.5);
		else TurnRight(1);
	}
}
void RobotXC::TurnLeft(float ratio){
	if(isSimulateMode){

	}else{
		//��ʵ�˶�
	}
}
void RobotXC::TurnRight(float ratio){
	if(isSimulateMode){

	}else{

	}
}
void RobotXC::MoveForward(float ratio){
	if(isSimulateMode){

	}else{

	}
}
void RobotXC::MoveBackward(float ratio){
	if(isSimulateMode){

	}else{

	}
}
float RobotXC::GetAngleFromVector(QPointF delta){
	float angle = 0.0;
	if(delta.x() > 0 && delta.y() > 0){
		angle =atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�һ����
	}else if(delta.x() < 0 && delta.y() < 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�������
	}else if(delta.x() <0 && delta.y() > 0){
		angle =180 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵڶ�����
	}else{
		angle =360 + atan(delta.y()/delta.x())/PI/2*360.0;	//Ŀ���ڵ�������
	}
	return angle;
}
float RobotXC::Modf360(float angle){
	while(angle>360.0 || angle<0.0){
		if(angle>360.0) angle -= 360.0;
		else angle += 360.0;
	}
	return angle;
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
		//���ļ�����ȡ����
		FILE* fp=fopen(gbk_name.c_str(),"rb");
		//�ļ��Ĵ�С
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
			fclose(fp);	//�ر��ļ�
		}else{
			return false;
		}
	}else{
		return false;
	}
	return true;
}
void RobotXC::OnBtnDisplayDilate(){
	if(ui.actionDisplayDilate->isChecked()){
		m_overview->m_showDilateMap = 1;
	}else{
		m_overview->m_showDilateMap = 0;
	}
}
void RobotXC::OnBtnChangeSimulateMode(){
	if(isSimulateMode == false){
		m_control->ui.btnSimulate->setText(GBK::ToUnicode("�ر�ģ��ģʽ"));
	}else{
		m_control->ui.btnSimulate->setText(GBK::ToUnicode("����ģ��ģʽ"));
	}
	isSimulateMode = !isSimulateMode;
}