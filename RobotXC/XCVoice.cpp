#include "StdAfx.h"
#include "XCVoice.h"


XCVoice::XCVoice(void){
	m_isASRReady = m_isTTSReady = m_isAuthReady = false;
	Init();
}

XCVoice::~XCVoice(void){}

void XCVoice::Speak(QString str){
	unsigned char* pszUTF8 = NULL;
	HciExampleComon::GBKToUTF8( (unsigned char*)str.toStdString().c_str(), &pszUTF8 );
	string startConfig = "property=cn_xiaokun_common,tagmode=none,capkey=tts.cloud.wangjing";
	hci_tts_player_stop();	//�󷢵�����ָ��Ὣ֮ǰ������ָ���
	QTest::qSleep(100);		//����stopָ������ж���ʱ���Թ�stop����ִ����ɣ������޷�start��һ��
	PLAYER_ERR_CODE eRetk = hci_tts_player_start( (const char*)pszUTF8, startConfig.c_str() );
}
void XCVoice::StartRecord(){
	RECORDER_ERR_CODE eRet = RECORDER_ERR_NONE;
	string startConfig = "";
	startConfig += "capkey=" + account_info->cap_key();
	startConfig += ",audioformat=pcm16k16bit";
	if(IS_RECORDER_CONTINUE)	//�Ƿ�����¼��
		startConfig += ",continuous=yes";	
	if ( m_RecogMode == kRecogModeDialog ){
		startConfig +=",intention=weather;joke;story;baike;calendar;translation;news"; 
	}
	eRet = hci_asr_recorder_start(startConfig.c_str(),"");
	if (RECORDER_ERR_NONE != eRet){
		QString strErrMessage;
		strErrMessage.sprintf( "��ʼ¼��ʧ��,������%d", eRet );
		QMessageBox msgBox;
		msgBox.setText(strErrMessage);
		msgBox.exec();
	}
}
bool XCVoice::Init(){
	account_info = AccountInfo::GetInstance();				// ��ȡAccountInfo����
	string account_info_file = "./Configure/AccountInfo.ini";	// �˺���Ϣ��ȡ
	account_info->LoadFromFile(account_info_file);
	// SYS��ʼ��
	HCI_ERR_CODE errCode = HCI_ERR_NONE;
	// ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
	string init_config = "";
	init_config += "appKey=" + account_info->app_key();              //����Ӧ�����
	init_config += ",developerKey=" + account_info->developer_key(); //���ƿ�������Կ
	init_config += ",cloudUrl=" + account_info->cloud_url();         //�����Ʒ���Ľӿڵ�ַ
	init_config += ",authpath=" + account_info->auth_path();         //��Ȩ�ļ�����·������֤��д
	init_config += ",logfilesize=1024000,loglevel=5";	// ��������ʹ��Ĭ��ֵ��������ӣ���������ÿ��Բο������ֲ�
	errCode = hci_init( init_config.c_str() );
	m_isAuthReady = CheckAndUpdataAuth();
	m_isTTSReady = InitTTS();
	m_isASRReady = InitASR();
	return true;
}
bool XCVoice::InitTTS(){
	//tts_player��ʼ��
	PLAYER_CALLBACK_PARAM cb;
	cb.pvStateChangeUsrParam			= this;
	cb.pvProgressChangeUsrParam			= this;
	cb.pvPlayerErrorUsrParam			= this;
	cb.pfnProgressChange			= XCVoice::CB_ProgressChange;
	cb.pfnStateChange				= XCVoice::CB_EventChange;
	cb.pfnPlayerError				= XCVoice::CB_SdkErr;

	PLAYER_ERR_CODE eReti = PLAYER_ERR_NONE;
	string initConfigtts = "InitCapkeys=tts.cloud.wangjing";
	initConfigtts += ",dataPath="+account_info->data_path();
	eReti = hci_tts_player_init( initConfigtts.c_str(), &cb );
	if(eReti == 0){
		qDebug()<<"tts ready!";
		Speak("���ѽ��");
		return true;
	}else{
		return false;
	}
}
bool XCVoice::InitASR(){
	m_RecogType = kRecogTypeUnkown;
	m_RecogMode = kRecogModeUnkown;
	GetASRCapkeyProperty(account_info->cap_key(),m_RecogType,m_RecogMode);
	RECORDER_ERR_CODE eRet = RECORDER_ERR_UNKNOWN;
	RECORDER_CALLBACK_PARAM call_back;
	memset( &call_back, 0, sizeof(RECORDER_CALLBACK_PARAM) );
	call_back.pvStateChangeUsrParam		= this;
	call_back.pvRecogFinishUsrParam		= this;
	call_back.pvErrorUsrParam			= this;
	call_back.pvRecordingUsrParam		= this;
	call_back.pvRecogProcessParam		= this;
	call_back.pfnStateChange	= XCVoice::RecordEventChange;
	call_back.pfnRecogFinish	= XCVoice::RecorderRecogFinish;
	call_back.pfnError			= XCVoice::RecorderErr;
	call_back.pfnRecording		= XCVoice::RecorderRecordingCallback;
	call_back.pfnRecogProcess   = XCVoice::RecorderRecogProcess;
	string initConfig = "initCapkeys=" + account_info->cap_key();	
	initConfig        += ",dataPath=" + account_info->data_path();
	eRet = hci_asr_recorder_init( initConfig.c_str(), &call_back);
	if (eRet == RECORDER_ERR_NONE){
		qDebug()<<"asr ready!";
		return true;
	}else{
		return false;
	}
}
bool XCVoice::CheckAndUpdataAuth(){
	//��ȡ����ʱ��
	int64 nExpireTime;
	int64 nCurTime = (int64)time( NULL );
	HCI_ERR_CODE errCode = hci_get_auth_expire_time( &nExpireTime );
	qDebug()<<"get auth expire time return("<<errCode<<":"<<hci_get_error_info(errCode)<<")";
	if( errCode == HCI_ERR_NONE ){
		//��ȡ�ɹ����ж��Ƿ����
		if( nExpireTime > nCurTime ){	//û�й���
			return true;
		}
	}
	//��ȡ����ʱ��ʧ�ܻ��Ѿ�����
	//�ֶ����ø�����Ȩ
	errCode = hci_check_auth();
	qDebug()<<"check auth return("<<errCode<<":"<<hci_get_error_info(errCode)<<")";
	if( errCode == HCI_ERR_NONE ){	//���³ɹ�
		return true;
	}
	else{	//����ʧ��
		return false;
	}
}
void XCVoice::GetASRCapkeyProperty(const string &cap_key,AsrRecogType &type,AsrRecogMode &mode){
	HCI_ERR_CODE errCode = HCI_ERR_NONE;
	CAPABILITY_ITEM *pItem = NULL;
	// ö�����е�asr����
	CAPABILITY_LIST list = {0};
	if ((errCode = hci_get_capability_list("asr", &list))!= HCI_ERR_NONE){		// û���ҵ���Ӧ��������
		return;
	}
	// ��ȡasr����������Ϣ��
	for (int i = 0; i < list.uiItemCount; i++){
		if (list.pItemList[i].pszCapKey != NULL && stricmp(list.pItemList[i].pszCapKey, cap_key.c_str()) == 0){
			pItem = &list.pItemList[i];
			break;
		}
	}
	// û�л�ȡ��Ӧ�������ã����ء�
	if (pItem == NULL || pItem->pszCapKey == NULL){
		hci_free_capability_list(&list);
		return;
	}
	if (strstr(pItem->pszCapKey, "cloud") != NULL){
		type = kRecogTypeCloud;
	}else{
		type = kRecogTypeLocal;
	}  
	if (strstr(pItem->pszCapKey, "freetalk") != NULL){
		mode = kRecogModeFreetalk;
	}else if (strstr(pItem->pszCapKey, "grammar") != NULL){
		mode = kRecogModeGrammar;
	}else if(strstr(pItem->pszCapKey, "dialog")!= NULL){
		mode = kRecogModeDialog;
	}else{
		mode = kRecogModeUnkown;
	}
	hci_free_capability_list(&list);
	return;
}

//TTS�ص�����ʵ��
void HCIAPI XCVoice::CB_EventChange(_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_OPT_ _IN_ void * pUsrParam){
	string strEvent;
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "��ʼ����";break;
	case PLAYER_EVENT_PAUSE:strEvent = "��ͣ����"; break;
	case PLAYER_EVENT_RESUME:strEvent = "�ָ�����";break;
	case PLAYER_EVENT_PROGRESS:strEvent = "���Ž���";break;
	case PLAYER_EVENT_BUFFERING:strEvent = "���Ż���";break;
	case PLAYER_EVENT_END:strEvent = "�������";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "�������";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "�豸����";break;
	}
}
void HCIAPI XCVoice::CB_ProgressChange (_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ int nStart,_MUST_ _IN_ int nStop,_OPT_ _IN_ void * pUsrParam){
	string strEvent;
	char szData[256] = {0};
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "��ʼ����";break;
	case PLAYER_EVENT_PAUSE:strEvent = "��ͣ����";break;
	case PLAYER_EVENT_RESUME:strEvent = "�ָ�����";break;
	case PLAYER_EVENT_PROGRESS:sprintf( szData, "���Ž��ȣ���ʼ=%d,�յ�=%d", nStart, nStop );strEvent = szData;break;
	case PLAYER_EVENT_BUFFERING:strEvent = "���Ż���";break;
	case PLAYER_EVENT_END:strEvent = "�������";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "�������";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "�豸����";break;
	}
}
void HCIAPI XCVoice::CB_SdkErr( _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ HCI_ERR_CODE eErrorCode,_OPT_ _IN_ void * pUsrParam ){
	string strEvent;
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "��ʼ����";break;
	case PLAYER_EVENT_PAUSE:strEvent = "��ͣ����";break;
	case PLAYER_EVENT_RESUME:strEvent = "�ָ�����";break;
	case PLAYER_EVENT_PROGRESS:strEvent = "���Ž���";break;
	case PLAYER_EVENT_BUFFERING:strEvent = "���Ż���";break;
	case PLAYER_EVENT_END:strEvent = "�������";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "�������";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "�豸����";break;
	}
}

//ASR�ص����������м�״̬�Ľṹ�弰�ص�����ʵ��
struct{
	char* pszName;
	char* pszComment;
}g_sStatus[] ={
	{"RECORDER_EVENT_BEGIN_RECORD",         "��ʼ����..."},
	{"RECORDER_EVENT_HAVING_VOICE",         "�������� ��⵽ʼ�˵�ʱ��ᴥ�����¼�"},
	{"RECORDER_EVENT_NO_VOICE_INPUT",       "û����������"},
	{"RECORDER_EVENT_BUFF_FULL",            "������������"},
	{"RECORDER_EVENT_END_RECORD",           "������ϣ�"},
	{"RECORDER_EVENT_BEGIN_RECOGNIZE",      "��ʼʶ��"},
	{"RECORDER_EVENT_RECOGNIZE_COMPLETE",   "ʶ�����"},
	{"RECORDER_EVENT_ENGINE_ERROR",         "�������"},
	{"RECORDER_EVENT_DEVICE_ERROR",         "�豸����"},
	{"RECORDER_EVENT_MALLOC_ERROR",         "����ռ�ʧ��"},
	{"RECORDER_EVENT_INTERRUPTED",          "�ڲ�����"},
	{"RECORDER_EVENT_PERMISSION_DENIED",    "�ڲ�����"},
	{"RECORDER_EVENT_TASK_FINISH",          "ʶ���������"},
	{"RECORDER_EVENT_RECOGNIZE_PROCESS",    "ʶ���м�״̬"}
};
void HCIAPI XCVoice::RecordEventChange(RECORDER_EVENT eRecorderEvent, void *pUsrParam){
	XCVoice* dlg = (XCVoice*)pUsrParam;
	if(eRecorderEvent == RECORDER_EVENT_BEGIN_RECOGNIZE){	
		dlg->m_startRecogClock = clock();	//��ʼʶ��ʱ��¼��ʼʱ��
	}
	if(eRecorderEvent == RECORDER_EVENT_BEGIN_RECORD || eRecorderEvent == RECORDER_EVENT_END_RECORD){	
		QString str(g_sStatus[eRecorderEvent].pszComment);	//����ʼ¼���ͽ���¼��ʱ����������ʾ
		qDebug()<<str;
	}
}
void HCIAPI XCVoice::RecorderRecogFinish(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam){
	XCVoice* dlg = (XCVoice*)pUsrParam;
	if(eRecorderEvent == RECORDER_EVENT_RECOGNIZE_COMPLETE){
		char buff[32];
		clock_t endClock = clock();
		QString recogtime = QString("ʶ��ʱ��:%1 ms").arg((int)endClock - (int)dlg->m_startRecogClock);
		qDebug()<<recogtime;
	}
	QString strMessage = "";
	if( psAsrRecogResult->uiResultItemCount > 0 ){
		unsigned char* pucUTF8 = NULL;
		HciExampleComon::UTF8ToGBK( (unsigned char*)psAsrRecogResult->psResultItemList[0].pszResult, &pucUTF8 );
		QString add;
		string str = (char*)pucUTF8;
		add = QString::fromStdString(str);
		strMessage += "ʶ����:";
		strMessage += add;
		HciExampleComon::FreeConvertResult( pucUTF8 );
		//cJSON��ȡ��Ч����
		QString QResult,QAnswer;
		dlg->Json_Explain(strMessage,QResult,QAnswer);
		strMessage="С��Ļش�:" + QAnswer  + "\n" + "��������:"+ QResult;
		unsigned char* pszUTF8 = NULL;
		HciExampleComon::GBKToUTF8( (unsigned char*)QAnswer.toStdString().c_str(), &pszUTF8 );
		string startConfig = "property=cn_xiaokun_common,tagmode=none,capkey=tts.cloud.wangjing";
		PLAYER_ERR_CODE eRetk = hci_tts_player_start( (const char*)pszUTF8, startConfig.c_str() );
	}else{
		strMessage.append( "*****��ʶ����*****" );
	}
	qDebug()<<strMessage;
}
void HCIAPI XCVoice::RecorderRecogProcess(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam){
	XCVoice* dlg = (XCVoice*)pUsrParam;
	QString str;
	if( psAsrRecogResult->uiResultItemCount > 0 ){
		unsigned char* pucUTF8 = NULL;
		HciExampleComon::UTF8ToGBK( (unsigned char*)psAsrRecogResult->psResultItemList[0].pszResult, &pucUTF8 );
		QString add;
		add.sprintf( "ʶ���м���: %s", pucUTF8 );
		add = QString::fromLocal8Bit(add.toStdString().c_str());
		str += add;
		HciExampleComon::FreeConvertResult( pucUTF8 );
		pucUTF8 = NULL;
	}else{
		str.append( "*****��ʶ����*****" );
	}
}
void HCIAPI XCVoice::RecorderErr(RECORDER_EVENT eRecorderEvent,HCI_ERR_CODE eErrorCode,void *pUsrParam){
	qDebug()<<"ϵͳ����:"<<eErrorCode;
}
void HCIAPI XCVoice::RecorderRecordingCallback(unsigned char * pVoiceData,unsigned int uiVoiceLen,void * pUsrParam){
	
}

//���ASRʶ������Jason��������
void XCVoice::Json_Explain (QString buf,QString& Qresult,QString& Qanswer){
	QString domain = Json_SearchKeyword(buf,"domain");
	if(domain == "weather"){
		Qanswer = "����������" + Json_SearchKeyword(buf,"description");
	}else if(domain == "calendar"){
		Qanswer = "������" + Json_SearchKeyword(buf,"date_gongli") + "��" + Json_SearchKeyword(buf,"date_nongli");
	}else if(domain == "joke" || domain == "story"){
		Qanswer = Json_SearchKeyword(buf,"content\":{\"content");
	}else if(domain == "news"){
		Qanswer = Json_SearchKeyword(buf,"desc");
	}else{
		Qanswer = Json_SearchKeyword(buf,"text");
	}
	Qresult = Json_SearchKeyword(buf,"result");
}
QString XCVoice::Json_SearchKeyword(QString buf,QString keyword){
	QString QAnswer;
	int i=0,j=0,n=3;
	i=buf.indexOf(keyword);
	while(i != -1 && buf.at(i+keyword.length()+n+j) != '"'){
		QAnswer += buf.at(i+keyword.length()+n+j);
		j++;
	}
	return QAnswer;
}