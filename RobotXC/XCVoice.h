#pragma once
#include "include/hci_sys.h"
#include "include/hci_tts.h"
#include "include/hci_tts_player.h"
#include "include/hci_asr.h"
#include "include/hci_asr_recorder.h"
#include "include/CommonTool.h"
#include "include/AccountInfo.h"

#define IS_RECORDER_CONTINUE FALSE	//是否连续录音

typedef enum _tag_AsrRecogType{
	kRecogTypeUnkown = -1,      //未知类型
	kRecogTypeCloud = 0,        //云端识别
	kRecogTypeLocal = 1,        //本地识别
}AsrRecogType;
typedef enum _tag_AsrRecogMode{
	kRecogModeUnkown = -1,      //未知类型
	kRecogModeFreetalk = 0,     //自由说
	kRecogModeGrammar = 1,      //语法识别
	kRecogModeDialog = 2,
}AsrRecogMode;
class XCVoice{
public:
	XCVoice(void);
	~XCVoice(void);
	void StartRecord();
	bool m_isAuthReady;
	bool m_isTTSReady;
	bool m_isASRReady;
	void Speak(QString str);
	void StopSpeak();
private:
	AccountInfo *account_info;
	AsrRecogType m_RecogType;
	AsrRecogMode m_RecogMode;
	clock_t m_startRecogClock;	//记录ASR开始识别的时间
	bool Init();
	bool InitTTS();
	bool InitASR();
	bool CheckAndUpdataAuth();	//检查授权是否过期
	void GetASRCapkeyProperty(const string &cap_key,AsrRecogType &type,AsrRecogMode &mode);	//获取ASR具体能力
	//TTS回调函数
	static void HCIAPI CB_EventChange(_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_OPT_ _IN_ void * pUsrParam);
	static void HCIAPI CB_ProgressChange (_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ int nStart,
		_MUST_ _IN_ int nStop,_OPT_ _IN_ void * pUsrParam);
	static void HCIAPI CB_SdkErr( _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ HCI_ERR_CODE eErrorCode,_OPT_ _IN_ void * pUsrParam );
	//ASR回调函数
	static void HCIAPI RecordEventChange(RECORDER_EVENT eRecorderEvent, void *pUsrParam);
	static void HCIAPI RecorderRecogFinish(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam);
	static void HCIAPI RecorderRecogProcess(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam);
	static void HCIAPI RecorderErr(RECORDER_EVENT eRecorderEvent,HCI_ERR_CODE eErrorCode,void *pUsrParam);
	static void HCIAPI RecorderRecordingCallback(unsigned char * pVoiceData,unsigned int uiVoiceLen,void * pUsrParam);
	//针对ASR识别结果的Jason解析函数
	void Json_Explain (QString buf,QString& Qresult,QString& Qanswer);
	QString Json_SearchKeyword(QString buf,QString keyword);
};