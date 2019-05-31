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
		ui.overview->setWidget(m_overview);	//����ͼ��������������ui.overview�ؼ�(QScrollArea)��
		if(m_overview->m_map!=NULL){
			m_overview->setFixedWidth(m_overview->m_map->N * m_config->map_scale());
			m_overview->setFixedHeight(m_overview->m_map->M * m_config->map_scale());
		}
		m_control->ui.checkMap->setChecked(true);
	}
	m_control->ui.checkHCI->setChecked(m_voice->m_isAuthReady);
	m_control->ui.checkTTS->setChecked(m_voice->m_isTTSReady);
	m_control->ui.checkASR->setChecked(m_voice->m_isASRReady);
	m_control->ui.checkTask->setChecked(LoadTask());
	m_control->ui.checkSpeak->setChecked(LoadSpeakContent());
	connect(ui.btnRecord,SIGNAL(clicked()),this,SLOT(OnBtnRecord()));
	connect(ui.actionZoomIn,SIGNAL(triggered()),m_config,SLOT(map_scale_add()));
	connect(ui.actionZoomout,SIGNAL(triggered()),m_config,SLOT(map_scale_diminish()));
	connect(ui.actionDisplayDilate,SIGNAL(triggered()),this,SLOT(OnBtnDisplayDilate()));
	connect(m_control->ui.btnSimulate,SIGNAL(clicked()),this,SLOT(OnBtnChangeSimulateMode()));
	connect(m_control->ui.btnAutoGuide,SIGNAL(clicked()),this,SLOT(OnBtnChangeAutoMode()));
	connect(m_control->ui.btnFastMode,SIGNAL(clicked()),this,SLOT(OnBtnChangeFastMode()));
	connect(m_control->ui.btnLeft,SIGNAL(clicked()),this,SLOT(OnBtnTurnLeft()));
	connect(m_control->ui.btnRight,SIGNAL(clicked()),this,SLOT(OnBtnTurnRight()));
	connect(m_control->ui.btnForward,SIGNAL(clicked()),this,SLOT(OnBtnMoveForward()));
	connect(m_control->ui.btnBackward,SIGNAL(clicked()),this,SLOT(OnBtnMoveBackward()));
	connect(m_control->ui.btnSpeak,SIGNAL(clicked()),this,SLOT(OnBtnSpeak()));
	connect(m_control->ui.btnStopSpeak,SIGNAL(clicked()),this,SLOT(OnBtnStopSpeak()));
	connect(m_control->ui.btnPreset1,SIGNAL(clicked()),this,SLOT(OnBtnPreset1()));
	connect(m_control->ui.btnPreset2,SIGNAL(clicked()),this,SLOT(OnBtnPreset2()));
	connect(m_control->ui.btnPreset3,SIGNAL(clicked()),this,SLOT(OnBtnPreset3()));
	connect(m_control->ui.btnHome,SIGNAL(clicked()),this,SLOT(OnBtnHome()));
	m_control->show();
	m_control->move(20,20);
	timer_instruction = startTimer(m_config->instruction_cycle());		//����ָ�����ڼ�ʱ��
	timer_data_refresh = startTimer(m_config->data_refresh_cycle());	//��������ˢ�¼�ʱ��
	m_astar = new AStar;
	robotPos = QPointF(43.00,23.00);
	goalPos = QPointF(142.0,160.0);
	robotFaceAngle = 0.0;
	m_astar->Init(m_map);
	//ʵ��Ӧ��ʱ��ͼ��Ҫ����,����m_dilate_maze��,��ԭͼ����m_maze�����б���
	m_astar->m_map->m_dilate_maze = m_astar->DilateMatrix(m_config->obstacle_threshold()/10/m_config->architect_scale(),m_map->m_maze);	
	m_map->x_start = robotPos.y()/m_config->architect_scale();
	m_map->y_start = robotPos.x()/m_config->architect_scale();
	m_map->x_end = goalPos.y()/m_config->architect_scale();
	m_map->y_end = goalPos.x()/m_config->architect_scale();
	m_astar->Calculate(false);
	m_result = m_astar->GetResultList();
	m_overview->m_result = m_result;
	GetResultF();								//����m_result_f�Բ���Ѱ·����

	isSimulateMode = false;
	isAutoMode = false;
	isFastMode = false;
	m_speakWaitCycle = 0;
	m_control->ParseTaskSequence("1,22,2,23,24,3,25,26,4,27,5,28,29,6,30,7,31,76");

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
		if(isAutoMode && !m_control->m_taskSequence.empty()){
			if(m_speakWaitCycle<1){
				AssignInstruction();	//����ָ��
				//����������ˣ����˳��Զ�����ģʽ�������Ӧ�����ȴ����ٲ���
				if(m_control->m_taskSequence.empty()){
					OnBtnChangeAutoMode();
					m_speakWaitCycle = 0;
				}
			}else{
				if(isFastMode){
					m_speakWaitCycle/=2;
				}else{
					m_speakWaitCycle-=1;
				}
			}
		}
	}else if(event->timerId() == timer_data_refresh){
		DataRefresh();
	}
}
void RobotXC::AssignInstruction(){
	int taskID = *(m_control->m_taskSequence.begin());
	XCTaskDataType* task = findTask(taskID);
	if(task != NULL){
		if(JudgeTaskType(taskID) == 0){		//λ������
			goalPos = QPointF(task->y,task->x);
			faceAudianceAngle = Modf360(90-task->FacingAngle);
			float errorRange_Distance = m_config->error_distance()/10;	//�ִ�Ŀ���ľ�����Χ����λcm
			QPointF d = goalPos - robotPos;
			float dDistance = sqrt(pow(d.x(),2)+pow(d.y(),2));		//���������ĵ�Ŀ���ľ��룬��λcm
			float errorRange_Angle = m_config->error_angle();		//ѡ��Ƕȵ���Χ����λ��
			if(dDistance > errorRange_Distance){					//�����û�еִﵱǰ����Ŀ���ͼ���ִ������
				float goalAngle = GetAngleFromVector(d);
				if(Modf360(abs(goalAngle - robotFaceAngle))>errorRange_Angle){		
					TrunForwardGoal(goalAngle);						//�����Ŀ��ǶȲ������ת��Ŀ��
				}else{
					MoveForward(1);									//ת����Ҫ�ĽǶȺ�ͼ�����
				}
			}else{													//����Ѿ��ִﵱǰĿ��㣬����Ҫת�����
				if(Modf360(abs(faceAudianceAngle - robotFaceAngle))>errorRange_Angle){		
					TrunForwardGoal(faceAudianceAngle);
				}else{												//����Ѿ�ת����ڣ����������λ������
					m_control->m_taskSequence.erase(m_control->m_taskSequence.begin());
					m_control->UpdateTaskSequence();
				}		
			}
		}else{		//��������
			int SpeakContentId = task->SpeakContentId;
			XCSpeakContentType* speakContent = findSpeakContent(SpeakContentId);
			QString str = speakContent->content;
			m_voice->Speak(str);
			m_speakWaitCycle = str.length()/SPEAKWORDSPERSECOND*1000/m_config->instruction_cycle()+10;
			m_control->m_taskSequence.erase(m_control->m_taskSequence.begin());
			m_control->UpdateTaskSequence();
		}
	}
}
void RobotXC::AssignPresetTask(int n){
	switch (n){
	case 1:m_control->ParseTaskSequence("1,22,2,23,24,3,25,26,4,27,5,28,29,6,30,7,31,76");break;
	case 2:m_control->ParseTaskSequence("8,32,9,33,34,10,35,11,36,37,38,39,76");break;
	case 3:m_control->ParseTaskSequence("12,40,41,42,43,13,44,45,14,46,47,48,49,15,50,51,52,53,63,64,16,54,55,56,57,58,59,60,17,61,62,18,65,70,71,72,73,74,75,19,66,20,67,68,69,76");break;
	default:m_control->ParseTaskSequence("8");	//ȱʡ�ؼ�
	}
}
void RobotXC::DataRefresh(){
	m_overview->m_robotPos = robotPos;
	m_overview->m_robotFaceAngle = robotFaceAngle;
}
void RobotXC::TrunForwardGoal(float goalAngle){
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
		robotFaceAngle -= ratio*m_config->speed_angle_basic_simulate();
	}else{
		//��ʵ�˶�
	}
}
void RobotXC::TurnRight(float ratio){
	if(isSimulateMode){
		robotFaceAngle += ratio*m_config->speed_angle_basic_simulate();
	}else{

	}
}
void RobotXC::MoveForward(float ratio){
	if(isSimulateMode){
		robotPos += QPointF(ratio*m_config->speed_line_basic_simulate()*cos(robotFaceAngle/360.0*2*PI),ratio*m_config->speed_line_basic_simulate()*sin(robotFaceAngle/360.0*2*PI));
	}else{

	}
}
void RobotXC::MoveBackward(float ratio){
	if(isSimulateMode){
		robotPos -= QPointF(ratio*m_config->speed_line_basic_simulate()*cos(robotFaceAngle/360.0*2*PI),ratio*m_config->speed_line_basic_simulate()*sin(robotFaceAngle/360.0*2*PI));
	}else{

	}
}
void RobotXC::OnBtnTurnLeft(){
	TurnLeft(1);
}
void RobotXC::OnBtnTurnRight(){
	TurnRight(1);
}
void RobotXC::OnBtnMoveForward(){
	MoveForward(1);
}
void RobotXC::OnBtnMoveBackward(){
	MoveBackward(1);
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
		m_overview->setFixedWidth(ui.overview->width());
		m_overview->setFixedHeight(ui.overview->height());
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
			m_map = new XCMap(M,N);
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
void RobotXC::OnBtnChangeAutoMode(){
	if(isAutoMode == false){
		m_control->ui.btnAutoGuide->setText(GBK::ToUnicode("�ر��Զ�����"));
		m_voice->Speak("�Զ������ѿ�����");
	}else{
		m_control->ui.btnAutoGuide->setText(GBK::ToUnicode("����ģ��ģʽ"));
		m_voice->Speak("�Զ������ѹرգ�");
	}
	isAutoMode = !isAutoMode;
}
void RobotXC::OnBtnChangeFastMode(){
	if(isFastMode == false){
		m_control->ui.btnFastMode->setText(GBK::ToUnicode("�رտ���ģʽ"));
	}else{
		m_control->ui.btnFastMode->setText(GBK::ToUnicode("��������ģʽ"));
	}
	isFastMode = !isFastMode;
}
void RobotXC::OnBtnSpeak(){
	QString str = m_control->ui.textSpeek->currentText();
	m_voice->Speak(str);
}
void RobotXC::OnBtnStopSpeak(){
	m_voice->StopSpeak();
}
void RobotXC::OnBtnPreset1(){
	AssignPresetTask(1);
	m_voice->Speak("���л���·��һ��");
}		
void RobotXC::OnBtnPreset2(){
	AssignPresetTask(2);
	m_voice->Speak("���л���·�߶���");
}		
void RobotXC::OnBtnPreset3(){
	AssignPresetTask(3);
	m_voice->Speak("���л���·������");
}		
void RobotXC::OnBtnHome(){
	AssignPresetTask(0);
	m_voice->Speak("���л����ؼ�·�ߣ�");
}
void RobotXC::GetResultF(){
	//����任��������������(m��,n��)��תΪoverview�µ�����ϵ(x,y)��x=n*map_scale,y=m*map_scale
	for(std::list<XCPoint>::iterator iter = m_result.begin(); iter != m_result.end(); iter++){
		QPointF a((iter->y + 0.5)*m_config->map_scale(),(iter->x + 0.5)*m_config->map_scale());
		m_result_f.push_back(a);
	}
}
bool RobotXC::LoadTask(){
	FILE* fp = fopen("Configure/task.data","rb");
	if(!fp) return false;
	m_TaskDataRecords.clear(); // ���
	// ��������	
	while( !feof(fp)){
		XCTaskDataType record;
		int n = fread(&record, 1, sizeof(XCTaskDataType), fp);
		if( n < 0 ) break;
		if( n == 0) continue;
		m_TaskDataRecords.push_back(record);
	}
	fclose(fp);
	return true;
}
bool RobotXC::LoadSpeakContent(){
	FILE* fp = fopen("Configure/speakContent.data", "rb");
	if(!fp) return false;
	m_SpeakContentRecords.clear(); // ���
	// ��������	
	while( !feof(fp)){
		XCSpeakContentType record;
		int n = fread(&record, 1, sizeof(XCSpeakContentType), fp);
		if( n < 0 ) break;
		if( n == 0) continue;
		m_SpeakContentRecords.push_back(record);
	}
	fclose(fp);
	return true;
}
XCTaskDataType* RobotXC::findTask(int taskId){
	for(XCTaskDataTypeList::iterator iter = m_TaskDataRecords.begin();iter != m_TaskDataRecords.end(); iter ++){
		XCTaskDataType& record = *iter;
		if(record.id == taskId){
			return &record;
		}
	}
	return NULL;
}
XCSpeakContentType* RobotXC::findSpeakContent(int speakContentId){
	for(XCSpeakContentTypeList::iterator iter = m_SpeakContentRecords.begin();iter != m_SpeakContentRecords.end(); iter ++){
		XCSpeakContentType& record = *iter;
		if(record.id == speakContentId){
			return &record;
		}
	}
	return NULL;
}
int RobotXC::JudgeTaskType(int taskID){
	XCTaskDataType* task = findTask(taskID);
	if(task != NULL){
		return task->taskType;
	}else{
		return -1;	//�����쳣ֵ��ΪtodoList����ֹ��
	}
}