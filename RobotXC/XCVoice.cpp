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
	hci_tts_player_stop();	//后发的语音指令会将之前的语音指令覆盖
	QTest::qSleep(100);		//发送stop指令后留有短暂时间以供stop命令执行完成，否则无法start下一句
	PLAYER_ERR_CODE eRetk = hci_tts_player_start( (const char*)pszUTF8, startConfig.c_str() );
}
void XCVoice::StartRecord(){
	RECORDER_ERR_CODE eRet = RECORDER_ERR_NONE;
	string startConfig = "";
	startConfig += "capkey=" + account_info->cap_key();
	startConfig += ",audioformat=pcm16k16bit";
	if(IS_RECORDER_CONTINUE)	//是否连续录音
		startConfig += ",continuous=yes";	
	if ( m_RecogMode == kRecogModeDialog ){
		startConfig +=",intention=weather;joke;story;baike;calendar;translation;news"; 
	}
	eRet = hci_asr_recorder_start(startConfig.c_str(),"");
	if (RECORDER_ERR_NONE != eRet){
		QString strErrMessage;
		strErrMessage.sprintf( "开始录音失败,错误码%d", eRet );
		QMessageBox msgBox;
		msgBox.setText(strErrMessage);
		msgBox.exec();
	}
}
bool XCVoice::Init(){
	account_info = AccountInfo::GetInstance();				// 获取AccountInfo单例
	string account_info_file = "./Configure/AccountInfo.ini";	// 账号信息读取
	account_info->LoadFromFile(account_info_file);
	// SYS初始化
	HCI_ERR_CODE errCode = HCI_ERR_NONE;
	// 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	string init_config = "";
	init_config += "appKey=" + account_info->app_key();              //灵云应用序号
	init_config += ",developerKey=" + account_info->developer_key(); //灵云开发者密钥
	init_config += ",cloudUrl=" + account_info->cloud_url();         //灵云云服务的接口地址
	init_config += ",authpath=" + account_info->auth_path();         //授权文件所在路径，保证可写
	init_config += ",logfilesize=1024000,loglevel=5";	// 其他配置使用默认值，不再添加，如果想设置可以参考开发手册
	errCode = hci_init( init_config.c_str() );
	m_isAuthReady = CheckAndUpdataAuth();
	m_isTTSReady = InitTTS();
	m_isASRReady = InitASR();
	return true;
}
bool XCVoice::InitTTS(){
	//tts_player初始化
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
		Speak("你好呀！");
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
	//获取过期时间
	int64 nExpireTime;
	int64 nCurTime = (int64)time( NULL );
	HCI_ERR_CODE errCode = hci_get_auth_expire_time( &nExpireTime );
	qDebug()<<"get auth expire time return("<<errCode<<":"<<hci_get_error_info(errCode)<<")";
	if( errCode == HCI_ERR_NONE ){
		//获取成功则判断是否过期
		if( nExpireTime > nCurTime ){	//没有过期
			return true;
		}
	}
	//获取过期时间失败或已经过期
	//手动调用更新授权
	errCode = hci_check_auth();
	qDebug()<<"check auth return("<<errCode<<":"<<hci_get_error_info(errCode)<<")";
	if( errCode == HCI_ERR_NONE ){	//更新成功
		return true;
	}
	else{	//更新失败
		return false;
	}
}
void XCVoice::GetASRCapkeyProperty(const string &cap_key,AsrRecogType &type,AsrRecogMode &mode){
	HCI_ERR_CODE errCode = HCI_ERR_NONE;
	CAPABILITY_ITEM *pItem = NULL;
	// 枚举所有的asr能力
	CAPABILITY_LIST list = {0};
	if ((errCode = hci_get_capability_list("asr", &list))!= HCI_ERR_NONE){		// 没有找到相应的能力。
		return;
	}
	// 获取asr能力配置信息。
	for (int i = 0; i < list.uiItemCount; i++){
		if (list.pItemList[i].pszCapKey != NULL && stricmp(list.pItemList[i].pszCapKey, cap_key.c_str()) == 0){
			pItem = &list.pItemList[i];
			break;
		}
	}
	// 没有获取相应能力配置，返回。
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

//TTS回调函数实现
void HCIAPI XCVoice::CB_EventChange(_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_OPT_ _IN_ void * pUsrParam){
	string strEvent;
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "开始播放";break;
	case PLAYER_EVENT_PAUSE:strEvent = "暂停播放"; break;
	case PLAYER_EVENT_RESUME:strEvent = "恢复播放";break;
	case PLAYER_EVENT_PROGRESS:strEvent = "播放进度";break;
	case PLAYER_EVENT_BUFFERING:strEvent = "播放缓冲";break;
	case PLAYER_EVENT_END:strEvent = "播放完毕";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "引擎出错";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "设备出错";break;
	}
}
void HCIAPI XCVoice::CB_ProgressChange (_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ int nStart,_MUST_ _IN_ int nStop,_OPT_ _IN_ void * pUsrParam){
	string strEvent;
	char szData[256] = {0};
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "开始播放";break;
	case PLAYER_EVENT_PAUSE:strEvent = "暂停播放";break;
	case PLAYER_EVENT_RESUME:strEvent = "恢复播放";break;
	case PLAYER_EVENT_PROGRESS:sprintf( szData, "播放进度：起始=%d,终点=%d", nStart, nStop );strEvent = szData;break;
	case PLAYER_EVENT_BUFFERING:strEvent = "播放缓冲";break;
	case PLAYER_EVENT_END:strEvent = "播放完毕";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "引擎出错";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "设备出错";break;
	}
}
void HCIAPI XCVoice::CB_SdkErr( _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ HCI_ERR_CODE eErrorCode,_OPT_ _IN_ void * pUsrParam ){
	string strEvent;
	switch ( ePlayerEvent ){
	case PLAYER_EVENT_BEGIN:strEvent = "开始播放";break;
	case PLAYER_EVENT_PAUSE:strEvent = "暂停播放";break;
	case PLAYER_EVENT_RESUME:strEvent = "恢复播放";break;
	case PLAYER_EVENT_PROGRESS:strEvent = "播放进度";break;
	case PLAYER_EVENT_BUFFERING:strEvent = "播放缓冲";break;
	case PLAYER_EVENT_END:strEvent = "播放完毕";break;
	case PLAYER_EVENT_ENGINE_ERROR:strEvent = "引擎出错";break;
	case PLAYER_EVENT_DEVICE_ERROR:strEvent = "设备出错";break;
	}
}

//ASR回调函数描述中间状态的结构体及回调函数实现
struct{
	char* pszName;
	char* pszComment;
}g_sStatus[] ={
	{"RECORDER_EVENT_BEGIN_RECORD",         "开始聆听..."},
	{"RECORDER_EVENT_HAVING_VOICE",         "听到声音 检测到始端的时候会触发该事件"},
	{"RECORDER_EVENT_NO_VOICE_INPUT",       "没有听到声音"},
	{"RECORDER_EVENT_BUFF_FULL",            "缓冲区已填满"},
	{"RECORDER_EVENT_END_RECORD",           "聆听完毕！"},
	{"RECORDER_EVENT_BEGIN_RECOGNIZE",      "开始识别"},
	{"RECORDER_EVENT_RECOGNIZE_COMPLETE",   "识别完毕"},
	{"RECORDER_EVENT_ENGINE_ERROR",         "引擎出错"},
	{"RECORDER_EVENT_DEVICE_ERROR",         "设备出错"},
	{"RECORDER_EVENT_MALLOC_ERROR",         "分配空间失败"},
	{"RECORDER_EVENT_INTERRUPTED",          "内部错误"},
	{"RECORDER_EVENT_PERMISSION_DENIED",    "内部错误"},
	{"RECORDER_EVENT_TASK_FINISH",          "识别任务结束"},
	{"RECORDER_EVENT_RECOGNIZE_PROCESS",    "识别中间状态"}
};
void HCIAPI XCVoice::RecordEventChange(RECORDER_EVENT eRecorderEvent, void *pUsrParam){
	XCVoice* dlg = (XCVoice*)pUsrParam;
	if(eRecorderEvent == RECORDER_EVENT_BEGIN_RECOGNIZE){	
		dlg->m_startRecogClock = clock();	//开始识别时记录起始时间
	}
	if(eRecorderEvent == RECORDER_EVENT_BEGIN_RECORD || eRecorderEvent == RECORDER_EVENT_END_RECORD){	
		QString str(g_sStatus[eRecorderEvent].pszComment);	//当开始录音和结束录音时给予文字提示
		qDebug()<<str;
	}
}
void HCIAPI XCVoice::RecorderRecogFinish(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam){
	XCVoice* dlg = (XCVoice*)pUsrParam;
	if(eRecorderEvent == RECORDER_EVENT_RECOGNIZE_COMPLETE){
		char buff[32];
		clock_t endClock = clock();
		QString recogtime = QString("识别时间:%1 ms").arg((int)endClock - (int)dlg->m_startRecogClock);
		qDebug()<<recogtime;
	}
	QString strMessage = "";
	if( psAsrRecogResult->uiResultItemCount > 0 ){
		unsigned char* pucUTF8 = NULL;
		HciExampleComon::UTF8ToGBK( (unsigned char*)psAsrRecogResult->psResultItemList[0].pszResult, &pucUTF8 );
		QString add;
		string str = (char*)pucUTF8;
		add = QString::fromStdString(str);
		strMessage += "识别结果:";
		strMessage += add;
		HciExampleComon::FreeConvertResult( pucUTF8 );
		//cJSON提取有效内容
		QString QResult,QAnswer;
		dlg->Json_Explain(strMessage,QResult,QAnswer);
		strMessage="小灵的回答:" + QAnswer  + "\n" + "您的提问:"+ QResult;
		unsigned char* pszUTF8 = NULL;
		HciExampleComon::GBKToUTF8( (unsigned char*)QAnswer.toStdString().c_str(), &pszUTF8 );
		string startConfig = "property=cn_xiaokun_common,tagmode=none,capkey=tts.cloud.wangjing";
		PLAYER_ERR_CODE eRetk = hci_tts_player_start( (const char*)pszUTF8, startConfig.c_str() );
	}else{
		strMessage.append( "*****无识别结果*****" );
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
		add.sprintf( "识别中间结果: %s", pucUTF8 );
		add = QString::fromLocal8Bit(add.toStdString().c_str());
		str += add;
		HciExampleComon::FreeConvertResult( pucUTF8 );
		pucUTF8 = NULL;
	}else{
		str.append( "*****无识别结果*****" );
	}
}
void HCIAPI XCVoice::RecorderErr(RECORDER_EVENT eRecorderEvent,HCI_ERR_CODE eErrorCode,void *pUsrParam){
	qDebug()<<"系统错误:"<<eErrorCode;
}
void HCIAPI XCVoice::RecorderRecordingCallback(unsigned char * pVoiceData,unsigned int uiVoiceLen,void * pUsrParam){
	
}

//针对ASR识别结果的Jason解析函数
void XCVoice::Json_Explain (QString buf,QString& Qresult,QString& Qanswer){
	QString domain = Json_SearchKeyword(buf,"domain");
	if(domain == "weather"){
		Qanswer = "今天天气：" + Json_SearchKeyword(buf,"description");
	}else if(domain == "calendar"){
		Qanswer = "今天是" + Json_SearchKeyword(buf,"date_gongli") + "，" + Json_SearchKeyword(buf,"date_nongli");
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