#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tracert.h"
#include <ws2tcpip.h>

#include <winsock2.h>
#include <math.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32")
#include <iostream>
#include <iomanip>

//ICMP�����ײ�
typedef struct
{
	BYTE type;  //8λ����
	BYTE code;  //8λ����
	USHORT cksum;  //16λУ���
	USHORT id;   //16λ��ʶ��
	USHORT seq;  //16λ���к�
} ICMP_HEADER;

//������
typedef struct
{
	USHORT usSeqNo;   //�����к�
	DWORD dwRoundTripTime; //����ʱ��
	DWORD startTimeStamp;	//��ʼʱ��
	in_addr dwIPaddr, hostinfo;  //�Զ�IP��ַ,����IP
	DWORD rtt;   		//����ʱ��

} DECODE_RESULT;

class Tracert : public QMainWindow
{
	Q_OBJECT
		
public:

	Tracert(QWidget *parent = Q_NULLPTR);

public slots:
	void  tracerting();

	

private:
	Ui::TracertClass ui;
	QString color(QString time);
	int Run(int argc, char * argv[]);
	USHORT GenerateChecksum(USHORT * pBuf, int iSize);
	BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT & stDecodeResult);
};

