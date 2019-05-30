#pragma once
#include "include/hci_sys.h"
#include "include/hci_tts.h"
#include "include/hci_tts_player.h"
#include "include/hci_asr.h"
#include "include/hci_asr_recorder.h"
#include "include/CommonTool.h"
#include "include/AccountInfo.h"

#define IS_RECORDER_CONTINUE FALSE	//�Ƿ�����¼��

typedef enum _tag_AsrRecogType{
	kRecogTypeUnkown = -1,      //δ֪����
	kRecogTypeCloud = 0,        //�ƶ�ʶ��
	kRecogTypeLocal = 1,        //����ʶ��
}AsrRecogType;
typedef enum _tag_AsrRecogMode{
	kRecogModeUnkown = -1,      //δ֪����
	kRecogModeFreetalk = 0,     //����˵
	kRecogModeGrammar = 1,      //�﷨ʶ��
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
	clock_t m_startRecogClock;	//��¼ASR��ʼʶ���ʱ��
	bool Init();
	bool InitTTS();
	bool InitASR();
	bool CheckAndUpdataAuth();	//�����Ȩ�Ƿ����
	void GetASRCapkeyProperty(const string &cap_key,AsrRecogType &type,AsrRecogMode &mode);	//��ȡASR��������
	//TTS�ص�����
	static void HCIAPI CB_EventChange(_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_OPT_ _IN_ void * pUsrParam);
	static void HCIAPI CB_ProgressChange (_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ int nStart,
		_MUST_ _IN_ int nStop,_OPT_ _IN_ void * pUsrParam);
	static void HCIAPI CB_SdkErr( _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,_MUST_ _IN_ HCI_ERR_CODE eErrorCode,_OPT_ _IN_ void * pUsrParam );
	//ASR�ص�����
	static void HCIAPI RecordEventChange(RECORDER_EVENT eRecorderEvent, void *pUsrParam);
	static void HCIAPI RecorderRecogFinish(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam);
	static void HCIAPI RecorderRecogProcess(RECORDER_EVENT eRecorderEvent,ASR_RECOG_RESULT *psAsrRecogResult,void *pUsrParam);
	static void HCIAPI RecorderErr(RECORDER_EVENT eRecorderEvent,HCI_ERR_CODE eErrorCode,void *pUsrParam);
	static void HCIAPI RecorderRecordingCallback(unsigned char * pVoiceData,unsigned int uiVoiceLen,void * pUsrParam);
	//���ASRʶ������Jason��������
	void Json_Explain (QString buf,QString& Qresult,QString& Qanswer);
	QString Json_SearchKeyword(QString buf,QString keyword);
};