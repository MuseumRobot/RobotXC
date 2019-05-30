#pragma once
#include <list>

using namespace std;

class XCTaskDataType{
public:
	XCTaskDataType(): id(0) {}
	int id;				
	int taskType;			//�������ͣ���������Ϊ1��λ������Ϊ0��
	double x;				//Ŀ����x���꣨���ڵ�ǰ������λ������ʱ��Ч��
	double y;				//Ŀ����y���꣨���ڵ�ǰ������λ������ʱ��Ч��
	int SpeakContentId;		//�������ݵ������ţ����ڵ�ǰ��������������ʱ��Ч��
	int FacingAngle;		//����Ŀ������Ҫ�泯�ķ��򣨽��ڵ�ǰ������λ������ʱ��Ч��
};

typedef list<XCTaskDataType> XCTaskDataTypeList;
